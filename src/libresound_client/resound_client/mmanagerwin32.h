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
// Class automatically generated by Dev-C++ New Class wizard
#ifdef __WIN32__

#ifndef SA_MMANAGERWIN32_H
#define SA_MMANAGERWIN32_H

#include "sa_mmanager.h" // inheriting class's header file

#include <windows.h>
#include <mmsystem.h>

class MInputDeviceWin32 : public MInputDevice
{
	// windows specific midi vars
	HMIDIIN hMidiIn;
	MIDIINCAPS midiInCaps;

public:
	MInputDeviceWin32(int deviceId);
	virtual ~MInputDeviceWin32();

	static void CALLBACK MidiInProc(HMIDIIN _hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
};

class MOutputDeviceWin32 : public MOutputDevice
{
	HMIDIOUT hMidiOut;
	MIDIOUTCAPS midiOutCaps;

public:
	MOutputDeviceWin32(int deviceId);
	virtual ~MOutputDeviceWin32();
	virtual void SendMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB);

};

/// Midi management class specific to the windows platform
class MManagerWin32 : public MManager
{
private:

public:
	// class constructor
	MManagerWin32();
	// class destructor
	virtual ~MManagerWin32();

	void GetDriverNames();

	virtual int OpenInputDevice(int id); ///< Open a midi device for input
	virtual int OpenOutputDevice(int id); ///< Open a midi device for output
};

#endif // SA_MMANAGERWIN32_H
#endif // __WIN32__