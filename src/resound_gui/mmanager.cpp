/*
 *   
 *   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
 *   
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *   
*/
#include "pch.h"

#ifdef __WIN32__
    #include "mmanagerwin32.h" // windows platform include
#endif

#include <resound_common/exception.h>

#include "mmanager.h"

// ------------------------------------- MListener ---------------------------
MListener::MListener()
{
	// Construct
	//MManager::GetSingleton().RegisterMListener(this); // linux midi incomplete FIXME
}

MListener::~MListener()
{
	// Destruct
	//MManager::GetSingleton().UnregisterMListener(this); // linux midi incomplete FIXME
}

// ------------------------------------ Functions -----------------------------
MIDI_STATUS_BYTE_TYPE MIDIStatusToType(MIDI_BYTE status)
{
	if(status >= 0xF0) {
		switch(status) {
		case 0xF0:
			return MIDI_SYSEX_BEGIN;
		case 0xF1:
			return MIDI_MTC;
		case 0xF2:
			return MIDI_SPP;
		case 0xF3:
			return MIDI_SONG_SELECT;
		case 0xF6:
			return MIDI_TUNE_REQUEST;
		case 0xF7:
			return MIDI_SYSEX_END;
		default:
			return MIDI_UNKNOWN; // there are some reservered and or undefined status messages
		}
	} else {
		status &= 0xF0; // splits of the bottom nibble
		switch(status) {
		case 0x80:
			return MIDI_NOTE_OFF;
		case 0x90:
			return MIDI_NOTE_ON;
		case 0xA0:
			return MIDI_POLY_AFTERTOUCH;
		case 0xB0:
			return MIDI_CONTROL_CHANGE;
		case 0xC0:
			return MIDI_PROGRAM_CHANGE;
		case 0xD0:
			return MIDI_CHANNEL_AFTERTOUCH;
		case 0xE0:
			return MIDI_PITCH_BEND;
		default:
			return MIDI_UNKNOWN; // should never do this but it doesn't hurt
		}
	}
	return MIDI_UNKNOWN; // likewise should never get here but...
}

int MIDIStatusToChannel(MIDI_BYTE status)
{
	if(status >= 0xF0)
		return -1; // this message shouldn't have a channel nibble
	return status & 0x0F;  // and of the status and just leave the channel
}

MIDI_BYTE MakeStatusByte(int type, int channel)
{
	MIDI_BYTE byte;
	switch(type) {
	case MIDI_NOTE_OFF:
		byte = 0x80;
		break;
	case MIDI_NOTE_ON:
		byte = 0x90;
		break;
	case MIDI_POLY_AFTERTOUCH:
		byte = 0xA0;
		break;
	case MIDI_CONTROL_CHANGE:
		byte = 0xB0;
		break;
	case MIDI_PROGRAM_CHANGE:
		byte = 0xC0;
		break;
	case MIDI_CHANNEL_AFTERTOUCH:
		byte = 0xD0;
		break;
	case MIDI_PITCH_BEND:
		byte = 0xE0;
		break;
	}
	MIDI_BYTE chan = (MIDI_BYTE)channel & 0x0F;
	return byte | chan;
}

// ----------------------- Imput device ----------------------------------
MInputDevice::MInputDevice()
{
	// Construct
	for(int ch = 0; ch < 16; ch++) {
		for(int cc = 0; cc < 128; cc++) {
			ccArray[ch][cc] = 0;
		}
	}
}
MInputDevice::~MInputDevice()
{}

void MInputDevice::OnMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp)
{
	// decode the message into the local storage buffers
	int type = MIDIStatusToType(status);
	int channel = MIDIStatusToChannel(status);
	switch(type) {
	case MIDI_CONTROL_CHANGE:

		ccArray[channel][(int)dataA] = (int) dataB;
		break;
	}

	// now call singleton
	MManager::GetSingleton().OnMidiMessage(0,status,dataA,dataB,timeStamp);
}

int MInputDevice::GetControllerValue(int channel, int controller)
{
	return ccArray[channel][controller];
}

// ----------------------- Output device ---------------------------------
MOutputDevice::MOutputDevice()
{}

MOutputDevice::~MOutputDevice()
{}

// ---------------------- midi manager -----------------------------------
MManager* MManager::singleton = 0;
MManager& MManager::GetSingleton()
{
	if(!singleton) {
#ifdef __WIN32__
		singleton = new MManagerWin32();
#endif

	}
	return *singleton;
}

void MManager::Destroy()
{
	if(singleton)
		delete singleton;
}

MManager::MManager()
{
}

MManager::~MManager()
{
	// Destruct
	CloseDevices();
}
MIDIDeviceNameArray& MManager::GetInputDeviceNames()
{
	return inputDeviceNames;
}
MIDIDeviceNameArray& MManager::GetOutputDeviceNames()
{
	return outputDeviceNames;
}

int MManager::GetNumInputDevices()
{
	return inputDevices.size();
}

int MManager::GetNumOutputDevices()
{
	return inputDevices.size();
}

int MManager::GetControllerValue(int deviceId, int channel,int controller)
{
	return inputDevices[deviceId]->GetControllerValue(channel,controller);
}

float MManager::GetMappedControllerValue(int deviceId, int channel,int controller,float min,float max)
{
	float scale = (max - min);
	return ((float)GetControllerValue(deviceId,channel,controller) / 128.0f) * scale + min;
}

void MManager::OnMidiMessage(int deviceId, MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp)
{
	for(int n = 0; n < listeners.size(); n++) {
		listeners[n]->OnMidiMessage(status,dataA,dataB,timeStamp);
	}
}

void MManager::SendMidiMessage(int deviceId, MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB)
{
	outputDevices[deviceId]->SendMidiMessage(status, dataA, dataB);
}


void MManager::RegisterMListener(MListener *listener)
{
	listeners.push_back(listener);
}

void MManager::UnregisterMListener(MListener *listener)
{
	for(int n = 0; n < listeners.size(); n++) {
		if(listeners[n] == listener) {
			listeners.erase(listeners.begin() + n);
		}
	}
}

void MManager::CloseDevices()
{
	for(int n = 0; n < inputDevices.size(); n++) {
		delete inputDevices[n];
	}
	inputDevices.clear();

	for(int n = 0; n < outputDevices.size(); n++) {
		delete outputDevices[n];
	}
	inputDevices.clear();
}
