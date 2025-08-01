/*
	GameFX (C)2005-2009 by jme
	Programm is based on Arguru Bass. Filter seems to be Public Domain.

	This plugin is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.\n"\

	This plugin is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <psycle/plugin_interface.hpp>
#include "voice.h"
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstdio>
namespace psycle::plugins::jme::gamefx13 {
using namespace psycle::plugin_interface;

#define MAX_ENV_TIME 65536
#define GAMEFX_VERSION "1.3.1"
int const IGAMEFX_VERSION =0x0131;

CMachineParameter const paraVol1 = {"Volume 1", "Volume 1", 0, 255, MPF_STATE, 129};
CMachineParameter const paraVol2 = {"Volume 2", "Volume 2", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol3 = {"Volume 3", "Volume 3", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol4 = {"Volume 4", "Volume 4", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol5 = {"Volume 5", "Volume 5", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol6 = {"Volume 6", "Volume 6", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol7 = {"Volume 7", "Volume 7", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol8 = {"Volume 8", "Volume 8", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol9 = {"Volume 9", "Volume 9", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol10 = {"Volume 10", "Volume 10", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol11 = {"Volume 11", "Volume 11", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol12 = {"Volume 12", "Volume 12", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol13 = {"Volume 13", "Volume 13", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol14 = {"Volume 14", "Volume 14", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol15 = {"Volume 15", "Volume 15", 0, 255, MPF_STATE, 0};
CMachineParameter const paraVol16 = {"Volume 16", "Volume 16", 0, 255, MPF_STATE, 0};

CMachineParameter const paraWave1 = {"Wave 1", "Wave 1", 0, 9, MPF_STATE, 1};
CMachineParameter const paraWave2 = {"Wave 2", "Wave 2", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave3 = {"Wave 3", "Wave 3", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave4 = {"Wave 4", "Wave 4", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave5 = {"Wave 5", "Wave 5", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave6 = {"Wave 6", "Wave 6", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave7 = {"Wave 7", "Wave 7", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave8 = {"Wave 8", "Wave 8", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave9 = {"Wave 9", "Wave 9", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave10 = {"Wave 10", "Wave 10", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave11 = {"Wave 11", "Wave 11", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave12 = {"Wave 12", "Wave 12", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave13 = {"Wave 13", "Wave 13", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave14 = {"Wave 14", "Wave 14", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave15 = {"Wave 15", "Wave 15", 0, 9, MPF_STATE, 0};
CMachineParameter const paraWave16 = {"Wave 16", "Wave 16", 0, 9, MPF_STATE, 0};

CMachineParameter const paraTranspose1 = {"Transpose 1", "Transpose 1", 0, 96, MPF_STATE, 1};
CMachineParameter const paraTranspose2 = {"Transpose 2", "Transpose 2", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose3 = {"Transpose 3", "Transpose 3", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose4 = {"Transpose 4", "Transpose 4", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose5 = {"Transpose 5", "Transpose 5", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose6 = {"Transpose 6", "Transpose 6", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose7 = {"Transpose 7", "Transpose 7", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose8 = {"Transpose 8", "Transpose 8", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose9 = {"Transpose 9", "Transpose 9", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose10 = {"Transpose 10", "Transpose 10", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose11 = {"Transpose 11", "Transpose 11", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose12 = {"Transpose 12", "Transpose 12", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose13 = {"Transpose 13", "Transpose 13", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose14 = {"Transpose 14", "Transpose 14", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose15 = {"Transpose 15", "Transpose 15", 0, 96, MPF_STATE, 0};
CMachineParameter const paraTranspose16 = {"Transpose 16", "Transpose 16", 0, 96, MPF_STATE, 0};

CMachineParameter const paraOption1 = {"Option 1", "Option 1", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption2 = {"Option 2", "Option 2", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption3 = {"Option 3", "Option 3", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption4 = {"Option 4", "Option 4", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption5 = {"Option 5", "Option 5", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption6 = {"Option 6", "Option 6", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption7 = {"Option 7", "Option 7", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption8 = {"Option 8", "Option 8", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption9 = {"Option 9", "Option 9", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption10 = {"Option 10", "Option 10", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption11 = {"Option 11", "Option 11", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption12 = {"Option 12", "Option 12", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption13 = {"Option 13", "Option 13", 0, 3, MPF_STATE,0};
CMachineParameter const paraOption14 = {"Option 14", "Option 14", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption15 = {"Option 15", "Option 15", 0, 3, MPF_STATE, 0};
CMachineParameter const paraOption16 = {"Option 16", "Option 16", 0, 3, MPF_STATE, 0};

CMachineParameter const paraCommand1 = {"Command 1", "Command 1", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand2 = {"Command 2", "Command 2", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand3 = {"Command 3", "Command 3", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand4 = {"Command 4", "Command 4", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand5 = {"Command 5", "Command 5", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand6 = {"Command 6", "Command 6", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand7 = {"Command 7", "Command 7", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand8 = {"Command 8", "Command 8", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand9 = {"Command 9", "Command 9", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand10 = {"Command 10", "Command 10", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand11 = {"Command 11", "Command 11", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand12 = {"Command 12", "Command 12", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand13 = {"Command 13", "Command 13", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand14 = {"Command 14", "Command 14", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand15 = {"Command 15", "Command 15", 0, 15, MPF_STATE, 0};
CMachineParameter const paraCommand16 = {"Command 16", "Command 16", 0, 15, MPF_STATE, 0};

CMachineParameter const paraParameter1 = {"Parameter 1", "Parameter 1", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter2 = {"Parameter 2", "Parameter 2", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter3 = {"Parameter 3", "Parameter 3", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter4 = {"Parameter 4", "Parameter 4", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter5 = {"Parameter 5", "Parameter 5", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter6 = {"Parameter 6", "Parameter 6", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter7 = {"Parameter 7", "Parameter 7", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter8 = {"Parameter 8", "Parameter 8", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter9 = {"Parameter 9", "Parameter 9", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter10 = {"Parameter 10", "Parameter 10", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter11 = {"Parameter 11", "Parameter 11", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter12 = {"Parameter 12", "Parameter 12", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter13 = {"Parameter 13", "Parameter 13", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter14 = {"Parameter 14", "Parameter 14", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter15 = {"Parameter 15", "Parameter 15", 0, 255, MPF_STATE, 0};
CMachineParameter const paraParameter16 = {"Parameter 16", "Parameter 16", 0, 255, MPF_STATE, 0};

CMachineParameter const paraSpeed1 = {"Duration 1", "Duration 1", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed2 = {"Duration 2", "Duration 2", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed3 = {"Duration 3", "Duration 3", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed4 = {"Duration 4", "Duration 4", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed5 = {"Duration 5", "Duration 5", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed6 = {"Duration 6", "Duration 6", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed7 = {"Duration 7", "Duration 7", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed8 = {"Duration 8", "Duration 8", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed9 = {"Duration 9", "Duration 9", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed10 = {"Duration 10", "Duration 10", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed11 = {"Duration 11", "Duration 11", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed12 = {"Duration 12", "Duration 12", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed13 = {"Duration 13", "Duration 13", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed14 = {"Duration 14", "Duration 14", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed15 = {"Duration 15", "Duration 15", 0, 255, MPF_STATE, 0};
CMachineParameter const paraSpeed16 = {"Duration 16", "Duration 16", 0, 255, MPF_STATE, 0};

CMachineParameter const paraStartPos = {"Start Pos", "Start Pos", 1, 16, MPF_STATE, 1};
CMachineParameter const paraLoopStart = {"Loop Start", "Loop Start", 1, 16, MPF_STATE, 1};
CMachineParameter const paraLoopEnd = {"Loop End", "Loop End", 1, 16, MPF_STATE, 1};
CMachineParameter const paraReplaySpeed = {"Replay Speed", "Replay Speed", 1, 255, MPF_STATE, 128};
CMachineParameter const paraVCAattack = {"VCA Attack", "VCA Attack", 32, MAX_ENV_TIME, MPF_STATE, 32};
CMachineParameter const paraVCAdecay = {"VCA Decay", "VCA Decay", 32, MAX_ENV_TIME, MPF_STATE, 6341};
CMachineParameter const paraVCAsustain = {"VCA Sustain", "VCA Sustain level", 0, 256, MPF_STATE, 192};
CMachineParameter const paraVCArelease = {"VCA Release", "VCA Release", 32, MAX_ENV_TIME, MPF_STATE, 2630};
CMachineParameter const paraVCFcutoff = {"VCF Cutoff", "VCF Cutoff", 0, 255, MPF_STATE, 120};
CMachineParameter const paraVCFresonance = {"VCF Resonance", "VCF Resonance", 1, 240, MPF_STATE, 1};
CMachineParameter const paraVCFenvmod = {"VCF Envmod", "VCF Envmod", 0, 255, MPF_STATE, 80};
CMachineParameter const paraVCFattack = {"VCF Attack", "VCF Attack", 32, MAX_ENV_TIME, MPF_STATE, 589};
CMachineParameter const paraVCFdecay = {"VCF Decay", "VCF Decay", 32, MAX_ENV_TIME, MPF_STATE, 2630};
CMachineParameter const paraVCFsustain = {"VCF Sustain", "VCF Sustain level", 0, 256, MPF_STATE, 0};
CMachineParameter const paraVCFrelease = {"VCF Release", "VCF Release", 32, MAX_ENV_TIME, MPF_STATE, 2630};
CMachineParameter const paraFinetune = {"Finetune", "Finetune", -256, 256, MPF_STATE, 0};

CMachineParameter const *pParameters[] = 
{ 
	&paraVol1,
	&paraVol2,
	&paraVol3,
	&paraVol4,
	&paraVol5,
	&paraVol6,
	&paraVol7,
	&paraVol8,
	&paraVol9,
	&paraVol10,
	&paraVol11,
	&paraVol12,
	&paraVol13,
	&paraVol14,
	&paraVol15,
	&paraVol16,
	&paraWave1,
	&paraWave2,
	&paraWave3,
	&paraWave4,
	&paraWave5,
	&paraWave6,
	&paraWave7,
	&paraWave8,
	&paraWave9,
	&paraWave10,
	&paraWave11,
	&paraWave12,
	&paraWave13,
	&paraWave14,
	&paraWave15,
	&paraWave16,
	&paraTranspose1,
	&paraTranspose2,
	&paraTranspose3,
	&paraTranspose4,
	&paraTranspose5,
	&paraTranspose6,
	&paraTranspose7,
	&paraTranspose8,
	&paraTranspose9,
	&paraTranspose10,
	&paraTranspose11,
	&paraTranspose12,
	&paraTranspose13,
	&paraTranspose14,
	&paraTranspose15,
	&paraTranspose16,
	&paraOption1,
	&paraOption2,
	&paraOption3,
	&paraOption4,
	&paraOption5,
	&paraOption6,
	&paraOption7,
	&paraOption8,
	&paraOption9,
	&paraOption10,
	&paraOption11,
	&paraOption12,
	&paraOption13,
	&paraOption14,
	&paraOption15,
	&paraOption16,
	&paraCommand1,
	&paraCommand2,
	&paraCommand3,
	&paraCommand4,
	&paraCommand5,
	&paraCommand6,
	&paraCommand7,
	&paraCommand8,
	&paraCommand9,
	&paraCommand10,
	&paraCommand11,
	&paraCommand12,
	&paraCommand13,
	&paraCommand14,
	&paraCommand15,
	&paraCommand16,
	&paraParameter1,
	&paraParameter2,
	&paraParameter3,
	&paraParameter4,
	&paraParameter5,
	&paraParameter6,
	&paraParameter7,
	&paraParameter8,
	&paraParameter9,
	&paraParameter10,
	&paraParameter11,
	&paraParameter12,
	&paraParameter13,
	&paraParameter14,
	&paraParameter15,
	&paraParameter16,
	&paraSpeed1,
	&paraSpeed2,
	&paraSpeed3,
	&paraSpeed4,
	&paraSpeed5,
	&paraSpeed6,
	&paraSpeed7,
	&paraSpeed8,
	&paraSpeed9,
	&paraSpeed10,
	&paraSpeed11,
	&paraSpeed12,
	&paraSpeed13,
	&paraSpeed14,
	&paraSpeed15,
	&paraSpeed16,
	&paraStartPos,
	&paraLoopStart,
	&paraLoopEnd,
	&paraReplaySpeed,
	&paraVCAattack,
	&paraVCAdecay,
	&paraVCAsustain,
	&paraVCArelease,
	&paraVCFcutoff,
	&paraVCFresonance,
	&paraVCFenvmod,
	&paraVCFattack,
	&paraVCFdecay,
	&paraVCFsustain,
	&paraVCFrelease,
	&paraFinetune,
};

CMachineInfo const MacInfo (
	MI_VERSION,
	IGAMEFX_VERSION,
	GENERATOR,
	sizeof pParameters / sizeof *pParameters,
	pParameters,
	"GameFX ver. " GAMEFX_VERSION
	#if !defined NDEBUG
		" (debug)"
	#endif
	,
	"GameFX",
	"jme",
	"Help",
	8
);

class mi : public CMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Init();
	virtual void Work(float *psamplesleft, float* psamplesright, int numsamples, int tracks);
	virtual bool DescribeValue(char* txt,int const param, int const value);
	virtual void Command();
	virtual void ParameterTweak(int par, int val);
	virtual void SeqTick(int channel, int note, int ins, int cmd, int val);
	virtual void Stop();
private:
	void InitWaveTable();

	CSynthTrack track[MAX_TRACKS];
	PERFORMANCE globals;
};

PSYCLE__PLUGIN__INSTANTIATOR("gamefx13", mi, MacInfo)

mi::mi()
{
	Vals=new int[MacInfo.numParameters];

	//Initialize here only those things that don't depend on
	//external values (like sampling rate)
	memset(&globals,0,sizeof(PERFORMANCE));
	//this prevents a warning in valgrind when calling parameterTweak
	for(int i=0; i < MacInfo.numParameters; i++) {
		Vals[i]=0;
	}

	InitWaveTable();
}

mi::~mi()
{
	delete[] Vals;
}

void mi::Init()
{
	globals.reg = 0x7ffff8; // init noise register
	globals.noiseindex = 0;
}

void mi::Stop()
{
	for(int c=0;c<MAX_TRACKS;c++)
		track[c].NoteOff();
}

void mi::ParameterTweak(int par, int val)
{
	Vals[par]=val;
	if(par < 16) {
		globals.Volume[par]=val;
	}
	else if(par < 32) {
		globals.Waveform[par&0xF]=val;
	}
	else if(par < 48) {
		globals.Transpose[par&0xF]=val;
	}
	else if(par < 64) {
		globals.Option[par&0xF]=val;
	}
	else if(par < 80) {
		globals.Command[par&0xF]=val;
	}
	else if(par < 96) {
		globals.Parameter[par&0xF]=val;
	}
	else if(par < 112) {
		globals.Speed[par&0xF]=val;
	}
	else {
		globals.StartPos=Vals[112]-1;
		globals.LoopStart=Vals[113]-1;
		globals.LoopEnd=Vals[114]-1;
		globals.ReplaySpeed=Vals[115];
		globals.AEGAttack=Vals[116];
		globals.AEGDecay=Vals[117];
		globals.AEGSustain=Vals[118];
		globals.AEGRelease=Vals[119];
		globals.Cutoff=Vals[120];
		globals.Resonance=Vals[121];
		globals.EnvMod=Vals[122];
		globals.FEGAttack=Vals[123];
		globals.FEGDecay=Vals[124];
		globals.FEGSustain=Vals[125];
		globals.FEGRelease=Vals[126];
		globals.Finetune=Vals[127];
	}
}

void mi::Command()
{
	char buffer[2048];
	sprintf
		(
			buffer,"%s%s%s",
			"Pattern commands\n",
			"\n03xx : Glide",
			"\n0Cxx : Volume"
		);
	pCB->MessBox(buffer,"GameFX",0);
}

void mi::Work(float *psamplesleft, float *psamplesright , int numsamples, int tracks)
{
	globals.noiseused=false;
	for(int c=0;c<tracks;c++)
	{
		if(track[c].AmpEnvStage)
		{
			float *xpsamplesleft=psamplesleft;
			float *xpsamplesright=psamplesright;
			int xnumsamples = numsamples;

			--xpsamplesleft;
			--xpsamplesright;

			track[c].PerformFx();
			do
			{
				const float sl=track[c].GetSample();

				*++xpsamplesleft+=sl;
				*++xpsamplesright+=sl;

			} while(--xnumsamples);
		}
	}
	if (globals.noiseused)
	{
		signed short noisework=0;
		for (int i = 0; i < 256; i++){
			globals.noise=(((globals.reg & (1<<22))? 1:0) << 7) |
			(((globals.reg & (1<<20))? 1:0) << 6) |
			(((globals.reg & (1<<16))? 1:0) << 5) |
			(((globals.reg & (1<<13))? 1:0) << 4) |
			(((globals.reg & (1<<11))? 1:0) << 3) |
			(((globals.reg & (1<<7))? 1:0) << 2)  |
			(((globals.reg & (1<<4))? 1:0) << 1)  |
			(((globals.reg & (1<<2))? 1:0) << 0);
			/* Save bits used to feed bit 0 */
			globals.bit22= (globals.reg & (1<<22))? 1:0;
			globals.bit17= (globals.reg & (1<<17))? 1:0;
			/* Shift 1 bit left */
			globals.reg= globals.reg << 1;
			/* Feed bit 0 */
			globals.reg= globals.reg | (globals.bit22 ^ globals.bit17);
			noisework=(signed short)globals.noise<<8;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.Wavetable[7][globals.noiseindex++]=noisework;
			globals.noiseindex=globals.noiseindex&2047;
		}
		globals.shortnoise=noisework;
	}
}

