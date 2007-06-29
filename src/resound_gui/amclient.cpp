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


// AMParameter -------------------------------------------------------------------------------------

Resound::AMParameter::AMParameter(const EntityName& name) :
Parameter(name),
m_needsUpdate(false)
{}

Resound::AMParameter::~AMParameter()
{}

void Resound::AMParameter::on_value_changed()
{
	// update the target value
	float v = CLAMPF((float)get_value() * (1.0f/128.0f), 0.0f, 1.0f); // set the value of the node and clamp it
	lo_send(m_hostAddress, m_oscAddress.c_str(), "f", v);
	m_needsUpdate = false;
}

void Resound::AMParameter::set_osc_target(lo_address host, std::string path){
	m_hostAddress = host;
	m_oscAddress = path;
}

BOOST_CLASS_EXPORT(Resound::AMParameter);

// AM Client -------------------------------------------------------------------------------------

Resound::AMClient::AMClient(int inputs, int outputs) :
Resound::OSCManager("8765"),
Resound::ParameterNamespace("am")
{
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

	lo_address host = lo_address_new(NULL, "5678");

	// fill in pvar details
	int r,c;
	for(r = 0; r < m_numInputs+1; r++) {
		for(c = 0; c < m_numOutputs+1; c++) {

			//create the local parameter and point it at its osc target
			std::stringstream s;
			s << "/att/" << r << "/" << c; // generate the name
			AMParameter* node = new AMParameter(s.str()); // make the node
			node->set_osc_target(host, std::string("/matrix") + s.str()); // set the external osc method address
			register_parameter(s.str(), ParameterPtr(node));
		}
	}
}

// tick
void Resound::AMClient::tick(float dT)
{

}




