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

#include "resound_client/behaviour.h"
#include "resound_client/performance.h"
#include <iostream>
#include <sstream>
// -------------------------------- Behaviour Parameter ----------------------

BOOST_CLASS_EXPORT(Resound::BehaviourParameter);

void Resound::BehaviourParameter::on_value_changed(){
	m_parentBehaviour->on_parameter_changed();
}
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
Resound::Behaviour::~Behaviour(){
	std::cout<<"Behaviour~ "<< m_name <<std::endl;
}


// operations used in construction
void Resound::Behaviour::register_parameter(ParameterPtr param)
{
	// add a new Parameter to the local refference
	m_parameters.push_back(param);
	// register with the namespace manager
	std::stringstream s;
	s << "/bm/" << m_name << "/" << param->get_name(); // generate the name
	RESOUND_NAMESPACE()->register_parameter(s.str(),param);
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

void Resound::Behaviour::unregister_parameters(){
	// remove registered parameters
	for(ParameterArray::iterator it = m_parameters.begin(); it != m_parameters.end(); it++){
		std::stringstream s;
		s << "/bm/" << m_name << "/" << (*it)->get_name(); // generate the name // FIXME building the address like this is bad
		RESOUND_NAMESPACE()->remove_parameter(s.str());
	}
}


// --------------------------------Behaviour Class--------------------------------

Resound::BehaviourClassFactory::BehaviourClassFactory(BehaviourClassId classId, std::string classNiceName, BehaviourFactory factory) :
m_classId(classId),
m_classNiceName(classNiceName),
m_factory(factory)
{}

Resound::BehaviourClassId Resound::BehaviourClassFactory::get_class_id() const
{
	return m_classId;
}

std::string Resound::BehaviourClassFactory::get_name() const
{
	return m_classNiceName;
}

Resound::BehaviourPtr Resound::BehaviourClassFactory::create() const
{
	// create one of these class of behaviour
	return m_factory();
}

// -------------------------------- Behaviour manager ----------------------------

BOOST_CLASS_EXPORT(Resound::BehaviourManager);

Resound::BehaviourClassFactoryMap Resound::BehaviourManager::m_classMap; // implementation of static

Resound::BehaviourManager::BehaviourManager() :
ParameterNamespace("bm")
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
/*
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
*/
Resound::BehaviourPtr Resound::BehaviourManager::create_behaviour(BehaviourClassId classId)
{
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

void Resound::BehaviourManager::remove_behaviour(int id){
	BehaviourMap::iterator it = m_behaviourMap.find(id);
	if(it != m_behaviourMap.end()){
		it->second->unregister_parameters();
		m_behaviourMap.erase(it);
	}
}




