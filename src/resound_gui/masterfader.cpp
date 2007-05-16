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
#include "pch.h" // precompiler header

#include "automation.h"
#include "pvar.h"
#include "vumeterwidget.h"
#include "faderwidget.h"
#include "pvarwidgets.h"
#include "masterfader.h" // class's header file

// event table
BEGIN_EVENT_TABLE(SA::MasterFader, wxPanel)
EVT_COMMAND(MFG_FADER1, saEVT_FADER_CHANGED, SA::MasterFader::OnFaderChanged)
END_EVENT_TABLE()

// class constructor
SA::MasterFader::MasterFader(wxWindow* parent, int id)
		: wxPanel(parent,id)
{
	SetWindowStyle(wxSIMPLE_BORDER | wxCLIP_CHILDREN);
	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *topTierSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *bottomTierSizer = new wxBoxSizer( wxVERTICAL );

	// faders meters
	fader1 = new SA::FaderWidget(this,MFG_FADER1,0,0,128);
	meter1 = new SA::PVarVUMeterWidget(this,MFG_METER1,rand() % 128,0,128);
	meter2 = new SA::VUMeterWidget(this,MFG_METER2,rand() % 128,0,128);

	topTierSizer->Add(fader1,wxSizerFlags(0).Align(0).Border(wxALL,0)); // fader
	topTierSizer->Add(meter1,wxSizerFlags(0).Align(0).Border(wxALL,0)); //meter1
	topTierSizer->Add(meter2,wxSizerFlags(0).Align(0).Border(wxALL,0)); // meter2

	// buttons
	collectiveWidget = new SA::CollectiveWidget(this,MFG_ASSIGN1,_T("Assign"),
	                   &collective, true, 16); // show max of 16 VU meters...
	bottomTierSizer->Add(collectiveWidget, wxSizerFlags(0).Align(0).Expand().Border(wxALL,0));

	// sizers
	topSizer->Add(topTierSizer,wxSizerFlags(0).Align(0).Border(wxALL,3));
	topSizer->Add(bottomTierSizer,wxSizerFlags(0).Align(0).Expand().Border(wxALL,3));
	// label
	topSizer->Add(new wxStaticText(this,MFG_LABEL,wxString::Format(_T("%d"),id+1), wxPoint(0,0),wxSize(38,20),wxALIGN_CENTRE),wxSizerFlags(0).Align(0).Border(wxALL,0));



	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

// class destructor
SA::MasterFader::~MasterFader()
{
	// insert your code here
}

// respond to mouse fader movement
void SA::MasterFader::OnFaderChanged(wxCommandEvent &event)
{
	int val = event.GetInt();
	collective.SetValue(val);

	// send midi message
	MManager::GetSingleton().SendMidiMessage(1,MakeStatusByte(MIDI_CONTROL_CHANGE,GetId()),7,val);
}

void SA::MasterFader::SetValue(int value)
{
	fader1->SetValue(value);
}

void SA::MasterFader::Save(wxDataOutputStream& stream)
{
	stream << fader1->GetValue(); // save the current value of this fader
	collective.Save(stream);
}

void SA::MasterFader::Load(wxDataInputStream& stream)
{
	int v;
	stream >> v;
	collective.Load(stream);
	collectiveWidget->SetCollective(&collective);
	fader1->SetValue(v); // load value of this fader
}

void SA::MasterFader::OnMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp)
{
	// decode the message into the local storage buffers
	int type = MIDIStatusToType(status);
	int channel = MIDIStatusToChannel(status);
	switch(type) {
	case MIDI_CONTROL_CHANGE:
		if(channel == GetId()) {
			SetValue((int) dataB);
		}
		break;
	}
}
