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

#include "performance.h" // class's header file
#include <iostream>

//#include <boost/serialization/export.hpp> // this file has interesting meta programming and has to be in a specific place

//BOOST_CLASS_EXPORT(Parameter);

// ------------------------------- Entity ----------------------------------
Resound::Entity::Entity(const EntityName& name) :
m_name(name)
{}

Resound::Entity::~Entity()
{}

void Resound::Entity::set_name(const EntityName& name){
	m_name = name;
}

const Resound::EntityName& Resound::Entity::get_name(){
	return m_name;
}

// -------------------------------- Parameter --------------------------------
Resound::Parameter::Parameter(const EntityName& name) :
Entity(name),
m_value(0),
m_lockedValue(0),
m_isLocked(false)
{}

Resound::Parameter::~Parameter()
{}

int Resound::Parameter::get_value()
{
	// obtain the value
	if(m_isLocked)
		return m_lockedValue;
	return m_value;
}
void Resound::Parameter::set_value(int oldVal, int newVal)
{
	// set using previous value
	int delta = newVal - oldVal;
	if(delta != 0) {
		m_value += delta; // summing
		this->on_value_changed(); // call virtual notification function
	}

}
void Resound::Parameter::set_value_direct(int newVal)
{
	// set directly used to initialize
	if(newVal != m_value) {
		m_value = newVal; // direct
		this->on_value_changed(); // call virtual notification function
	}
}
void Resound::Parameter::lock(int lockedValue)
{
	// locks the value - the real value will still be changed but GetValue will return the locked value
	m_isLocked = true;
	m_lockedValue = lockedValue;
	this->on_value_changed(); // call virtual notification function
}

void Resound::Parameter::unlock()
{
	// get value will return the real value;
	m_isLocked = false;
	this->on_value_changed(); // call virtual notification function
}

void Resound::Parameter::on_value_changed()
{}


// -------------------------------- ParameterNamespace ------------------
void Resound::ParameterNamespace::register_parameter(std::string address, ParameterPtr param){
	RESOUND_NAMESPACE()->register_parameter(std::string("/") + get_name() + address, param);
}
// -------------------------------- ParameterNamespaceManager ------------------


Resound::ParameterNamespaceManager::ParameterNamespaceManager()
{}

Resound::ParameterNamespaceManager::~ParameterNamespaceManager()
{}

void Resound::ParameterNamespaceManager::register_parameter_namespace(ParameterNamespacePtr parameterNamespace)
{
	//subSystem->SetId(subSystemList.size()); // sets the global id of this sub system // FIXME sub system ids no longer exist
	m_parameterNamespaceList.push_back(parameterNamespace);
}
Resound::ParameterPtr Resound::ParameterNamespaceManager::get_parameter(const ParameterAddress &addr)
{
	
	ParameterAddressMap::iterator it = m_parameterAddressMap.find(addr);
	if(it != m_parameterAddressMap.end()){
		return it->second;
	} else {
		throw ParameterAddressException(addr);
	}
}
int Resound::ParameterNamespaceManager::peek_parameter_value(const ParameterAddress &addr)
{
	return get_parameter(addr)->get_value();
}
void Resound::ParameterNamespaceManager::register_parameter(std::string address, ParameterPtr param){
	ParameterAddressMap::iterator it = m_parameterAddressMap.find(address);
	if(it != m_parameterAddressMap.end()){
		throw ParameterAddressException(address); // non unique address registered
	} else {
		m_parameterAddressMap[address]=param;
		std::cout << "Registered Parameter: " << address << std::endl;
	}
}
// -------------------------------- Parameter link --------------------------------
Resound::ParameterLink::ParameterLink() :
m_lastValue(0)
{}

Resound::ParameterLink::ParameterLink(const ParameterAddress &t) :
m_lastValue(0),
m_targetAddress(t),
m_targetPtr(RESOUND_NAMESPACE()->get_parameter(m_targetAddress))
{}

// copy construct
// each ParameterLink must operat successfully on targets so
// copy and assignment must take this into account
// targets are maintained but influence on values is reset
Resound::ParameterLink::ParameterLink(const ParameterLink& p) :
m_lastValue(0),
m_targetAddress(p.m_targetAddress),
m_targetPtr(p.m_targetPtr)
{}

