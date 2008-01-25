//   
//   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
//   
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   aint with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//   MA 02111-1307 USA
//   
#ifndef __SA_DSP__
#define __SA_DSP__

#include "audio_buffer.h"


namespace Resound{
/// interpolation logarithmic and simple
inline float dsp_log_interpolate(float &dest, const float &src, float factor = 0.95f)
{
	return dest = (factor * dest) + ((1.0f - factor) * src);
}

/// Take an input buffer and sum it to and output buffer using the gain value specified
/// asumes that interpSize <= bufferSize
inline void dsp_sum_to_buss(AudioBuffer* bufferIn, AudioBuffer* bufferOut, float startGain, float endGain, int interpSize, int bufferSize)
{
	float* in = bufferIn->get_data_ptr();
	float* out = bufferOut->get_data_ptr();

	if(startGain == 1.0f && endGain == 1.0f) // optimised with gain factor of 1
	{
		while(--bufferSize >= 0)
		{
			*out += *in;
			++out;
			++in;
		}
	}
	else if(startGain == -1.0f && endGain == -1.0f) // optimised with gain factor of -1
	{
		while(--bufferSize >= 0)
		{
			*out -= *in;
			++out;
			++in;
		}
	}
	else if(startGain == 0.0f && endGain == 0.0f) // optimised for 0 values
	{
	} 
	else if(startGain == endGain ) // no interpolation needed
	{	
		while(--bufferSize >= 0) {
			*out += *in * startGain;
			++out;
			++in;
		}
	} 
	else { // linear interpolate and sum
		float f = (endGain - startGain)/interpSize;
		for(int n = 0; n < interpSize; n++, --bufferSize){
			*out += *in * (startGain + f * (float)n);
			++out;
			++in;
		}// remainder of buffer
		while(--bufferSize >= 0) {
			*out += *in * endGain;
			++out;
			++in;
		}
	}
};

/// clear a buffer of its contents
inline void MemsetBuffer(AudioBuffer* bufferIn, float value, int bufferSize)
{
	float* buffer = bufferIn->get_data_ptr();
	while(--bufferSize >= 0) {
		*buffer = value;
		++buffer;
	}
}

inline void compute_vu_meters(AudioBuffer* bufferIn, int bufferSize, float& rms, float& peak){
	rms = peak = 0.0f;
	float* buffer = bufferIn->get_data_ptr();
	int b = bufferSize;
	while(--b >= 0) {
		float t = fabs(*buffer);
		rms += t;
		peak = (t > peak) ? t : peak;
		++buffer;
	}
	rms = rms / (float)bufferSize;
}

} // namespace resound
#endif
