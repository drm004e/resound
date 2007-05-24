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

#include <resound_common/osc_manager.h>

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

/// a list of pointers to ports
typedef std::vector<jack_port_t*> JackPortList;

/// a class for handling signal processing in the resound_server
class DSPManager : public OSCManager
{
public:
	/// construct
	DSPManager(const std::string& name, int inputs, int outputs, const char* port);
	/// detruct 
	virtual ~DSPManager(); 

	/// main dsp operation
	int process(jack_nframes_t nframes);

private:

	AudioMatrix* m_audioMatrix; ///< the matrix of buffers
	AttenuationMatrix m_nAttMatrix; ///< non interpolated attenuations
	AttenuationMatrix m_iAttMatrix; ///< interpolated attenuations

	int m_numInputs; ///< number of inputs created
	int m_numOutputs; ///< number of outputs created

	JackPortList m_inputs; ///< the actual jack input ports
	JackPortList m_outputs; ///< the actual jack output ports

	std::string m_name; ///< the jack client name
	jack_client_t* m_jc; ///< the jack client ptr

	jack_nframes_t m_bufferSize; ///< the current bufferSize
	jack_nframes_t m_sampleRate; ///< the current sample rate

public: // callbacks
	/// static dsp process callback
	static int jack_process_callback(jack_nframes_t nframes, void *arg);

	/// callback from osc att methods
	static int lo_cb_att(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);

};
}

#endif // DSP_MANAGER_H
