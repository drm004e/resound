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

#include "pvar.h" // class's header file
#include <iostream>
// -------------------------------- Parameter --------------------------------
SA::Parameter::Parameter() :
m_value(0),
m_name("No name"),
m_isLocked(false)
{}

SA::Parameter::~Parameter()
{}

int SA::Parameter::get_value()
{
	// obtain the value
	if(m_isLocked)
		return m_lockedValue;
	return m_value;
}
void SA::Parameter::set_value(int oldVal, int newVal)
{
	// set using previous value
	int delta = newVal - oldVal;
	if(delta != 0) {
		m_value += delta; // summing
		this->on_value_changed(); // call virtual notification function
	}

}
void SA::Parameter::set_value_direct(int newVal)
{
	// set directly used to initialize
	if(newVal != m_value) {
		m_value = newVal; // direct
		this->on_value_changed(); // call virtual notification function
	}
}
void SA::Parameter::lock(int lockedValue)
{
	// locks the value - the real value will still be changed but GetValue will return the locked value
	m_isLocked = true;
	m_lockedValue = lockedValue;
	this->on_value_changed(); // call virtual notification function
}

void SA::Parameter::unlock()
{
	// get value will return the real value;
	m_isLocked = false;
	this->on_value_changed(); // call virtual notification function
}

void SA::Parameter::on_value_changed()
{}

void SA::Parameter::set_name(std::string name)
{
	m_name = name;
}

std::string SA::Parameter::get_name()
{
	return m_name;
}
// -------------------------------- ParameterNamespace ------------------
void SA::ParameterNamespace::register_parameter(std::string address, ParameterPtr param){
	SA::ParameterNamespaceManager::get_instance().register_parameter(std::string("/") + m_name + address, param);
}
// -------------------------------- ParameterNamespaceManager ------------------
// singleton manager object
SA::ParameterNamespaceManager* SA::ParameterNamespaceManager::s_singleton = 0;
SA::ParameterNamespaceManager& SA::ParameterNamespaceManager::get_instance()
{
	if(s_singleton == 0) {
		s_singleton = new ParameterNamespaceManager();
	}
	return *s_singleton;
}
void SA::ParameterNamespaceManager::destroy_instance()
{
	if(s_singleton)
		delete s_singleton;
}

SA::ParameterNamespaceManager::ParameterNamespaceManager()
{}

SA::ParameterNamespaceManager::~ParameterNamespaceManager()
{}

void SA::ParameterNamespaceManager::register_parameter_namespace(ParameterNamespacePtr parameterNamespace)
{
	//subSystem->SetId(subSystemList.size()); // sets the global id of this sub system // FIXME sub system ids no longer exist
	m_parameterNamespaceList.push_back(parameterNamespace);
}
SA::ParameterPtr SA::ParameterNamespaceManager::get_parameter(const ParameterAddress &addr)
{
	
	ParameterAddressMap::iterator it = m_parameterAddressMap.find(addr);
	if(it != m_parameterAddressMap.end()){
		return it->second;
	} else {
		throw ParameterAddressException(addr);
	}
}
int SA::ParameterNamespaceManager::peek_parameter_value(const ParameterAddress &addr)
{
	return get_parameter(addr)->get_value();
}
void SA::ParameterNamespaceManager::register_parameter(std::string address, ParameterPtr param){
	ParameterAddressMap::iterator it = m_parameterAddressMap.find(address);
	if(it != m_parameterAddressMap.end()){
		throw ParameterAddressException(address); // non unique address registered
	} else {
		m_parameterAddressMap[address]=param;
		std::cout << "Registered Parameter: " << address << std::endl;
	}
}
// -------------------------------- Parameter link --------------------------------
SA::ParameterLink::ParameterLink() :
m_lastValue(0)
{}

SA::ParameterLink::ParameterLink(const ParameterAddress &t) :
m_lastValue(0),
m_targetAddress(t)
{}

// copy construct
// each ParameterLink must operat successfully on targets so
// copy and assignment must take this into account
// targets are maintained but influence on values is reset
SA::ParameterLink::ParameterLink(const ParameterLink& p) :
m_lastValue(0),
m_targetAddress(p.m_targetAddress)
{}

//assignemtn
SA::ParameterLink& SA::ParameterLink::operator=(const ParameterLink& p){
	if(&p == this) {
		return *this; // ignore self assignment
	}
	set_value(0); // remove influence on old target
	m_targetAddress = p.m_targetAddress; // get the new target
	m_lastValue = 0; // set last value to 0
	return *this;
}


SA::ParameterLink::~ParameterLink(){
	set_value(0); // get rid of influence before destroying
}

void SA::ParameterLink::set_target(const ParameterAddress &addr){
	// set the target - assumes valid address
	// notify previous target and remove influence
	set_value(0); // removes any influence
	m_targetAddress = addr;
}
void SA::ParameterLink::set_value(int val){
	// uses the suming system to set a new value
	if(val != m_lastValue) {
		// only update if different because it avoids calls to obtain the target param
		ParameterNamespaceManager::get_instance().get_parameter(m_targetAddress)->set_value(m_lastValue, val);
		m_lastValue = val;
	}
}
int SA::ParameterLink::get_value()
{
	return ParameterNamespaceManager::get_instance().peek_parameter_value(m_targetAddress);
}

