/*--------------------------------------------------------------------
	Atari Audio Library
	Small & accurate ATARI-ST audio emulation
	Arnaud Carré aka Leonard/Oxygene
	@leonard_coder
--------------------------------------------------------------------*/
// Tiny & cycle accurate ym2149 emulation.
// operate at original YM freq divided by 8 (so 250Khz, as nothing runs faster in the chip)
#include <assert.h>
#include "ym2149c.h"
#include "ym2149_tables.h"

// fix the high pitch issue with quartet-like STF code where square waves are active at highest frequency (period=0)
#define	D_DOWNSAMPLE_USE_MAX_VALUE		1

static uint32_t sRndSeed = 1;
uint16_t stdLibRand()
{
	sRndSeed = sRndSeed*214013+2531011;
	return uint16_t((sRndSeed >> 16) & 0x7fff);
}

void	Ym2149c::Reset(uint32_t hostReplayRate, uint32_t ymClock)
{
	InitYmTable(s_ymMixingVolumeTable, sSTReplayTable);
	for (int v = 0; v < 3; v++)
	{
		m_toneCounter[v] = 0;
		m_tonePeriod[v] = 0;
	}
	m_toneEdges = (stdLibRand()&0x111)*0xf;		// YM internal edge state are un-predictable
	m_insideTimerIrq = false;
	m_hostReplayRate = hostReplayRate;
	m_ymClockOneEighth = ymClock/8;
	m_resamplingDividor = (hostReplayRate << 12) / m_ymClockOneEighth;
	m_noiseRndRack = 1;
	m_noiseEnvHalf = 0;
	for (auto& dcAdjust : m_dcAdjust)
		dcAdjust = {};
	for (int r=0;r<14;r++)
		WriteReg(r, (7==r)?0x3f:0);
	m_selectedReg = 0;
	m_currentLevel = 0;
	m_innerCycle = 0;
	m_envPos = 0;
	m_currentDebugThreeVoices = 0;
}

void	Ym2149c::WritePort(uint8_t port, uint8_t value)
{
	if (port & 2)
		WriteReg(m_selectedReg, value);
	else
		m_selectedReg = value;
}

void	Ym2149c::WriteReg(int reg, uint8_t value)
{
	if ((unsigned int)reg < 14)
	{
		static const uint8_t regMask[14] = { 0xff,0x0f,0xff,0x0f,0xff,0x0f,0x1f,0x3f,0x1f,0x1f,0x1f,0xff,0xff,0x0f };
		m_regs[reg] = value & regMask[reg];

		switch (reg)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		{
			const int voice = reg >> 1;
			m_tonePeriod[voice] = (m_regs[voice * 2 + 1] << 8) | m_regs[voice * 2];

			// when period change and counter is above, counter will restart at next YM cycle
			// but there is some delay if you do that on 3 voices using CPU. Simulate this delay
			// with empirical offset
			if (m_toneCounter[voice] >= m_tonePeriod[voice])
				m_toneCounter[voice] = voice * 8;

			if (m_tonePeriod[voice] <= 1)
			{
				if (m_insideTimerIrq)
					m_edgeNeedReset[voice] = true;
			}
			break;
		}
		case 6:
			m_noisePeriod = m_regs[6];
			break;
		case 7:
		{
			static const uint32_t masks[8] = { 0x000,0x00f,0x0f0, 0x0ff, 0xf00, 0xf0f, 0xff0, 0xfff };
			m_toneMask = masks[value & 0x7];
			m_noiseMask = masks[(value >> 3) & 0x7];
		}
			break;
		case 11:
		case 12:
			m_envPeriod = (m_regs[12] << 8) | m_regs[11];
			break;
		case 13:
		{
			static const uint8_t shapeToEnv[16] = { 0,0,0,0,1,1,1,1,2,3,4,5,6,7,8,9 };
			m_pCurrentEnv = s_envData + shapeToEnv[m_regs[13]] * 64;
			m_envPos = -32;
			m_envCounter = 0;
			break;
		}
		default:
			break;
		}
	}
}

uint8_t Ym2149c::ReadPort(uint8_t port) const
{
	if (0 == (port & 2))
		return m_regs[m_selectedReg];
	return ~0;
}

Ym2149c::Levels	Ym2149c::dcAdjust(Levels v)
{
	Levels ov = { 0 };
	for (int i = 0; i < 3; i++)
	{
		m_dcAdjust[i].sum -= m_dcAdjust[i].buffer[m_dcAdjust[i].pos];
		m_dcAdjust[i].sum += v.uLevels[i];
		m_dcAdjust[i].buffer[m_dcAdjust[i].pos] = v.uLevels[i];
		m_dcAdjust[i].pos++;
		m_dcAdjust[i].pos &= (1 << kDcAdjustHistoryBit) - 1;
		ov.sLevels[i] = int16_t(int32_t(v.uLevels[i]) - int32_t(m_dcAdjust[i].sum >> kDcAdjustHistoryBit));
	}
	// max amplitude is 15bits (not 16) so dc adjuster should never overshoot
	return ov;
}

