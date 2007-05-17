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
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//   MA 02111-1307 USA
//   
#ifndef __SA_DSP__
#define __SA_DSP__

namespace Resound{
// interpolation logarithmic and simple
inline float DSPLogInterpolate(float &dest, const float &src, float factor = 0.95f)
{
	return dest = (factor * dest) + ((1.0f - factor) * src);
}

// Take an input buffer and sum it to and output buffer using the gain value specified
inline void DSPSumToBuss(AudioBuffer* bufferIn, AudioBuffer* bufferOut, float factor, long bufferSize)
{
	float* in = bufferIn->get_data_ptr();
	float* out = bufferOut->get_data_ptr();

	if(factor == 1.0f) // gain factor of 1
	{
		while(--bufferSize >= 0)
		{
			*out += *in;
			++out;
			++in;
		}
	}
	else if(factor == -1.0f) // gain factor of -1
	{
		while(--bufferSize >= 0)
		{
			*out -= *in;
			++out;
			++in;
		}
	}
	else if(factor == 0.0f) // dont sum
	{
	} else {
		while(--bufferSize >= 0) {
			*out += *in * factor;
			++out;
			++in;
		}
	}
};

// clear a buffer of its contents
inline void MemsetBuffer(AudioBuffer* bufferIn, float value, long bufferSize)
{
	float* buffer = bufferIn->get_data_ptr();
	while(--bufferSize >= 0) {
		*buffer = value;
		++buffer;
	}
}

} // namespace resound
#endif
