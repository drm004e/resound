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

#include "mmanager.h"
#include <alsa/asoundlib.h>

class MInputDeviceALSA : public MInputDevice
{
public:
	MInputDeviceALSA(int deviceId);
	virtual ~MInputDeviceALSA();
	/// overload for midi systems that need polling
	virtual void tick();
private:
	snd_seq_t* m_seq_handle;
	int m_npfd;
	pollfd* m_pfd; 
	snd_midi_event_t* m_decoder;
};

class MOutputDeviceALSA : public MOutputDevice
{
public:
	MOutputDeviceALSA(int deviceId);
	virtual ~MOutputDeviceALSA();
	virtual void SendMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB);
private:
	snd_seq_t* seq_handle;
};

/// Midi management class specific to the windows platform
class MManagerALSA : public MManager
{
private:

public:
	// class constructor
	MManagerALSA();
	// class destructor
	virtual ~MManagerALSA();

	void GetDriverNames();

	virtual int OpenInputDevice(int id); ///< Open a midi device for input
	virtual int OpenOutputDevice(int id); ///< Open a midi device for output
	virtual void tick(); ///< tick the midi system poll for any pending events etc.

	/// open an alsa midi sequencer
	static snd_seq_t* open_seq(const std::string name);
};