bool mi::DescribeValue(char* txt,int const param, int const value)
{
	if(param<112 && (param&0xF) > globals.LoopEnd) {
		sprintf(txt,"--");return true;
	}
	if(param<16)
	{
		if (value==0)sprintf(txt,"(keep)");
		else if(value==1) sprintf(txt, "-inf dB");
		else sprintf(txt, "%.02f dB", 20.0f * std::log10((float) (value-1) / 256.0f));
		return true;
	}

	// Oscillators waveform descriptions
	else if(param<32)
	{
		switch(value)
		{
			case 0:sprintf(txt,"(keep)");return true;break;
			case 1:sprintf(txt,"Sine");return true;break;
			case 2:sprintf(txt,"Triangle");return true;break;
			case 3:sprintf(txt,"Sine'n'back");return true;break;
			case 4:sprintf(txt,"Triangle'n'back");return true;break;
			case 5:sprintf(txt,"Square");return true;break;
			case 6:sprintf(txt,"Pulse");return true;break;
			case 7:sprintf(txt,"Sawtooth");return true;break;
			case 8:sprintf(txt,"Noise");return true;break;
			case 9:sprintf(txt,"Low Noise");return true;break;
		}
	}

	else if(param<48)
	{
		if (value==0) { sprintf(txt,"(keep)");return true; }
		//If Fix or Fix&retrig enabled
		else if(Vals[0x30+(param&0xF)] & 1) {
			int octave=(value-1)/12;
			switch((value-1)%12)
			{
				case 0:sprintf(txt,"C-%d",octave);return true;
				case 1:sprintf(txt,"C#%d",octave);return true;
				case 2:sprintf(txt,"D-%d",octave);return true;
				case 3:sprintf(txt,"D#%d",octave);return true;
				case 4:sprintf(txt,"E-%d",octave);return true;
				case 5:sprintf(txt,"F-%d",octave);return true;
				case 6:sprintf(txt,"F#%d",octave);return true;
				case 7:sprintf(txt,"G-%d",octave);return true;
				case 8:sprintf(txt,"G#%d",octave);return true;
				case 9:sprintf(txt,"A-%d",octave);return true;
				case 10:sprintf(txt,"A#%d",octave);return true;
				case 11:sprintf(txt,"B-%d",octave);return true;
			}
		}
		else {
			sprintf(txt,"%d semis", value-1);
			return true;
		}
	}

	else if(param<64)
	{
		switch(value)
		{
			case 0:sprintf(txt,"(keep)");return true;
			case 1:sprintf(txt,"Fix");return true;
			case 2:sprintf(txt,"Retrig");return true;
			case 3:sprintf(txt,"Fix'n'Retrig");return true;
		}
	}

	else if(param<80)
	{
		switch(value)
		{
			case 0:sprintf(txt,"(keep)");return true;
			case 1:sprintf(txt,"Inc Pitch");return true;
			case 2:sprintf(txt,"Dec Pitch");return true;
			case 3:sprintf(txt,"Inc Note");return true;
			case 4:sprintf(txt,"Dec Note");return true;
			case 5:sprintf(txt,"Set Pulse Width");return true;
			case 6:sprintf(txt,"Inc Pulse Width");return true;
			case 7:sprintf(txt,"Dec Pulse Width");return true;
			case 8:sprintf(txt,"Set Osc Phase");return true;
			case 9:sprintf(txt,"Filter: Off");return true;
			case 10:sprintf(txt,"Filter: Lowpass (Default)");return true;
			case 11:sprintf(txt,"Filter: Highpass");return true;
			case 15:sprintf(txt,"Gate Off (Release)");return true;
			default: sprintf(txt,"Reserved");return true;
		}
	}
	else if(param<96)
	{
		switch(Vals[0x40+(param&0xF)])
		{
		case 1://fallthrough
		case 2: std::sprintf(txt,"%.01f cents/repl.",value*0.1f); return true;
		case 3://fallthrough
		case 4: std::sprintf(txt,"%d semis",value); return true;
		case 5://fallthrough
		case 6://fallthrough
		case 7: std::sprintf(txt,"%.01f %%",value*0.390625f); return true;
		case 8: std::sprintf(txt,"%.0f",value*360.f/2.56f); return true;
		case 10://fallthrough
		case 11://fallthrough
		case 12://fallthrough
		case 13://fallthrough
		case 14://fallthrough
		case 15://fallthrough
		case 16:
			std::sprintf(txt,"%.0f Hz",(200.0+(17800.0*6.0*(value/256.f)))*44100.0*6.283185307179586476925286766559/985248.0);
			return true;
		}
	}
	else if(param<112)
	{
		switch(value)
		{
			case 0:sprintf(txt,"(keep)");return true;
			default:
				std::sprintf(txt,"%dx (%.0f Hz)",value,44100.f/(value*Vals[115]));
				return true;
		}
	}
	else
	{
		switch(param)
		{
		case 112: 
			{
				if(value <= Vals[114])
					std::sprintf(txt,"%d",value);
				else
					std::sprintf(txt,"%d",Vals[114]);
				return true;
			}
		case 113:
			{
				if(value <= Vals[114])
					std::sprintf(txt,"%d",value);
				else
					std::sprintf(txt,"%d",Vals[114]);
				return true;
			}
		case 115: std::sprintf(txt,"%.0f Hz",44100.f/value);return true;
		case 116://fallthrough
		case 117://fallthrough
		case 119://fallthrough
		case 123://fallthrough
		case 124://fallthrough
		case 126://fallthrough
			std::sprintf(txt,"%.02f ms",value/44.1f);
			return true;
		case 118:
			if (value > 0) sprintf(txt, "%.02f dB", 20.0f * std::log10((float) value / 256.0f));
			else sprintf(txt, "-inf dB");
			return true;
		case 120:
			std::sprintf(txt,"%.0f Hz",264*pow(32.,value/240.));
			return true;
		case 121:
			std::sprintf(txt,"%.03f Q",(float)sqrt(1.01+14*value/240.0));
			return true;
		case 122:
			{
			int top = Vals[120]+value;
			if(top > 256) top = 256;
			if(top < 0) top = 0;
			float result = 264*pow(32.,top/240.);
			std::sprintf(txt,"%.0f Hz", result);
			}
			return true;
		case 125:
			{
			int top = Vals[120]+Vals[122];
			if(top > 256) top = 256;
			if(top < 0) top = 0;
			float tmpval = Vals[120]+((top-Vals[120])*value/256.f);
			std::sprintf(txt,"%.0f Hz",264*pow(32.,tmpval/240.));
			}
			return true;
		case 127: std::sprintf(txt,"%.01f cents", value/2.56f); return true;
		}
	}
	return false;
}

