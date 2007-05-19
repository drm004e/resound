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

Resound::DSPManager::DSPManager(const std::string& name, int inputs, int outputs) :
m_name(name),
m_numInputs(inputs),
m_numOutputs(outputs),
m_audioMatrix(new AudioMatrix(m_numInputs, m_numOutputs))
{
	m_nAttMatrix.Create(m_numInputs+1, m_numOutputs+1);
	m_iAttMatrix.Create(m_numInputs+1, m_numOutputs+1);

	// jack initialisation
	m_jc = jack_client_open(m_name.c_str(),JackNullOption,0);

	// register ports
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
	jack_set_process_callback(m_jc,Resound::DSPManager::jack_process_callback,this);

	// get some info from jackd about current SR and bufferSize;
	m_bufferSize =  jack_get_buffer_size(m_jc);
	m_sampleRate =  jack_get_sample_rate(m_jc);

	// set some default attenuations


	// G Node
		m_nAttMatrix.Index(0,0) = 0.5f;
		m_iAttMatrix.Index(0,0) = 0.5f;
	// I nodes
	for(int n=1; n<m_numInputs+1;n++){
		m_nAttMatrix.Index(n,0) = 1.0f;
		m_iAttMatrix.Index(n,0) = 1.0f;
	}
	// this is the I->O nodes
	int minIO = m_numInputs;
	if(minIO > m_numOutputs) minIO = m_numOutputs;
	for(int n=1; n < minIO+1; n++){
		m_nAttMatrix.Index(n,n) = 1.0f;
		m_iAttMatrix.Index(n,n) = 1.0f;
	}
	// O nodes
	for(int n=1; n<m_numOutputs+1;n++){
		m_nAttMatrix.Index(0,n) = 1.0f;
		m_iAttMatrix.Index(0,n) = 1.0f;
	}

	// now activate the callback
	jack_activate(m_jc);

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
	float globalAtt, outAtt, inAtt, matAtt, finalAtt;

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


	globalAtt = DSPLogInterpolate(m_iAttMatrix.Index(0,0),m_nAttMatrix.Index(0,0));
	if(globalAtt != 0.0f) {
		for(iOut = 1; iOut < maxOuts; iOut++) {
			outputBuffer = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_outputs[iOut-1],nframes),nframes); // we get this one encapsulating the jack port
			// Clear output buffers ready for summing
			MemsetBuffer(outputBuffer,0.0f,nframes);
			// interpolate dsp factors
			outAtt = DSPLogInterpolate(m_iAttMatrix.Index(0,iOut),m_nAttMatrix.Index(0,iOut));
			if(outAtt != 0.0f) {

				for(iIn = 1; iIn < maxIns; iIn++) {
					inAtt = DSPLogInterpolate(m_iAttMatrix.Index(iIn,0),m_nAttMatrix.Index(iIn,0));
					if(inAtt != 0.0f) {
						inputBuffer = new AudioBuffer((AudioSample*)jack_port_get_buffer(m_inputs[iIn-1],nframes),nframes); // we get this one encapsulating the jack port
						matAtt = DSPLogInterpolate(m_iAttMatrix.Index(iIn,iOut),m_nAttMatrix.Index(iIn,iOut));
						if(matAtt != 0.0f) {
							finalAtt = globalAtt * outAtt * inAtt * matAtt; // factor gains together
							DSPSumToBuss(inputBuffer, outputBuffer, finalAtt ,nframes); // sum onto the buss
						} // nodes
					}
					delete inputBuffer;
				}// inputs
			}
			delete outputBuffer;
		} // outputs
	}//*/
	return 0;
}

