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
#ifndef __MManager__
#define __MManager__

#include <vector>

typedef unsigned char MIDI_BYTE;
typedef long MIDI_TIME_STAMP;
typedef std::vector<std::wstring> MIDIDeviceNameArray;

/// Enumeration of Midi Status byte types
enum MIDI_STATUS_BYTE_TYPE
{
    MIDI_NOTE_OFF,
    MIDI_NOTE_ON,
    MIDI_POLY_AFTERTOUCH,
    MIDI_CONTROL_CHANGE,
    MIDI_PROGRAM_CHANGE,
    MIDI_CHANNEL_AFTERTOUCH,
    MIDI_PITCH_BEND,
    MIDI_CHANNEL_MODE,
    MIDI_SYSEX_BEGIN,
    MIDI_MTC,
    MIDI_SPP,
    MIDI_SONG_SELECT,
    MIDI_TUNE_REQUEST,
    MIDI_SYSEX_END,
    MIDI_UNKNOWN

};

/// Abstract class for midi input device
/// Inherited to provide platform specific versions
class MInputDevice
{
	int ccArray[16][128]; // array to hold controller data for the active device; // used for polling
	int pitchWheel; // pitch data
public:
	MInputDevice();
	virtual ~MInputDevice();
	void OnMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp);
	int GetControllerValue(int channel, int controller); ///< Get the value of a Midi CC
	/// overload for midi systems that need polling
	virtual void tick(){};
};

/// Abstract class for midi ouput device
/// Inherited to provide platform specific versions
class MOutputDevice
{
public:
	MOutputDevice();
	virtual ~MOutputDevice();
	virtual void SendMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB) = 0;
};

/// Midi listening abstract class
class MListener
{
public:
	/// constructor registers this instance with the manager singleton
	MListener();
	/// destructor de-registers this instance with the manager singleton
	virtual ~MListener();
	/// overide this for reception of midi messages
	virtual void OnMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp) = 0;
};

/// Returns a message type from the enumerated type MIDI_STATUS_BYTE_TYPE
MIDI_STATUS_BYTE_TYPE MIDIStatusToType(MIDI_BYTE status);

/// Returns the associated channel 0 to 15 or -1 if no channel
int MIDIStatusToChannel(MIDI_BYTE status);

/// Make a status byte with specified type and channel. Channel is ignored for some types.
MIDI_BYTE MakeStatusByte(int type, int channel);

typedef std::vector<MListener*> MListenerList;
typedef std::vector<MInputDevice*> MInputDeviceList;
typedef std::vector<MOutputDevice*> MOutputDeviceList;

/// Singleton MIDI management class
/// Platform specific versions in herit and are automatically return in place of this generic version
class MManager
{
private:
	static MManager* singleton;

protected:

	MIDIDeviceNameArray inputDeviceNames;
	MIDIDeviceNameArray outputDeviceNames;

	MListenerList listeners;
	MInputDeviceList inputDevices;
	MOutputDeviceList outputDevices;

	/// Protected constructor (singleton)
	MManager();
	/// Protected desstructor (singleton)
	virtual ~MManager();

public:

	/// singleton creation and accessor
	/// \return The one instance of the midi manager singleton
	static MManager& get_instance();

	/// Destroy the one instance of the singleton
	/// Should be called during system cleanup
	static void destroy_instance();

	/// Add a listener object.
	/// called from singleton by MListener constructor
	/// \param[in] listener : A listener object to be registered to reception of midi events
	void RegisterMListener(MListener *listener);

	/// Add a listener object.
	/// called from singleton by MListener destructor
	/// \param[in] listener : A listener object to be removed from registered list
	void UnregisterMListener(MListener *listener);

	MIDIDeviceNameArray& GetInputDeviceNames(); ///< Return a vector containing all avaliable input device names
	MIDIDeviceNameArray& GetOutputDeviceNames(); ///< Return a vector containing all avaliable output device names

	int GetNumInputDevices(); ///< Get the number of open input devices \return num ins
	int GetNumOutputDevices();///< Get the number of open output devices \return num outs

	void OnMidiMessage(int deviceId, MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp); // message reception loop
	void SendMidiMessage(int deviceId, MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB); // Send midi message on active device

	virtual int OpenInputDevice(int id) = 0; ///< Open a midi device for input
	virtual int OpenOutputDevice(int id) = 0; ///< Open a midi device for output

	void Start(); ///< Start midi processing
	void Stop(); ///< Stop midi processing
	void CloseDevices(); ///< Close all active devices

	int GetControllerValue(int deviceId, int channel, int controller); ///< Get the value of a Midi CC
	float GetMappedControllerValue(int deviceId, int channel, int controller, float min, float max); ///< Get a CC Value mapped onto a new range

	/// overload for midi systems that need polling
	virtual void tick(){};
};

#endif
