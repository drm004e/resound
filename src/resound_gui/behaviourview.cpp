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
#include "pvar.h" // class's header file


#include "behaviour.h"
#include "amclient.h"
#include "vumeterwidget.h"
#include "monitornodewidget.h"
#include "pvarwidgets.h"

#include "behaviourview.h" // class's header file

//------------------------------------------ BehaviourViewItem ----------------------------
//events
BEGIN_EVENT_TABLE(SA::BehaviourViewItem, wxPanel)
EVT_BUTTON(BSID_RENAME, SA::BehaviourViewItem::OnRename)
END_EVENT_TABLE()

// class constructor
SA::BehaviourViewItem::BehaviourViewItem(wxWindow* parent, int id, Behaviour* b)
		: wxPanel(parent, id)
{
	behaviour = b;

	SetWindowStyle(wxNO_BORDER | wxCLIP_CHILDREN);
	SetBackgroundColour(wxColour(30,100,150));
	SetForegroundColour(wxColour(255,255,255));

	// construct the sub objects and sizer
	wxSizer* topSizer = new wxBoxSizer( wxVERTICAL );
	label = new wxStaticText(this,-1,behaviour->GetName());
	topSizer->Add(label);

	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	sizer->Add(new wxButton(this,BSID_EDITOR,_("Editor")));
	sizer->Add(new wxButton(this,BSID_RENAME,_("Rename")));
	collectiveWidget = new SA::CollectiveWidget(this,-1,_T("Assign"), &behaviour->GetCollective(), true);
	sizer->Add(collectiveWidget);

	for(int n = 0; n < behaviour->GetNumPVars(); n++) {
		sizer->Add(new SA::MonitorNodeWidget(this,-1,PVarAddress(1,0,behaviour->GetId(),n)));
	}

	topSizer->Add(sizer);

	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}


// class destructor
SA::BehaviourViewItem::~BehaviourViewItem()
{
	// insert your code here
}
void SA::BehaviourViewItem::OnRename(wxCommandEvent &event)
{
	behaviour->SetName(wxGetTextFromUser(_("Enter a new name for this behaviour"),_("Name Behaviour"),behaviour->GetName()));
	label->SetLabel(behaviour->GetName());
}
//----------------------------------------- BehaviourView ---------------------------------
//events
BEGIN_EVENT_TABLE(SA::BehaviourView, wxScrolledWindow)
EVT_BUTTON( BSID_CREATE, SA::BehaviourView::OnCreateBehaviour)
END_EVENT_TABLE()

// class constructor
SA::BehaviourView::BehaviourView(wxWindow* parent, int id, BehaviourManager* manager)
		: wxScrolledWindow(parent, id, wxPoint(0,0), wxSize(320,240))
{
	behaviourManager = manager;
	// construct the sub objects and sizer
	topSizer = new wxBoxSizer( wxVERTICAL );

	topSizer->Add(new wxStaticText(this,-1,_("Behaviour panel")));
	topSizer->Add(new wxButton(this,BSID_CREATE,_("Create Behaviour")));


	// build the dynamic section of the panel
	behaviourSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add(behaviourSizer);

	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}


// class destructor
SA::BehaviourView::~BehaviourView()
{
	// insert your code here
}

void SA::BehaviourView::BuildPanel()
{
	// build of the panel
	behaviourSizer->Remove(0); // remove old sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	behaviourSizer->Add(sizer);
	for(BehaviourMap::iterator i = behaviourManager->GetBehaviourMap().begin(); i !=  behaviourManager->GetBehaviourMap().end(); i++) {
		int id = (*i).first;
		Behaviour* b = (*i).second;
		sizer->Add(new BehaviourViewItem(this,-1,b));
	}
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

void SA::BehaviourView::OnCreateBehaviour(wxCommandEvent &event)
{
	try {
		behaviourManager->CreateBehaviour();
	} catch(SA::CreateBehaviourException& e) {
		//Cancel was pressed so ignore this exception here.
	}
	BuildPanel();
}
