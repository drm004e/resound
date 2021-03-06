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

#include "wxincludes.h"
#include "performanceview.h"
#include "resound_client/resoundclient.h"
#include <sstream>

// event table

enum{
PRESET_COMBO = 0,
PRESET_BUTTON
};

BEGIN_EVENT_TABLE(Resound::PerformanceView, wxScrolledWindow)
EVT_COMMAND_RANGE(0,127,saEVT_FADER_CHANGED, Resound::PerformanceView::OnFaderMove)
EVT_COMBOBOX(PRESET_COMBO, Resound::PerformanceView::on_select_preset)
EVT_BUTTON(PRESET_BUTTON, Resound::PerformanceView::on_store_preset)
EVT_KEY_DOWN(Resound::PerformanceView::on_key)
END_EVENT_TABLE()

// class constructor
Resound::PerformanceView::PerformanceView(wxWindow* parent)
		: wxScrolledWindow(parent, wxID_ANY, wxPoint(0,0), wxSize(320,240)),
		  OSCManager(ResoundClient::get_instance().get_client_port().c_str())
{
	// insert your code here

	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *upperTierSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *lowerTierSizer = new wxBoxSizer( wxHORIZONTAL );

	Resound::MasterFader* temp; // temporary, for storing MasterFaders in fader array.

	int fId = 0;
	int n, g;
	for(n = 0; n < 4; n++) // create top row of MasterFaders
	{
		wxBoxSizer* grp = new wxBoxSizer(wxHORIZONTAL);
		for(g = 0; g < 4; g++)
		{
			temp = new Resound::MasterFader(this,fId++, 7); // create new MasterFader
			// register with OSC /// NOTE this is in two blocks see below
			std::stringstream s;
			s << "/fader/" << fId;
			add_method(s.str(),"f",Resound::PerformanceView::lo_cb_att, (void*)temp);
			/// end of block OSC
			masterFaderArray.push_back(temp); // store in array
			grp->Add(temp, wxSizerFlags(1).Align(0).Border(wxALL,1)); // add to GUI
		}void on_select_preset(wxCommandEvent& evt);
		upperTierSizer->Add(grp, wxSizerFlags(1).Align(0).Border(wxALL,5));
	}

	for(n = 0; n < 4; n++) // create bottom row of MasterFaders
	{
		wxBoxSizer* grp = new wxBoxSizer(wxHORIZONTAL);
		for(g = 0; g < 4; g++)
		{
			temp = new Resound::MasterFader(this,fId++, 10); // create new MasterFader
			// register with OSC /// NOTE this is in two blocks see above
			std::stringstream s;
			s << "/fader/" << fId;
			add_method(s.str(),"f",Resound::PerformanceView::lo_cb_att, (void*)temp);
			///// end of block OSC
			masterFaderArray.push_back(temp); // store in array
			grp->Add(temp, wxSizerFlags(1).Align(0).Border(wxALL,1)); // add to GUI
		}
		lowerTierSizer->Add(grp, wxSizerFlags(1).Align(0).Border(wxALL,5));
	}

	m_presetCombo = new wxComboBox(this, PRESET_COMBO, _("1:"));
	for(int n = 0; n < Resound::MAX_PRESETS; n++){
		m_presetCombo->Append(wxString::Format(_("%d:"),n));
	}
	topSizer->Add(m_presetCombo);
	topSizer->Add(new wxButton(this, PRESET_BUTTON, _("Store Preset")));
	topSizer->Add(upperTierSizer);
	topSizer->Add(lowerTierSizer);

	SetSizer(topSizer);
	//topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

// class destructor
Resound::PerformanceView::~PerformanceView()
{
	// insert your code here
}

// automation update for this window // triggers redraw
void Resound::PerformanceView::Tick(float dT)
{}

void Resound::PerformanceView::OnFaderMove(wxCommandEvent& event)
{
	int id = event.GetId();
	int newVal = event.GetInt();
}

void Resound::PerformanceView::Init()
{
	for (int n = 0; n < masterFaderArray.size(); n++) {
		masterFaderArray[n]->SetValue(0);
	}
}

void Resound::PerformanceView::store_to_preset(int index){
	for (int n = 0; n < masterFaderArray.size(); n++) {
		masterFaderArray[n]->store_to_preset(index);
	}
}
void Resound::PerformanceView::recall_from_preset(int index){
	for (int n = 0; n < masterFaderArray.size(); n++) {
		masterFaderArray[n]->recall_from_preset(index);
	}
}
void Resound::PerformanceView::on_select_preset(wxCommandEvent& evt){
	recall_from_preset(m_presetCombo->GetCurrentSelection());
}
void Resound::PerformanceView::on_store_preset(wxCommandEvent& evt){
	int p = wxGetNumberFromUser(_("Enter preset number"),_("Preset"),_("Store Preset"),m_presetCombo->GetCurrentSelection(),0,Resound::MAX_PRESETS-1);
	if (p > -1) store_to_preset(p); // wxGetNumberFromUser returns -1 if the dialog is cancelled
}
void Resound::PerformanceView::on_key(wxKeyEvent& evt){

	switch(evt.GetKeyCode()){
		case '1': m_presetCombo->Select(1); recall_from_preset(1); break;
		case '2': m_presetCombo->Select(2); recall_from_preset(2);break;
		case '3': m_presetCombo->Select(3); recall_from_preset(3);break;
		case '4': m_presetCombo->Select(4); recall_from_preset(4);break;
		case '5': m_presetCombo->Select(5); recall_from_preset(5);break;
		case '6': m_presetCombo->Select(6); recall_from_preset(6);break;
		case '7': m_presetCombo->Select(7); recall_from_preset(7);break;
		case '8': m_presetCombo->Select(8); recall_from_preset(8);break;
		case '9': m_presetCombo->Select(9); recall_from_preset(9);break;
		case '0': m_presetCombo->Select(0); recall_from_preset(0);break;
	}

}
int Resound::PerformanceView::lo_cb_att(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	Resound::MasterFader* f = (Resound::MasterFader*)user_data; 
	if(f && argv[0]){
		f->SetValue(int(argv[0]->f));
	}
    return 1;
}


