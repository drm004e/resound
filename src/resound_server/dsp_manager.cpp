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

	for(int n = 0; n < m_numInputs; n++){
		std::stringstream s;
		s << "Input" << n;
		jack_port_register(m_jc,s.str().c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput,0);
	}
	for(int n = 0; n < m_numOutputs; n++){
		std::stringstream s;
		s << "Output" << n;
		jack_port_register(m_jc,s.str().c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput,0);
	}
}

Resound::DSPManager::~DSPManager()
{
	if(m_jc) jack_client_close(m_jc);
	m_jc=0;
}

// ------- DSP MAIN ENTRY HERE ---------------
bool Resound::DSPManager::DSP(AudioBufferArray& inputs, AudioBufferArray& outputs, long bufferSize)
{
	int maxIns = inputs.size() + 1; // plus one to allow for inputs outputs and global
	int maxOuts = outputs.size() + 1;
	int iOut, iIn;
	float globalAtt, outAtt, inAtt, matAtt, finalAtt;

	AudioBuffer* outputBuffer; // cached output buffer pointer
	AudioBuffer* inputBuffer; // cached input buffer pointer

	globalAtt = DSPLogInterpolate(m_iAttMatrix.Index(0,0),m_nAttMatrix.Index(0,0));
	if(globalAtt != 0.0f) {
		for(iOut = 1; iOut < maxOuts; iOut++) {
			outputBuffer = outputs[iOut-1]; // cache the buffer to eliminate indexing
			// Clear output buffers ready for summing
			MemsetBuffer(outputBuffer,0.0f,bufferSize);
			// interpolate dsp factors
			outAtt = DSPLogInterpolate(m_iAttMatrix.Index(0,iOut),m_nAttMatrix.Index(0,iOut));
			if(outAtt != 0.0f) {

				for(iIn = 1; iIn < maxIns; iIn++) {
					inAtt = DSPLogInterpolate(m_iAttMatrix.Index(iIn,0),m_nAttMatrix.Index(iIn,0));
					if(inAtt != 0.0f) {
						inputBuffer = inputs[iIn-1]; // cache the buffer to eliminate indexing
						matAtt = DSPLogInterpolate(m_iAttMatrix.Index(iIn,iOut),m_nAttMatrix.Index(iIn,iOut));
						if(matAtt != 0.0f) {
							finalAtt = globalAtt * outAtt * inAtt * matAtt; // factor gains together
							DSPSumToBuss(inputBuffer, outputBuffer, finalAtt ,bufferSize); // sum onto the buss
						} // nodes
					}
				}// inputs
			}
		} // outputs
	}
}