//assignemtnPtr
Resound::ParameterLink& Resound::ParameterLink::operator=(const ParameterLink& p){
	if(&p == this) {
		return *this; // ignore self assignment
	}
	set_target(p.m_targetAddress); // get the new target
	return *this;
}


Resound::ParameterLink::~ParameterLink(){
	set_value(0); // get rid of influence before destroying
}

void Resound::ParameterLink::set_target(const ParameterAddress &addr){
	// set the target - assumes valid address
	// notify previous target and remove influence
	set_value(0); // removes any influence
	m_targetAddress = addr;
	m_targetPtr = RESOUND_NAMESPACE()->get_parameter(m_targetAddress);
	m_lastValue = 0; // set last value to 0
}
void Resound::ParameterLink::set_value(int val){
	// uses the suming system to set a new value
	if(val != m_lastValue) {
		// only update if different because it avoids calls to obtain the target param
		if(m_targetPtr){
			m_targetPtr->set_value(m_lastValue, val);
			m_lastValue = val;
		}
	}
}
int Resound::ParameterLink::get_value()
{
	if(m_targetPtr){
		return m_targetPtr->get_value();
	} else {
		return 0;
	}
}

Resound::ParameterPtr Resound::ParameterLink::get_parameter()
{
	return m_targetPtr;
}


// -------------------------------- Element ------------------------------------------

Resound::Element::Element()
{}
Resound::Element::~Element()
{}
void Resound::Element::set_value(int value)
{
	for(int n = 0; n < m_parameterLinkArray.size(); n++) {
		m_parameterLinkArray[n].set_value(value);
	}
}
// ParameterLink insert and add
void Resound::Element::add_link(const ParameterLink& link)
{
	m_parameterLinkArray.push_back(link);
}
void Resound::Element::insert_link(int index, const ParameterLink& link)
{
	// insert at index
	ParameterLinkArray::iterator it = m_parameterLinkArray.begin() + index + 1;
	m_parameterLinkArray.insert(it,link);
}
void Resound::Element::prepend_link(int index, const ParameterLink& link)
{
	// insert before index
	ParameterLinkArray::iterator it = m_parameterLinkArray.begin() + index;
	m_parameterLinkArray.insert(it,link);
}



// -------------------------------- Collective ---------------------------------------
Resound::Collective::Collective()
{
	// add_element(); // a single basic element
}
Resound::Collective::~Collective()
{
}

void Resound::Collective::set_value(int value)
{
	for(int n = 0; n < m_elementArray.size(); n++) {
		m_elementArray[n].set_value(value);
	}
}

void Resound::Collective::display()
{
/* FIXME removal of gui functions from pvar
	std::string str;
	for(int el = 0; el < m_elementArray.size(); el++) {
		str.Append(std::string::Format(_T("Element %d\n"),el));

		Element& element = m_elementArray[el];
		for(int n = 0; n < element.GetNumLinks(); n++) {
			ParameterLink& link = element[n];
			ParameterAddress pVarAddress = link.GetTarget();
			ParameterPtr pVar = link.GetParameter();
			str.Append(_T(" |- ( "));
			//str.Append(pVarAddress.get_address().c_str()); // FIXME string conversion
			str.Append(_T(" ) - "));
			//str.Append(pVar.get_name()); // FIXME string conversion
			str.Append(_T("\n"));
		}
	}
	wxMessageBox(str,_T("Collective"));
*/
}

void Resound::Collective::add_element(const Element& element)
{
	// add element at end
	m_elementArray.push_back(element);
};
void Resound::Collective::insert_element(int index, const Element& element)
{
	// insert at index
	ElementArray::iterator it = m_elementArray.begin() + index + 1;
	m_elementArray.insert(it,element);

};
void Resound::Collective::prepend_element(int index, const Element& element)
{
	// insert before element at index
	ElementArray::iterator it = m_elementArray.begin() + index;
	m_elementArray.insert(it,element);
};

void Resound::Collective::remove_element(int index)
{
	ElementArray::iterator it = m_elementArray.begin() + index;
	m_elementArray.erase(it);
}



// --------------------------------- Collective Cursor Edit ----------------------------
Resound::CollectiveCursorEdit::CollectiveCursorEdit(Collective* collective)
{
	// construct passing collective to be edited
	m_collective = collective;
	set_last(); // move to last element;
}

