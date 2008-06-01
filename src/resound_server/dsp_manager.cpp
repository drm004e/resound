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

#include "pch.h"
#include "audio_buffer.h"
#include "dsp.h"
#include "dsp_manager.h"

#include <sstream>
#include <iostream>

#include <resound_common/verbose.h>

Resound::DSPManager::DSPManager(const std::string& name, int inputs, int outputs, const char* port, bool autoConnect) :
m_name(name),
m_numInputs(inputs),
m_numOutputs(outputs),
m_audioMatrix(new AudioMatrix(inputs, outputs)),

OSCManager(port)
{
	VERBOSE(std::cout << "Initialising I/O matrix... \n";)
	m_nAttMatrix.Create(m_numInputs+1, m_numOutputs+1);
	m_prevGainMatrix.Create(m_numInputs+1, m_numOutputs+1);

	// jack initialisation
	VERBOSE(std::cout << "Connecting to jackd... \n";)
	m_jc = jack_client_open(m_name.c_str(),JackNullOption,0);

	// register ports
	VERBOSE(std::cout << "Registering I/O ports... \n";)
	for(int n = 0; n < m_numInputs; n++){
		std::stringstream s;
		s << "Input" << n;
		m_inputs.push_back(jack_port_register(m_jc,s.str().c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput,0));
	}
	for(int n = 0; n < m_numOutputs; n++){
		std::stringstream s;
		s << "Output" << n;
		m_outputs.push_back(jack_port_register(m_jc,s.str().c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput,0));
	}

	// register callbacks
	VERBOSE(std::cout << "Registering callbacks... \n";)
	jack_set_process_callback(m_jc,Resound::DSPManager::jack_process_callback,this);
	for(int r = 0; r < m_numInputs+1; r++){
		for(int c = 0; c < m_numOutputs+1; c++){
			std::stringstream s;
			s << "/matrix/att/" << r << "/" << c;
			add_method(s.str(),"f",Resound::DSPManager::lo_cb_att, (void*)&m_nAttMatrix.Index(r,c));
		}
	}

	

	// get some info from jackd about current SR and bufferSize;
	m_bufferSize =  jack_get_buffer_size(m_jc);
	m_sampleRate =  jack_get_sample_rate(m_jc);

	// set some default attenuations, usefull for stress tes
	for(int r=0; r < m_numInputs;r++){
		for(int c=0; c < m_numOutputs;c++){
			m_nAttMatrix.Index(r,c) = 0.0f;
			m_prevGainMatrix.Index(r,c) = 0.0f;
		}
	}

	// now activate the callback
	VERBOSE(std::cout << "Activating DSP... \n";)
	jack_activate(m_jc);

	// now connect ports
	if(autoConnect){
		VERBOSE(std::cout << "Auto connecting ports... \n";)
		for(int n = 0; n < m_numInputs; n++){
			// auto connection
			std::stringstream name1, name2;
			name1 << "system:capture_" << n+1;		
			name2 << "Resound:Input" << n;		
			jack_connect(m_jc,name1.str().c_str(),name2.str().c_str());
		}
		for(int n = 0; n < m_numOutputs; n++){
			// auto connection
			std::stringstream name1, name2;	
			name1 << "Resound:Output" << n;
			name2 << "system:playback_" << n+1;	
			jack_connect(m_jc,name1.str().c_str(),name2.str().c_str());
		}
	}
	std::cout << "\n---- Resound Server Running ----\n";
	std::cout << "  Inputs  : " << inputs << "\n";
	std::cout << "  Outputs : " << outputs << "\n";
	std::cout << "  OSC Port: " << port << "\n";
	std::cout << "--------------------------------\n\n";
}

Resound::DSPManager::~DSPManager()
{
	if(m_jc) jack_client_close(m_jc);
	m_jc=0;
}

// jack callback
int Resound::DSPManager::jack_process_callback(jack_nframes_t nframes, void *arg){
	Resound::DSPManager* ptr = static_cast<Resound::DSPManager*>(arg);
	return ptr->process(nframes);
}

// ------- DSP MAIN ENTRY HERE ---------------
int Resound::DSPManager::process(jack_nframes_t nframes)
{
	int maxIns = m_inputs.size() + 1; // plus one to allow for inputs outputs and global
	int maxOuts = m_outputs.size() + 1;
	int iOut, iIn;
	float globalAtt, outAtt, inAtt, matAtt, finalGain;

	AudioBuffer* outputBuffer; // cached output buffer pointer
	AudioBuffer* inputBuffer; // cached input buffer pointer

/* some test code
	outputBuffer = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_outputs[0],nframes),nframes); // we get this one encapsulating the jack port
	inputBuffer = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_inputs[0],nframes),nframes); // we get this one encapsulating the jack port
	// Clear output buffers ready for summing
	MemsetBuffer(outputBuffer,0.0f,nframes);
	DSPSumToBuss(inputBuffer, outputBuffer, 1.0f ,nframes); // sum onto the buss
	delete inputBuffer;
	delete outputBuffer;
//*/


	globalAtt = m_nAttMatrix.Index(0,0); // get global gain factor
	//if(globalAtt != 0.0f) {
		for(iOut = 1; iOut < maxOuts; iOut++) {
			outputBuffer = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_outputs[iOut-1],nframes),nframes); // we get this one encapsulating the jack port
			// Clear output buffers ready for summing
			MemsetBuffer(outputBuffer,0.0f,nframes);
			outAtt = m_nAttMatrix.Index(0,iOut); // get output gain factor
			if(outAtt != 0.0f) {

				for(iIn = 1; iIn < maxIns; iIn++) {
					inAtt = m_nAttMatrix.Index(iIn,0); // get input gain factor
					if(inAtt != 0.0f) {
						inputBuffer = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_inputs[iIn-1],nframes),nframes); // we get this one encapsulating the jack port
						matAtt = m_nAttMatrix.Index(iIn,iOut); // get node gain factor
						if(matAtt != 0.0f) {
							float& prevGain = m_prevGainMatrix.Index(iIn,iOut);
							finalGain = globalAtt * outAtt * inAtt * matAtt; // factor gains together
							Resound::dsp_sum_to_buss(inputBuffer, outputBuffer, prevGain, finalGain, m_interpSize ,nframes); // sum onto the buss
							prevGain = finalGain; // remmember the gain factor
						} // nodes
						delete inputBuffer;
					}
				}// inputs
			}

			delete outputBuffer;

		} // outputs
	//}//*/

	// compute vu's
	float rms,peak;
	AudioBuffer* b;
	for(int n = 0; n < m_inputs.size(); n++){
		b = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_inputs[n],nframes),nframes); // we get this one encapsulating the jack port
		compute_vu_meters(b,nframes,rms,peak);
		delete b;
	}
	for(int n = 0; n < m_outputs.size(); n++){
		AudioBuffer* b = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_outputs[n],nframes),nframes); // we get this one encapsulating the jack port
		compute_vu_meters(b,nframes,rms,peak);
		delete b;
	}
	return 0;
}


int Resound::DSPManager::lo_cb_att(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	float* f = (float*)user_data; // the user data should be a pointer to the newattmatrix element
	*f = argv[0]->f; // the argument should be a float
	VERBOSE(std::cout << path << " " << *f << std::endl;)
    return 1;
}



