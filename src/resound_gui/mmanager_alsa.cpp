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

#ifdef __LINUX

#include "mmanager_alsa.h"
#include <iostream>
#include <sstream>
#include <resound_common/exception.h>
// ------------------------- MInputDeviceALSA
MInputDeviceALSA::MInputDeviceALSA(int deviceId) :
m_seq_handle(0),
m_decoder(0)
{	
	std::stringstream s;
	s << "Port_" <<  deviceId;
	std::cout << "Creating ALSA Midi input device " << s.str() << std::endl;
	m_seq_handle = MManagerALSA::open_seq(s.str());
	if(!m_seq_handle) throw Resound::Exception();
	m_npfd = snd_seq_poll_descriptors_count(m_seq_handle, POLLIN);
	m_pfd = (struct pollfd *)alloca(m_npfd * sizeof(struct pollfd));
	snd_seq_poll_descriptors(m_seq_handle, m_pfd, m_npfd, POLLIN);
	snd_midi_event_new(1024,&m_decoder);
	snd_midi_event_init(m_decoder);
}
MInputDeviceALSA::~MInputDeviceALSA(){
}
void MInputDeviceALSA::tick(){	

	
		snd_seq_event_t *ev;
		MIDI_BYTE buf[3];
		while(snd_seq_event_input(m_seq_handle, &ev) > 0){
			//snd_midi_event_decode (m_decoder, buf, 3, ev);
			//OnMidiMessage(buf[0], buf[1], buf[2], 0);
			switch (ev->type) {
      				case SND_SEQ_EVENT_CONTROLLER: 
        				std::cout << "MIDI Control" << int(ev->data.control.channel) << " " << ev->data.control.param << " " << ev->data.control.value << std::endl;
					OnMidiMessage(MakeStatusByte(MIDI_CONTROL_CHANGE, ev->data.control.channel), ev->data.control.param, ev->data.control.value, 0);
        			break;
			}
			snd_seq_free_event(ev);
			//std::cout << "MIDI " << int(buf[0]) << " " << int(buf[1]) << " " << int(buf[2]) << std::endl;
		}
		


}
// --------------------------MOutputDeviceALSA
MOutputDeviceALSA::MOutputDeviceALSA(int deviceId) :
seq_handle(0)
{
	std::stringstream s;
	s << "Port_" << deviceId;
	std::cout << "Creating ALSA Midi output device " << s.str() << "...\n";
	seq_handle = MManagerALSA::open_seq(s.str());
	if(!seq_handle) throw Resound::Exception();
}
MOutputDeviceALSA::~MOutputDeviceALSA(){
}
void MOutputDeviceALSA::SendMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB){
}


// ------------------------ MMAnagerALSA ----------------------
MManagerALSA::MManagerALSA(){

}
MManagerALSA::~MManagerALSA(){
}

void MManagerALSA::GetDriverNames(){
}

int MManagerALSA::OpenInputDevice(int id){

	try {
		MInputDeviceALSA* device = new MInputDeviceALSA(id);
		inputDevices.push_back(device);
	} catch (Resound::Exception& e) {
		std::cout << "ALSA Midi Error\n";
	}
	
}
int MManagerALSA::OpenOutputDevice(int id){
	try {
		MOutputDeviceALSA* device = new MOutputDeviceALSA(id);
		outputDevices.push_back(device);
	} catch (Resound::Exception& e) {
		std::cout << "ALSA Midi Error\n";
	}
}

void MManagerALSA::tick(){
	for(MInputDeviceList::iterator it = inputDevices.begin(); it != inputDevices.end(); it++){
		(*it)->tick();
	}
}


snd_seq_t* MManagerALSA::open_seq(const std::string name) {

    snd_seq_t *seq_handle;
    
    if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK) < 0) {
         std::cout << "Error opening ALSA sequencer.\n";
    }
    snd_seq_set_client_name(seq_handle, "ResoundGUI");
    if (snd_seq_create_simple_port(seq_handle, name.c_str(),
        SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION) < 0) {
        std::cout<< "Error creating sequencer port.\n";
    }
    return(seq_handle);
}

#endif