Resound::Collective* Resound::CollectiveCursorEdit::get_collective()
{
	return m_collective;
}

Resound::Element& Resound::CollectiveCursorEdit::get_current_element() // change to & in implementation!
{
	if (m_cursorIndex == -1) // if we are asking (erroneously) for the element at index -1...
	{
		/* TODO (James#5#): Un-caught exception!
		                    We should never ask for the element at index -1, 
		                    because this doesn't exist (it is the head). */
		throw std::exception(); // we don't need this actually, as long as we only ask for a +ve index!
	} else // if the cursor position is a valid element...
	{
		return (*m_collective)[m_cursorIndex]; // ...return a reference to it
	}
}

int Resound::CollectiveCursorEdit::get_cursor_position()
{
	// get the cursor position -1 is head element, 0 is first real element
	return m_cursorIndex;
}

void Resound::CollectiveCursorEdit::set_cursor_position(int newCursorIndex)
{
	if (newCursorIndex < -1) // if we tried to set the cursor to before the head 'element'...
	{
		m_cursorIndex = -1; // ...set cursor to head
	} else if (newCursorIndex > m_collective->get_num_elements()-1) // if we tried to set it to a non-existent element
	{
		m_cursorIndex = m_collective->get_num_elements()-1; // set it to the last existing element in the collective
	} else {
		m_cursorIndex = newCursorIndex; // is this 100% safe? Do we want to check that this element exists?
	}
}

void Resound::CollectiveCursorEdit::add_link(const ParameterLink& link)
{
	// adds a link at the current location

	// check for head
	if(m_cursorIndex == -1) {
		insert();
	}
	(*m_collective)[m_cursorIndex].add_link(link);
}

void Resound::CollectiveCursorEdit::clear_links()
{
	// FIXME gui functionality removed from pvar
	//wxMessageBox(_T("CollectiveCursorEdit::ClearLinks() - Not implemented yet!"));
}

void Resound::CollectiveCursorEdit::next()
{
	// moves to the next element
	int numElements = m_collective->get_num_elements();
	++m_cursorIndex;
	if(m_cursorIndex >= numElements) {
		m_cursorIndex = numElements-1;
	}
}
void Resound::CollectiveCursorEdit::previous()
{
	// moves to the previous element
	--m_cursorIndex;
	if(m_cursorIndex < -1) {
		m_cursorIndex = -1;
	}
}
void Resound::CollectiveCursorEdit::insert(const Element& element)
{
	// insert element after current element // cursor is set to the new element
	m_collective->prepend_element(m_cursorIndex + 1, element);
	m_cursorIndex++;
}

void Resound::CollectiveCursorEdit::shift_left()
{
	if (m_cursorIndex >= 1) // shift left cannot apply to the first element or head!
	{
		Element elementToBeShifted = get_current_element(); // make a copy of the current element
		remove(); // remove it
		previous(); // shift the cursor back...
		insert(elementToBeShifted);
	}
}

void Resound::CollectiveCursorEdit::shift_right()
{
	if (m_cursorIndex > -1 && m_cursorIndex < m_collective->get_num_elements()-1) // shift right cannot apply to last element or head!
	{
		Element elementToBeShifted = get_current_element(); // make a copy of the current element
		remove(); // remove it
		next(); // shift cursor forwards
		insert(elementToBeShifted);
	}
}

/// Removes the current element as indicated by the cursor position
/// Cursor is then set to the previous element or head
void Resound::CollectiveCursorEdit::remove()
{
	if (m_cursorIndex > -1) // as long as we are not at the collective head...
	{
		m_collective->remove_element(m_cursorIndex); // ...remove element from collective...
		previous(); // ...and call own function to decrement the cursor
	}
	// Do we need a check to see if the cursor is BEYOND the last valid element???
}

void Resound::CollectiveCursorEdit::set_head()
{
	// move to the head element // move to element -1
	m_cursorIndex = -1;
}
void Resound::CollectiveCursorEdit::set_first()
{
	// move to the first element
	m_cursorIndex = 0;
}
void Resound::CollectiveCursorEdit::set_last()
{
	// move to the last element
	m_cursorIndex = m_collective->get_num_elements() - 1;
}
