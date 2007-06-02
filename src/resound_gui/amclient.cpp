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

//BOOST_CLASS_EXPORT_GUID(Resound::AMParameter, "Resound::AMParameter");

// AM Client -------------------------------------------------------------------------------------

Resound::AMClient::AMClient(int inputs, int outputs) :
Resound::OSCManager("8765"),
Resound::ParameterNamespace("audio_matrix")
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
			s << "/matrix/att/" << r << "/" << c; // generate the name
			AMParameter* node = new AMParameter(s.str()); // make the node
			node->set_osc_target(host, s.str()); // set the external osc method address
			register_parameter(s.str(), ParameterPtr(node));
		}
	}
}


// implement the ParameterNamespace interface
void* Resound::AMClient::SettingsPanel(wxWindow* parent)
{
	// open a sub system settings dialog

	return 0;
}

void* Resound::AMClient::SelectPanel(wxWindow* parent)
{
	// open an appropriate dialog for Parameter selection return the address or null address
	return new Resound::AudioMatrixSelectPanel(parent,this);
}



// tick
void Resound::AMClient::tick(float dT)
{

}

// ----------------------------------------- AudioMatrixSelectPanel -----------------
Resound::AudioMatrixSelectPanel::AudioMatrixSelectPanel(wxWindow* parent, ParameterNamespace* _subSystem)
		: PVSSelectPanel(parent,_subSystem)
{
	// cast subsystem to amClient - theoretically it can only be one of these
	AMClient* amClient = (AMClient*) subSystem;

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxGridSizer* gridSizer = new wxGridSizer(0,amClient->get_num_outputs()+1,1,1);

	wxScrolledWindow *scroll = new wxScrolledWindow(this,-1);



	topSizer->Add(new wxStaticText(this,-1,_("Select panel")));
	for(int r = 0; r <= amClient->get_num_inputs(); r++) {
		for(int c = 0; c <= amClient->get_num_outputs(); c++) {
			std::stringstream s;
			s << "/" << amClient->get_name() <<"/matrix/att/" << r << "/" << c; // generate the name
			Resound::AddressSelectWidget *panel = new Resound::AddressSelectWidget(scroll,-1,ParameterAddress(s.str())); // FIXME drastic pvar addressing changes // FIXED, pending testing

			wxColour bkColour;

			if(r == 0 && c == 0) // global
			{
				bkColour = wxColour(50,50,50);
			} else if(r > 0 && c ==0) // input
			{
				bkColour = wxColour(50,150,50);
			} else if(r == 0 && c > 0) // output
			{
				bkColour = wxColour(150,50,50);
			} else // matrix node
			{
				bkColour = wxColour(50,50,150);
			}

			panel->SetBackgroundColour(bkColour);

			gridSizer->Add(panel);
		}
	}

	// for scroll
	// set minimum size to represent real size of window
	// set sizer
	// set virtual size hints
	// layout the sizer

	scroll->SetMinSize(wxSize(400,300));
	scroll->SetSizer(gridSizer);
	gridSizer->SetVirtualSizeHints(scroll);   // set size hints to honour minimum size
	gridSizer->Layout();

	scroll->SetScrollbars(20, 20, 50, 50);

	topSizer->Add(scroll);

	SetSizer(topSizer);
	topSizer->Layout();

}


