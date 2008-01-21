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

#include "resound_client/automation.h" 

// ----------------------------------------- Automation manager --------------

// null singleton
Resound::AutomationManager* Resound::AutomationManager::s_singleton = 0;

Resound::AutomationManager& Resound::AutomationManager::get_instance()
{
	if(s_singleton == 0) {
		s_singleton = new Resound::AutomationManager();
	}
	return *s_singleton;
}

void Resound::AutomationManager::destroy_instance()
{
	if(s_singleton) {
		delete s_singleton;
		s_singleton = 0;
	}
}

// -------------

// class constructor
Resound::AutomationManager::AutomationManager()
{
	// start the timer // 5 miliseconds interval
	// sort out stopwatch
	// get accurate timing
}

// class destructor
Resound::AutomationManager::~AutomationManager()
{
}

void Resound::AutomationManager::add_automated_object(AutomatedObject* object)
{
	m_automatedList.push_back(object);
}
void Resound::AutomationManager::remove_automated_object(AutomatedObject* object)
{
	// remove object
	m_automatedList.remove(object);
}


void Resound::AutomationManager::tick(float dT)
{
	for(std::list<AutomatedObject*>::iterator i = m_automatedList.begin(); i != m_automatedList.end(); i++) {
		(*i)->tick(dT);
	}
}

// --------------------------------------- Automated Object ----------------------
Resound::AutomatedObject::AutomatedObject()
{
	Resound::AutomationManager::get_instance().add_automated_object(this);
}

Resound::AutomatedObject::~AutomatedObject()
{
	Resound::AutomationManager::get_instance().remove_automated_object(this);
}
