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
#include "pvarwidgets.h"
#include "behaviour.h" // class's header file

#include <sstream>
// -------------------------------- Behaviour --------------------------------
int Resound::Behaviour::s_globalId=0;

// class constructor
Resound::Behaviour::Behaviour(std::string name)
{
	// tag on the unique global id
	std::stringstream s;
	s << name << "_" << get_global_id();
	m_name = s.str();
}
// class destructor
Resound::Behaviour::~Behaviour()
{}


// operations used in construction
void Resound::Behaviour::register_parameter(ParameterPtr param)
{
	// add a new Parameter to the local refference
	m_parameters.push_back(param);
	// register with the namespace manager
	std::stringstream s;
	s << "/behaviour/" << m_name << "/" << param->get_name(); // generate the name
	Resound::ParameterNamespaceManager::get_instance().register_parameter(s.str(),param);
}
void Resound::Behaviour::set_name(std::string name)
{
	// TODO this needs to be done
	//m_name = name;
	// rename all Parameters !
}
std::string Resound::Behaviour::get_name()
{
	return m_name;
}

// setting and getting the collective for this behaviour
void Resound::Behaviour::set_collective(const Collective& coll)
{
	m_collective = coll;
}
Resound::Collective& Resound::Behaviour::get_collective()
{
	return m_collective;
}



// --------------------------------Behaviour Class--------------------------------

Resound::BehaviourClassFactory::BehaviourClassFactory(BehaviourClassId classId, std::string classNiceName, BehaviourFactory factory) :
m_classId(classId),
m_classNiceName(classNiceName),
m_factory(factory)
{}

Resound::BehaviourClassId Resound::BehaviourClassFactory::get_class_id()
{
	return m_classId;
}

std::string Resound::BehaviourClassFactory::get_name()
{
	return m_classNiceName;
}

Resound::BehaviourPtr Resound::BehaviourClassFactory::create()
{
	// create one of these class of behaviour
	return m_factory();
}

// -------------------------------- Behaviour manager ----------------------------

Resound::BehaviourManager::BehaviourManager() :
ParameterNamespace("behaviour")
{
	m_nextId = 0;
}
Resound::BehaviourManager::~BehaviourManager()
{

}
void Resound::BehaviourManager::register_behaviour_class_factory(BehaviourClassFactory bClass)
{
	// check for existing
	BehaviourClassId classId = bClass.get_class_id();
	BehaviourClassFactoryMap::iterator i = m_classMap.find(classId);
	if(i != m_classMap.end()) {
		// throw behaviour class already exists
		throw Resound::RegisterBehaviourException();
	} else // add the class
	{
		m_classMap[classId] = bClass; // table look-up
	}
}

Resound::BehaviourPtr Resound::BehaviourManager::create_behaviour_direct(BehaviourClassId classId)
{
	// first search for the class id
	BehaviourClassFactoryMap::iterator i = m_classMap.find(classId);
	if(i != m_classMap.end()) {
		// do exception checking here because the plugin may throw
		BehaviourPtr temp = (*i).second.create(); // create using the factory

		if(temp) // if its ok
		{
			temp->m_classId = classId; // set the class id
		}
		return temp;
	} else {
		// classId does not exist
		// throw exception
		throw Resound::CreateBehaviourException();
	}
}

Resound::BehaviourPtr Resound::BehaviourManager::create_behaviour(BehaviourClassId classId)
{
	//TODO remove dialog from here
	if(classId == "") // check for null type and set
	{
		// have to get user to select type first

		// construct arraystring of all names
		wxArrayString aStr;
		std::vector<BehaviourClassId> idLookup; // temp array for lookup
		for(BehaviourClassFactoryMap::iterator i = m_classMap.begin(); i != m_classMap.end(); i++)
		{
			idLookup.push_back((*i).first); // store the associated FourCharId by index
			aStr.Add(wxConvertMB2WX((*i).second.get_name().c_str())); // add the string for the dialog // FIXME wxString conversion problem FIXED
		}
		int classIndex = wxGetSingleChoiceIndex(_T("Please select a behaviour class"),_T("Select Behaviour"),aStr);

		// lookup the index
		classId = idLookup[classIndex]; // set the class id
	}
	// create a behaviour and add it to the manager

	// first search for the class id
	BehaviourClassFactoryMap::iterator i = m_classMap.find(classId);
	if(i != m_classMap.end()) {
		// do exception checking here because the plugin may throw
		BehaviourPtr temp = (*i).second.create(); // create using the factory

		if(temp) // if its ok
		{
			std::stringstream s;
			s << "new " << (*i).second.get_name() << " Id:" << m_nextId;
			temp->set_name(s.str()); // set a temp name//FIXME string conversion FIXED
			temp->m_id = m_nextId; // set an id for the behaviour
			temp->m_classId = classId; // set the class id
			m_behaviourMap[m_nextId] = temp; // add to the map
			m_nextId++; // update unique id
		}
		return temp;
	} else {
		// classId does not exist
		// throw exception
		throw Resound::CreateBehaviourException();
	}

}


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
	topSizer->Add(new wxButton(this,BSID_CREATE,_T("Create Behaviour")));


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

