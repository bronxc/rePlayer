/*      Copyright (C) 2002 Vincenzo Demasi.

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

		Vincenzo Demasi. E-Mail: <v.demasi@tiscali.it>
*/

#pragma once
namespace psycle::plugins::vincenzo_demasi::vdAllPass {
class AllPassFilter
{
private:
	float* buffer;
	int size;
	unsigned int writein, readout;
	float gain;
public:

	AllPassFilter():buffer(0), size(0), writein(0), readout(0), gain(1.0f)
	{
	}

	~AllPassFilter()
	{
		if(buffer) delete[] buffer;
	}

	//Resizing the buffer to half of new samplerate (half a second)
	inline void changeSamplerate(int newSR) {
		int const newSize=newSR/2 + 1;
		if (buffer) delete[] buffer;
		buffer = new float[newSize];
		for(int i=0; i<newSize; i++ ) {
			buffer[i]=0.0f;
		}
		size=newSize;
		if(readout>= size) {
			readout = 0;
			writein = 0;
		}
	}

	inline void setDelay(unsigned int samples)
	{
		if((writein = readout + samples) >= size)
			writein -= size;
	}

	inline void setGain(float value)
	{
		gain = value;
	}

	inline float process(float sample)
	{
		/* WARNING: not properly work when delay=0 because readout==writein
			(ignored for efficiency) */
		float output = -gain * sample + buffer[readout];
		buffer[writein] = sample + output * gain;
		if(++writein == size) writein = 0;
		if(++readout == size) readout = 0;
		return output;
	}
};
}