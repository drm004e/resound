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

#ifndef __AUDIO_BUFFER__
#define __AUDIO_BUFFER__
namespace Resound{

/// our typedef for internal sample format, current as jack (float 32bit )
typedef jack_default_audio_sample_t AudioSample;

/// an audio data buffer container
class AudioBuffer{
private:
	AudioSample* m_data;
public:
	AudioBuffer(){m_data = 0;};
	AudioBuffer(long size){m_data = new AudioSample[size];}
	~AudioBuffer(){if(m_data) delete m_data;};
	AudioSample* get_data_ptr(){return m_data;};
};

/// AudioBufferArray is a vector of audio buffers
typedef std::vector<AudioBuffer*> AudioBufferArray;

}
#endif
