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

#include "wxincludes.h"
#include "masterfader.h"


// event table
BEGIN_EVENT_TABLE(Resound::MasterFader, wxPanel)
EVT_COMMAND(MFG_FADER1, saEVT_FADER_CHANGED, Resound::MasterFader::OnFaderChanged)
END_EVENT_TABLE()

// class constructor
Resound::MasterFader::MasterFader(wxWindow* parent, int id, int cc)
: wxPanel(parent,id),
parameter(cc),
m_id(id)
{
	SetWindowStyle(wxSIMPLE_BORDER );
	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *topTierSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *bottomTierSizer = new wxBoxSizer( wxVERTICAL );

	// faders meters
	fader1 = new Resound::FaderWidget(this,MFG_FADER1,0,0,128);
	//meter1 = new Resound::ParameterVUMeterWidget(this,MFG_METER1,rand() % 128,0,128);
	//meter2 = new Resound::VUMeterWidget(this,MFG_METER2,rand() % 128,0,128);

	topTierSizer->Add(fader1,wxSizerFlags(0).Align(0).Border(wxALL,0)); // fader
	//topTierSizer->Add(meter1,wxSizerFlags(0).Align(0).Border(wxALL,0)); //meter1
	//topTierSizer->Add(meter2,wxSizerFlags(0).Align(0).Border(wxALL,0)); // meter2

	// buttons
	collectiveWidget = new Resound::CollectiveWidget(this,MFG_ASSIGN1,_T("Assign"),
	                   preset.get_collective(), true, 16); // show max of 16 VU meters...
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
Resound::MasterFader::~MasterFader()
{
	// insert your code here
}

// respond to mouse fader movement
void Resound::MasterFader::OnFaderChanged(wxCommandEvent &event)
{
	int val = event.GetInt();
	preset.set_value(val);

	// send midi message
	//MManager::GetSingleton().SendMidiMessage(1,MakeStatusByte(MIDI_CONTROL_CHANGE,GetId()),7,val); // FIXME midi fix for linux
	// FIXME note that the fader id was previously used for channel and now isnt
}

void Resound::MasterFader::SetValue(int value)
{
	fader1->SetValueNoEvent(value);
	preset.set_value(value);
}


void Resound::MasterFader::OnMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp)
{
	// decode the message into the local storage buffers
	int type = MIDIStatusToType(status);
	int channel = MIDIStatusToChannel(status);
	switch(type) {
	case MIDI_CONTROL_CHANGE:
		if(channel == (GetId()&15) && parameter == dataA) {// FIXME note that the fader id was previously used for channel and now isnt
			SetValue((int) dataB);
		}
		break;
	}
}
void Resound::MasterFader::store_to_preset(int index){
	MasterFaderPreset& p = PerformanceManager::get_instance().get_performance()->get_preset(index).get_master_fader_preset(m_id);
	p = preset;
}
void Resound::MasterFader::recall_from_preset(int index){
	MasterFaderPreset& p = PerformanceManager::get_instance().get_performance()->get_preset(index).get_master_fader_preset(m_id);
	preset = p;
	collectiveWidget->SetCollective(preset.get_collective());
	preset.set_value(p.get_value());
	fader1->SetValueNoEvent(p.get_value());
}