SA::ParameterPtr SA::ParameterLink::get_parameter()
{
	return ParameterNamespaceManager::get_instance().get_parameter(m_targetAddress);
}


// -------------------------------- Element ------------------------------------------

SA::Element::Element()
{}
SA::Element::~Element()
{}
void SA::Element::set_value(int value)
{
	for(int n = 0; n < m_parameterLinkArray.size(); n++) {
		m_parameterLinkArray[n].set_value(value);
	}
}
// ParameterLink insert and add
void SA::Element::add_link(const ParameterLink& link)
{
	m_parameterLinkArray.push_back(link);
}
void SA::Element::insert_link(int index, const ParameterLink& link)
{
	// insert at index
	ParameterLinkArray::iterator it = m_parameterLinkArray.begin() + index + 1;
	m_parameterLinkArray.insert(it,link);
}
void SA::Element::prepend_link(int index, const ParameterLink& link)
{
	// insert before index
	ParameterLinkArray::iterator it = m_parameterLinkArray.begin() + index;
	m_parameterLinkArray.insert(it,link);
}



// -------------------------------- Collective ---------------------------------------
SA::Collective::Collective()
{
	// add_element(); // a single basic element
}
SA::Collective::~Collective()
{
}

void SA::Collective::set_value(int value)
{
	for(int n = 0; n < m_elementArray.size(); n++) {
		m_elementArray[n].set_value(value);
	}
}

void SA::Collective::display()
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

void SA::Collective::add_element(const Element& element)
{
	// add element at end
	m_elementArray.push_back(element);
};
void SA::Collective::insert_element(int index, const Element& element)
{
	// insert at index
	ElementArray::iterator it = m_elementArray.begin() + index + 1;
	m_elementArray.insert(it,element);

};
void SA::Collective::prepend_element(int index, const Element& element)
{
	// insert before element at index
	ElementArray::iterator it = m_elementArray.begin() + index;
	m_elementArray.insert(it,element);
};

void SA::Collective::remove_element(int index)
{
	ElementArray::iterator it = m_elementArray.begin() + index;
	m_elementArray.erase(it);
}



// --------------------------------- Collective Cursor Edit ----------------------------
SA::CollectiveCursorEdit::CollectiveCursorEdit(Collective* collective)
{
	// construct passing collective to be edited
	m_collective = collective;
	set_last(); // move to last element;
}

SA::Collective* SA::CollectiveCursorEdit::get_collective()
{
	return m_collective;
}

SA::Element& SA::CollectiveCursorEdit::get_current_element() // change to & in implementation!
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

int SA::CollectiveCursorEdit::get_cursor_position()
{
	// get the cursor position -1 is head element, 0 is first real element
	return m_cursorIndex;
}

void SA::CollectiveCursorEdit::set_cursor_position(int newCursorIndex)
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

void SA::CollectiveCursorEdit::add_link(const ParameterLink& link)
{
	// adds a link at the current location

	// check for head
	if(m_cursorIndex == -1) {
		insert();
	}
	(*m_collective)[m_cursorIndex].add_link(link);
}

void SA::CollectiveCursorEdit::clear_links()
{
	// FIXME gui functionality removed from pvar
	//wxMessageBox(_T("CollectiveCursorEdit::ClearLinks() - Not implemented yet!"));
}

void SA::CollectiveCursorEdit::next()
{
	// moves to the next element
	int numElements = m_collective->get_num_elements();
	++m_cursorIndex;
	if(m_cursorIndex >= numElements) {
		m_cursorIndex = numElements-1;
	}
}
void SA::CollectiveCursorEdit::previous()
{
	// moves to the previous element
	--m_cursorIndex;
	if(m_cursorIndex < -1) {
		m_cursorIndex = -1;
	}
}
void SA::CollectiveCursorEdit::insert(const Element& element)
{
	// insert element after current element // cursor is set to the new element
	m_collective->prepend_element(m_cursorIndex + 1, element);
	m_cursorIndex++;
}

void SA::CollectiveCursorEdit::shift_left()
{
	if (m_cursorIndex >= 1) // shift left cannot apply to the first element or head!
	{
		Element elementToBeShifted = get_current_element(); // make a copy of the current element
		remove(); // remove it
		previous(); // shift the cursor back...
		insert(elementToBeShifted);
	}
}

void SA::CollectiveCursorEdit::shift_right()
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
void SA::CollectiveCursorEdit::remove()
{
	if (m_cursorIndex > -1) // as long as we are not at the collective head...
	{
		m_collective->remove_element(m_cursorIndex); // ...remove element from collective...
		previous(); // ...and call own function to decrement the cursor
	}
	// Do we need a check to see if the cursor is BEYOND the last valid element???
}

void SA::CollectiveCursorEdit::set_head()
{
	// move to the head element // move to element -1
	m_cursorIndex = -1;
}
void SA::CollectiveCursorEdit::set_first()
{
	// move to the first element
	m_cursorIndex = 0;
}
void SA::CollectiveCursorEdit::set_last()
{
	// move to the last element
	m_cursorIndex = m_collective->get_num_elements() - 1;
}
