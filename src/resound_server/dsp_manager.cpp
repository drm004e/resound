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

Resound::DSPManager::DSPManager() :
m_numInputs(1),
m_numOutputs(1)
{
	m_audioMatrix = new AudioMatrix(m_numInputs, m_numOutputs);
	m_nAttMatrix.Create(m_numInputs+1, m_numOutputs+1);
	m_iAttMatrix.Create(m_numInputs+1, m_numOutputs+1);
}

Resound::DSPManager::~DSPManager()
{
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

