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
#include "pch.h" // precompiler header

#include "automation.h"
#include "pvar.h"
#include "vumeterwidget.h"
#include "faderwidget.h"
#include "pvarwidgets.h"
#include "util.h"

#include "app.h"
#include "amclient.h" // classes header
#include <sstream>

lo_address global_host_address = 0; 

// AMParameter -------------------------------------------------------------------------------------

Resound::AMParameter::AMParameter(const EntityName& name) :
Parameter(name),
m_needsUpdate(false)
{}

Resound::AMParameter::~AMParameter()
{}

const float FADER_RANGE = 96.0f; //dB

void Resound::AMParameter::on_value_changed()
{
	// update the target value
	// assume that the midi fader range 0 - 127 represents a dB range from FADER_LOW_LIMIT dB - 0dB with a special case of 0 = -inf dB
	int f = get_value();
	if(f==0){ // gain -inf dB
		VERBOSE(std::cout << m_oscAddress.c_str() << " -infdB 0.0" << std::endl;)
		lo_send(global_host_address, m_oscAddress.c_str(), "f", 0.0f);
	} else { // range map into FADER_LOW_LIMIT dB - 0dB
		float pos = 1.0f/128.0f * f; 
		// formula below based on that of ardour2 . ie position has range 0 -> 1.0
		// 2 to the power of the eight root of the position scaled by the desired dB range ie 96 dB and shifted so it goes from -96dB to 0
		// so we get curve going from 0 to 1.0 with very steep graph
		float v = CLAMPF(pow (2.0,(sqrt(sqrt(sqrt(pos)))*(FADER_RANGE*2.0f)-(FADER_RANGE*2.0f))/6.0),0.,1.); // set the value of the node and clamp it
		VERBOSE(std::cout << m_oscAddress.c_str() << " " << v << " " << 20.0 * log10(v) << " dB" << std::endl;)
		lo_send(global_host_address, m_oscAddress.c_str(), "f", v);
	}
	m_needsUpdate = false;
}

void Resound::AMParameter::set_osc_target(std::string path){
	m_oscAddress = path;
}

BOOST_CLASS_EXPORT(Resound::AMParameter);

// AM Client -------------------------------------------------------------------------------------

BOOST_CLASS_EXPORT(Resound::AMClient);

Resound::AMClient::AMClient()
{
	global_host_address = lo_address_new(wxGetApp().get_server_ip().c_str(), wxGetApp().get_server_port().c_str());
}

Resound::AMClient::AMClient(int inputs, int outputs) :

Resound::ParameterNamespace("am") // FIXME
{
	global_host_address = lo_address_new(wxGetApp().get_server_ip().c_str(), wxGetApp().get_server_port().c_str());
	build_parameter_matrix(inputs,outputs); // fake matrix
}
Resound::AMClient::~AMClient()
{}

// build the audio matrix and associated Parameters
void Resound::AMClient::build_parameter_matrix(int numInputs, int numOutputs)
{

	// make pvar matrix
//	m_parameterMatrix.Create(numInputs+1,numOutputs+1);
	m_numInputs = numInputs;
	m_numOutputs = numOutputs;

	

	// fill in pvar details
	int r,c;
	for(r = 0; r < m_numInputs+1; r++) {
		for(c = 0; c < m_numOutputs+1; c++) {

			//create the local parameter and point it at its osc target
			std::stringstream s;
			s << "/att/" << r << "/" << c; // generate the name
			AMParameter* node = new AMParameter(s.str()); // make the node
			node->set_osc_target(std::string("/matrix") + s.str()); // set the external osc method address
			register_parameter(s.str(), ParameterPtr(node));
		}
	}
}

// tick
void Resound::AMClient::tick(float dT)
{

}