void mi::SeqTick(int channel, int note, int ins, int cmd, int val)
{
	if (channel < MAX_TRACKS){

		track[channel].InitEffect(cmd,val);
		
		// Global scope synth pattern commands
		switch(cmd)
		{
		case 7: // Change envmod
			globals.EnvMod=val;
			break;
		case 8: // Change cutoff
			globals.Cutoff=val>>1;
		break;
		
		case 9: // Change reso
			globals.Resonance=val>>1;
		break;
		}
	
		if(note<=NOTE_MAX)
		{
			if ( cmd == 0x0C ) track[channel].NoteOn(note-24,&globals,val);
			else track[channel].NoteOn(note-24,&globals,60);
		}

		else if(note==NOTE_NOTEOFF)
			track[channel].NoteOff();
	}
}

void mi::InitWaveTable()
{
	int c=0;
	for(c=0;c<2048;c++)
	{
		double sval=c*0.00306796157577128245943617517898389;

		//Sine
		globals.Wavetable[0][c]=int(sin(sval)*16384.0f);

		//Triangle
		if(c<1024)
		globals.Wavetable[1][c]=(c*32)-16384;
		else
		globals.Wavetable[1][c]=16384-((c-1024)*32);

		//Square
		if(c<1024)
		globals.Wavetable[4][c]=-16384;
		else
		globals.Wavetable[4][c]=16384;

		//Saw
		globals.Wavetable[6][c]=(c*16)-16384;
	}
	for (c=0;c<4096;c++)
	{
		//Sine'n'back
		if(c<1024)
		{
			globals.Wavetable[2][c]=globals.Wavetable[0][c+c];
			globals.Wavetable[2][2047-c]=globals.Wavetable[2][c];
		}

		//Tri'n'back
		if(c<1024)
		{
			globals.Wavetable[3][c]=globals.Wavetable[1][(c+c+512)&2047];
			globals.Wavetable[3][2047-c]=globals.Wavetable[3][c];
		}

		//Pulse
		if(c<2048)
			globals.Wavetable[5][c]=-16384;
		else
			globals.Wavetable[5][c]=16384;
	}
}}