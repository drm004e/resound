/**********************************************************************************
    Resound
    Copyright 2007 David Moore and James Mooney 

    This file is part of Resound.

    Resound is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Resound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Resound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

************************************************************************************/

#ifndef DSP_MANAGER_H
#define DSP_MANAGER_H

#include <string>
#include "dsp.h"


enum ENUM_AM_SERVER_SOCKET_IDS{
    AM_SERVER_SOCKET_ID = 1,
    AM_SOCKET_ID
};

namespace Resound
{


/// audio matrix is a simple 2d array of audio buffers
typedef Array2<AudioBuffer*> AudioMatrix;

/// attenuation matrix is a simple 2d array of floating point attenuation factors
typedef Array2<float> AttenuationMatrix;

/// a class for handling signal processing in the resound_server
class DSPManager
{
public:
	/// construct
	DSPManager(const std::string& name, int inputs, int outputs);
	/// detruct 
	virtual ~DSPManager(); 

	/// main dsp operation
	bool DSP(AudioBufferArray& inputs, AudioBufferArray& outputs, long bufferSize);

private:

	AudioMatrix* m_audioMatrix; ///< the matrix of buffers
	AttenuationMatrix m_nAttMatrix; ///< non interpolated attenuations
	AttenuationMatrix m_iAttMatrix; ///< interpolated attenuations

	int m_numInputs; ///< number of inputs created
	int m_numOutputs; ///< number of outputs created

	std::string m_name; ///< the jack client name
	jack_client_t* m_jc; ///< the jack client ptr
};
}

#endif // DSP_MANAGER_H