// Tick internal YM2149 state machine at 250Khz ( 2Mhz/8 )
uint16_t Ym2149c::Tick()
{
	// three voices at same time
	const uint32_t vmask = (m_toneEdges | m_toneMask) & (m_currentNoiseMask | m_noiseMask);

	// update internal state
	for (int v = 0; v < 3; v++)
	{
		m_toneCounter[v]++;
		if (m_toneCounter[v] >= m_tonePeriod[v])
		{
			m_toneEdges ^= 0xf<<(v*4);
			m_toneCounter[v] = 0;
		}
	}

	// noise & env state machine is running half speed
	m_noiseEnvHalf ^= 1;
	if (m_noiseEnvHalf)
	{
		// noise
		m_noiseCounter++;
		if (m_noiseCounter >= m_noisePeriod)
		{
			m_currentNoiseMask = ((m_noiseRndRack ^ (m_noiseRndRack >> 2))&1) ? ~0 : 0;
			m_noiseRndRack = (m_noiseRndRack >> 1) | ((m_currentNoiseMask&1) << 16);
			m_noiseCounter = 0;
		}

		m_envCounter++;
		if ( m_envCounter >= m_envPeriod )
		{
			m_envPos++;
			if (m_envPos > 0)
				m_envPos &= 31;
			m_envCounter = 0;
		}
	}
	return vmask;
}

// called at host replay rate ( like 48Khz )
// internally update YM chip state machine at 250Khz and average output for each host sample
Ym2149c::Levels Ym2149c::ComputeNextSample(uint32_t* pSampleDebugInfo)
{
#if D_DOWNSAMPLE_USE_MAX_VALUE
	uint16_t highMask = 0;
	do
	{
		highMask |= Tick();
		m_innerCycle += m_hostReplayRate;
	} while (m_innerCycle < m_ymClockOneEighth);
	m_innerCycle -= m_ymClockOneEighth;

	const uint32_t envLevel = m_pCurrentEnv[m_envPos + 32];
	uint32_t levels[3];	//mono (a+b+c), left (a+c), right (b)
	levels[0] = ((m_regs[8] & 0x10) ? envLevel : m_regs[8]) << 0;
	levels[0] |= ((m_regs[9] & 0x10) ? envLevel : m_regs[9]) << 4;
	levels[0] |= ((m_regs[10] & 0x10) ? envLevel : m_regs[10]) << 8;
	levels[1] = ((m_regs[8] & 0x10) ? envLevel : m_regs[8]) << 0;
	levels[2] = ((m_regs[9] & 0x10) ? envLevel : m_regs[9]) << 4;
	levels[1] |= ((m_regs[10] & 0x10) ? envLevel : m_regs[10]) << 8;

	Levels out;
	for (int i = 0; i < 3; i++)
	{
		levels[i] &= highMask;
		assert(levels[i] < 0x1000);
		out.uLevels[i] = s_ymMixingVolumeTable[levels[i]];
	}
	out = dcAdjust(out);
	if (pSampleDebugInfo)
		*pSampleDebugInfo = s444to888[levels[0]];
#else
	// down-sample by averaging samples
	uint16_t tickLevel;
	for (;;)
	{
		tickLevel = Tick();
		m_innerCycle += m_hostReplayRate;
		if (m_innerCycle >= m_ymClockOneEighth)
			break;
		m_currentLevel += tickLevel;
	}

	m_innerCycle -= m_ymClockOneEighth;
	const int beta = (m_innerCycle << 12) / m_hostReplayRate;
	const int alpha = (1 << 12) - beta;

	m_currentLevel += (tickLevel * alpha) >> 12;
	m_currentLevel = (m_currentLevel * m_resamplingDividor) >> 12;	// average divide by (250Khz/replayrate)
	int16_t out = dcAdjust(m_currentLevel);
	m_currentLevel = (tickLevel * beta) >> 12;	// keep reminder for next call
#endif
	return out;
}

void	Ym2149c::InsideTimerIrq(bool inside)
{
	if (!inside)
	{
		// when exiting timer IRQ code, do any pending edge reset ( "square-sync" modern fx )
		for (int v = 0; v < 3; v++)
		{
			if (m_edgeNeedReset[v])
			{
				m_toneEdges ^= 0xf<<(v*4);
				m_toneCounter[v] = 0;
				m_edgeNeedReset[v] = false;
			}
		}
	}
	m_insideTimerIrq = inside;
}
