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
#include "sa_cpch.h" // precompiler header
#include "sa_automation.h" // class's header file

// ----------------------------------------- Automation manager --------------

// null songleton
SA::AutomationManager* SA::AutomationManager::singleton = 0;

SA::AutomationManager& SA::AutomationManager::GetSingleton()
{
	if(singleton == 0) {
		singleton = new SA::AutomationManager();
	}
	return *singleton;
}

void SA::AutomationManager::Destroy()
{
	if(singleton) {
		delete singleton;
		singleton = 0;
	}
}

// -------------

// class constructor
SA::AutomationManager::AutomationManager()
{
	Start(1000/25); // start the timer // 5 miliseconds interval
	// sort out stopwatch
	// get accurate timing
	clock.Start(0);
}

// class destructor
SA::AutomationManager::~AutomationManager()
{
	Stop();

}

void SA::AutomationManager::AddAutomatedObject(AutomatedObject* object)
{
	// add object
	if(object) {
		automatedList.push_back(object);
	}
}
void SA::AutomationManager::RemoveAutomatedObject(AutomatedObject* object)
{
	// remove object
	automatedList.remove(object);
}

// used this for notification of timer triggers may change in future
// due to timing issues on other platforms
// therefore kept in
void SA::AutomationManager::Notify()
{
	Tick();
}

void SA::AutomationManager::Tick()
{
	long t = clock.Time();
	clock.Start(0);

	float dT = (float)t * 0.001f;
	for(std::list<AutomatedObject*>::iterator i = automatedList.begin(); i != automatedList.end(); i++) {
		(*i)->Tick(dT);
	}
}

// --------------------------------------- Automated Object ----------------------
SA::AutomatedObject::AutomatedObject()
{
	SA::AutomationManager::GetSingleton().AddAutomatedObject(this);
}

SA::AutomatedObject::~AutomatedObject()
{
	SA::AutomationManager::GetSingleton().RemoveAutomatedObject(this);
}
