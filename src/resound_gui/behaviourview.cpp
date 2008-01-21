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
#include "behaviourview.h"
#include "monitornodewidget.h"
#include "resound_client/performance.h"
#include <sstream>

// ------------------------------------ Behaviour Select Panel -----------------------
BEGIN_EVENT_TABLE(Resound::BehaviourSelectPanel, PVSSelectPanel)
EVT_BUTTON( BSID_CREATE, Resound::BehaviourSelectPanel::OnCreateBehaviour)
END_EVENT_TABLE()

Resound::BehaviourSelectPanel::BehaviourSelectPanel(wxWindow* parent, ParameterNamespace* _subSystem)
		: PVSSelectPanel(parent,_subSystem)
{
	// construct the sub objects and sizer
	topSizer = new wxBoxSizer( wxVERTICAL );

	topSizer->Add(new wxStaticText(this,-1,_T("Behaviour panel")));
	//topSizer->Add(new wxButton(this,BSID_CREATE,_T("Create Behaviour")));


	// build the dynamic section of the panel
	behaviourSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add(behaviourSizer);

	// causes rebuild of the panel
	BuildPanel();

	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

void Resound::BehaviourSelectPanel::BuildPanel()
{
	// cast sub system to known type
	BehaviourManager* behaviourManager = (BehaviourManager*) subSystem;
	behaviourSizer->Remove(0); // remove old sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	behaviourSizer->Add(sizer);

	for(BehaviourMap::iterator i = behaviourManager->get_behaviour_map().begin(); i !=  behaviourManager->get_behaviour_map().end(); i++) {
		int id = (*i).first;
		BehaviourPtr b = (*i).second;
		wxGridSizer* gridSizer = new wxGridSizer(1,0,1,1);
		
		for(int n = 0; n < b->get_num_parameters() ; n++) {
			std::stringstream s;
			s << "/" << behaviourManager->get_name() << "/" << b->get_name() << "/" << b->get_parameter(n)->get_name(); // generate the name
			gridSizer->Add(new Resound::AddressSelectWidget(this,-1,ParameterAddress(s.str()))); 
		}
		sizer->Add(new wxStaticText(this,-1,wxConvertMB2WX(b->get_name().c_str()))); 
		sizer->Add(gridSizer);
	}

	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();

}
void Resound::BehaviourSelectPanel::OnCreateBehaviour(wxCommandEvent &event)
{
	// cast sub system to known type
	BehaviourManager* behaviourManager = (BehaviourManager*) subSystem;
	behaviourManager->create_behaviour();
	BuildPanel();
}

//------------------------------------------ BehaviourViewItem ----------------------------
//events
BEGIN_EVENT_TABLE(Resound::BehaviourViewItem, wxPanel)
END_EVENT_TABLE()

// class constructor
Resound::BehaviourViewItem::BehaviourViewItem(wxWindow* parent, int id, BehaviourPtr b)
		: wxPanel(parent, id)
{
	behaviour = b;

	SetWindowStyle(wxNO_BORDER | wxCLIP_CHILDREN);
	SetBackgroundColour(wxColour(30,100,150));
	SetForegroundColour(wxColour(255,255,255));

	// construct the sub objects and sizer
	wxSizer* topSizer = new wxBoxSizer( wxVERTICAL );
	label = new wxStaticText(this,-1,wxString(wxConvertMB2WX(behaviour->get_name().c_str())));
	topSizer->Add(label);

	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	sizer->Add(new wxButton(this,BSID_REMOVE,_("Remove")));
	collectiveWidget = new Resound::CollectiveWidget(this,-1,_T("Assign"), &behaviour->get_collective(), true);
	sizer->Add(collectiveWidget);

	for(int n = 0; n < behaviour->get_num_parameters(); n++) { 
		std::stringstream s;
		s << "/bm/" << behaviour->get_name() << "/" << behaviour->get_parameter(n)->get_name();
		sizer->Add(new Resound::MonitorNodeWidget(this,-1,ParameterAddress(s.str())));
	}

	topSizer->Add(sizer);

	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}


// class destructor
Resound::BehaviourViewItem::~BehaviourViewItem()
{
	// insert your code here
	std::cout << "~BehaviourViewItem\n";
}

//----------------------------------------- BehaviourView ---------------------------------
//events
BEGIN_EVENT_TABLE(Resound::BehaviourView, wxScrolledWindow)
EVT_BUTTON( BSID_CREATE, Resound::BehaviourView::OnCreateBehaviour)
EVT_BUTTON(BSID_REMOVE, Resound::BehaviourView::OnRemove)
END_EVENT_TABLE()

// class constructor
Resound::BehaviourView::BehaviourView(wxWindow* parent, int id)
		: wxScrolledWindow(parent, id, wxPoint(0,0), wxSize(320,240))
{
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
Resound::BehaviourView::~BehaviourView()
{
	// insert your code here
}

void Resound::BehaviourView::BuildPanel()
{
	Resound::BehaviourManager* behaviourManager = dynamic_cast<Resound::BehaviourManager*>(&RESOUND_NAMESPACE()->get_parameter_namespace(1));
	// build of the panel
	// destroy old windows
	std::list<wxWindow*>::iterator it;
	for(it = m_windowList.begin(); it != m_windowList.end(); it++)
	{
		(*it)->Destroy();
	}
	m_windowList.clear();
	behaviourSizer->Remove(0); // remove old sizer
	behaviourSizer->Layout();
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	behaviourSizer->Add(sizer);
	for(BehaviourMap::iterator i = behaviourManager->get_behaviour_map().begin(); i !=  behaviourManager->get_behaviour_map().end(); i++) {
		int id = (*i).first;
		BehaviourPtr b = (*i).second;
		wxWindow* p = new BehaviourViewItem(this,id,b);
		m_windowList.push_back(p);
		sizer->Add(p);
	}
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();

}

void Resound::BehaviourView::OnCreateBehaviour(wxCommandEvent &event)
{
	Resound::BehaviourManager* behaviourManager = dynamic_cast<Resound::BehaviourManager*>(&RESOUND_NAMESPACE()->get_parameter_namespace(1));
	try {
		behaviourManager->create_behaviour();
	} catch(Resound::CreateBehaviourException& e) {
		//Cancel was pressed so ignore this exception here.
	}
	BuildPanel();
}
void Resound::BehaviourView::OnRemove(wxCommandEvent &event)
{
	BehaviourViewItem* p = (BehaviourViewItem*)((wxWindow*)event.GetEventObject())->GetParent();
	Resound::BehaviourManager* behaviourManager = dynamic_cast<Resound::BehaviourManager*>(&RESOUND_NAMESPACE()->get_parameter_namespace(1));
	behaviourManager->remove_behaviour(p->GetId());
	std::cout << "Behaviour " << p->GetId();
	BuildPanel();	
}
