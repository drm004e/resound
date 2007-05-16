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
#include <resound_common/comms.h>
#include "amclient.h" // possible circular dep here requires pvar and comms
#include "vumeterwidget.h"
// -------------------------------- PVar --------------------------------
SA::PVar::PVar()
{
	value = 0;
	name = _T("No name");
	isLocked = false;
}

SA::PVar::~PVar()
{}

int SA::PVar::GetValue()
{
	// obtain the value
	if(isLocked)
		return lockedValue;
	return value;
}
void SA::PVar::SetValue(int oldVal, int newVal)
{
	// set using previous value
	int delta = newVal - oldVal;
	if(delta != 0) {
		value += delta; // summing
		this->OnValueChanged(); // call virtual notification function
	}

}
void SA::PVar::SetValueDirect(int newVal)
{
	// set directly used to initialize
	if(newVal != value) {
		value = newVal; // direct
		this->OnValueChanged(); // call virtual notification function
	}
}
void SA::PVar::Lock(int _lockedValue)
{
	// locks the value - the real value will still be changed but GetValue will return the locked value
	isLocked = true;
	lockedValue = _lockedValue;
	this->OnValueChanged(); // call virtual notification function
}

void SA::PVar::Unlock()
{
	// get value will return the real value;
	isLocked = false;
	this->OnValueChanged(); // call virtual notification function
}

void SA::PVar::OnValueChanged()
{
}

void SA::PVar::SetName(wxString _name)
{
	name = _name;
}

wxString SA::PVar::GetName()
{
	return name;
}
// --------------------------------PVarAddress--------------------------------------

void SA::PVarAddress::Save(wxDataOutputStream& stream)
{
	stream << subSystem;
	stream << type;
	stream << row;
	stream << col;
}

void SA::PVarAddress::Load(wxDataInputStream& stream)
{
	stream >> subSystem;
	stream >> type;
	stream >> row;
	stream >> col;
}

// -------------------------------- PVarAddressWidgetBase --------------------
SA::PVarAddressWidgetBase::PVarAddressWidgetBase(wxWindow* parent, int id, PVarAddress _addr)
		: wxWindow(parent,id)
{
	addr = _addr;
}
SA::PVarAddressWidgetBase::~PVarAddressWidgetBase()
{}

SA::PVarAddress SA::PVarAddressWidgetBase::PVarAddressWidgetBase::GetAddress()
{
	return addr;
};
// -------------------------------- PVSSettingsPanel -------------------------

SA::PVSSettingsPanel::PVSSettingsPanel(wxWindow* parent, PVarSubSystem* _subSystem, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxPanel(parent, id, pos, size, style, name)
{
	subSystem = _subSystem; // store the related sub system

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add(new wxStaticText(this,-1,name));
	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

// -------------------------------- PVSSelectPanel -------------------------
SA::PVSSelectPanel::PVSSelectPanel(wxWindow* parent, PVarSubSystem* _subSystem, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxPanel(parent, id, pos, size, style, name)
{
	subSystem = _subSystem; // store the related subsystem
}


// -------------------------------- PVar Sub System Manager ------------------
// singleton manager object
SA::PVarSubSystemManager* SA::PVarSubSystemManager::singleton = 0;
SA::PVarSubSystemManager& SA::PVarSubSystemManager::GetSingleton()
{
	if(singleton == 0) {
		singleton = new PVarSubSystemManager();
	}
	return *singleton;
}
void SA::PVarSubSystemManager::Destroy()
{
	if(singleton)
		delete singleton;
}

SA::PVarSubSystemManager::PVarSubSystemManager()
{}
SA::PVarSubSystemManager::~PVarSubSystemManager()
{
}
void SA::PVarSubSystemManager::RegisterPVarSubSystem(PVarSubSystem* subSystem)
{
	subSystem->SetId(subSystemList.size()); // sets the global id of this sub system
	subSystemList.push_back(subSystem);
}
SA::PVar& SA::PVarSubSystemManager::GetPVar(const PVarAddress &addr)
{
	int subId = addr.subSystem;
	// validate sub system address then pass on
	if(subId >= 0 && subId < subSystemList.size()) {
		PVarSubSystem* subSystem = subSystemList[subId];
		return subSystem->GetPVar(addr); // pass on to actual sub system // may yet return a null
	} else {
		return nullPVar; // its valid but it dont do shit // maybe exception here
	}
}
int SA::PVarSubSystemManager::PeekPVarValue(const PVarAddress &addr)
{
	return GetPVar(addr).GetValue();
}

// -------------------------------- PVar link --------------------------------
SA::PVarLink::PVarLink()
{
	lastValue = 0;
}

SA::PVarLink::PVarLink(const PVarAddress &t)
{
	target = t;
	lastValue = 0;
}

// copy construct
// each PVarLink must operat successfully on targets so
// copy and assignment must take this into account
// targets are maintained but influence on values is reset
SA::PVarLink::PVarLink(const PVarLink& p)
{
	target = p.target;
	lastValue = 0;
}

//assignemtn
SA::PVarLink& SA::PVarLink::operator=(const PVarLink& p)
{
	if(&p == this) {
		return *this;
	}
	SetValue(0); // remove influence
	target = p.target; // get the new target
	lastValue = 0; // set last value to 0
	return *this;
}


SA::PVarLink::~PVarLink()
{
	SetValue(0); // get rid of influence before destroying

}
void SA::PVarLink::SetTarget(const PVarAddress &addr)
{
	// set the target - assumes valid address
	// notify previous target and remove influence
	SetValue(0); // removes any influence
	target = addr;
}
void SA::PVarLink::SetValue(int val)
{
	// uses the suming system to set a new value
	if(val != lastValue) {
		// only update if different because it avoids calls to obtain the target param
		PVarSubSystemManager::GetSingleton().GetPVar(target).SetValue(lastValue, val);
		lastValue = val;
	}
}
int SA::PVarLink::GetValue()
{
	return PVarSubSystemManager::GetSingleton().PeekPVarValue(target);
}

SA::PVar& SA::PVarLink::GetPVar()
{
	return PVarSubSystemManager::GetSingleton().GetPVar(target);
}

void SA::PVarLink::Save(wxDataOutputStream& stream)
{
	target.Save(stream); // calls PVarAddress Save() function
}

void SA::PVarLink::Load(wxDataInputStream& stream)
{
	PVarAddress loadTarget;
	loadTarget.Load(stream); // calls PVarAddress Load() function
	SetTarget(loadTarget); // use SetTarget function to copy temporary PVarAddress into target
}
// -------------------------------- Element ------------------------------------------

SA::Element::Element()
{}
SA::Element::~Element()
{}
void SA::Element::SetValue(int value)
{
	for(int n = 0; n < pVarLinkArray.size(); n++) {
		pVarLinkArray[n].SetValue(value);
	}
}
// PVarLink insert and add
void SA::Element::AddLink(const PVarLink& link)
{
	pVarLinkArray.push_back(link);
}
void SA::Element::InsertLink(int index, const PVarLink& link)
{
	// insert at index
	PVarLinkArray::iterator it = pVarLinkArray.begin() + index + 1;
	pVarLinkArray.insert(it,link);
}
void SA::Element::PrependLink(int index, const PVarLink& link)
{
	// insert before index
	PVarLinkArray::iterator it = pVarLinkArray.begin() + index;
	pVarLinkArray.insert(it,link);
}

// save and load
void SA::Element::Save(wxDataOutputStream& stream)
{
	stream << name; // save the element's name
	stream << pVarLinkArray.size(); // save the size of the PVarLinkArray

	for (int n = 0; n < pVarLinkArray.size(); n++) {
		pVarLinkArray[n].Save(stream); // tell each PVar in the array to save itself!
	}
}

void SA::Element::Load(wxDataInputStream& stream)
{
	pVarLinkArray.clear(); // re-initialise the PVarLinkArray
	int numberOfPVarLinks; // temp variable for number of PVars in this element

	stream >> name; // load name
	stream >> numberOfPVarLinks; // load number of PVars into temp variable.

	for (int n = 0; n < numberOfPVarLinks; n++) {
		PVarLink tempPVarLink; // make temp PVarLink
		tempPVarLink.Load(stream); // load its contents from disk
		pVarLinkArray.push_back(tempPVarLink); // push it into pVarLinkArray
	}

}

// -------------------------------- Collective ---------------------------------------
SA::Collective::Collective()
{
	// AddElement(); // a single basic element
}
SA::Collective::~Collective()
{
}

void SA::Collective::SetValue(int value)
{
	for(int n = 0; n < elementArray.size(); n++) {
		elementArray[n].SetValue(value);
	}
}

void SA::Collective::Display()
{

	wxString str;
	for(int el = 0; el < elementArray.size(); el++) {
		str.Append(wxString::Format(_T("Element %d\n"),el));

		Element& element = elementArray[el];
		for(int n = 0; n < element.GetNumLinks(); n++) {
			PVarLink& link = element[n];
			PVarAddress pVarAddress = link.GetTarget();
			PVar& pVar = link.GetPVar();
			str.Append(_T(" |- ( "));
			str.Append(pVarAddress.GetString());
			str.Append(_T(" ) - "));
			str.Append(pVar.GetName());
			str.Append(_T("\n"));
		}
	}
	wxMessageBox(str,_T("Collective"));

}

void SA::Collective::AddElement(const Element& element)
{
	// add element at end
	elementArray.push_back(element);
};
void SA::Collective::InsertElement(int index, const Element& element)
{
	// insert at index
	ElementArray::iterator it = elementArray.begin() + index + 1;
	elementArray.insert(it,element);

};
void SA::Collective::PrependElement(int index, const Element& element)
{
	// insert before element at index
	ElementArray::iterator it = elementArray.begin() + index;
	elementArray.insert(it,element);
};

void SA::Collective::RemoveElement(int index)
{
	ElementArray::iterator it = elementArray.begin() + index;
	elementArray.erase(it);
}

void SA::Collective::Save(wxDataOutputStream& stream)
{
	stream << name; // save collective's name
	stream << elementArray.size(); // save number of elements in array

	for (int n = 0; n < elementArray.size(); n++) {
		elementArray[n].Save(stream); // tell each element to save itself.
	}
}

void SA::Collective::Load(wxDataInputStream& stream)
{
	elementArray.clear(); // re-initialise the element array
	int numberOfElements; // temp storage for number of elements...

	stream >> name; // load collective's name
	stream >> numberOfElements; // get number of elements in array

	for (int n = 0; n < numberOfElements; n++) {
		Element tempElement; // make a temporary collective element
		tempElement.Load(stream); // load contents of that element from disk
		elementArray.push_back(tempElement); // push loaded element into elementArray.
	}

}

// --------------------------------- Collective Cursor Edit ----------------------------
SA::CollectiveCursorEdit::CollectiveCursorEdit(Collective* _collective)
{
	// construct passing collective to be edited
	collective = _collective;
	SetLast(); // move to last element;
}

SA::Collective* SA::CollectiveCursorEdit::GetCollective()
{
	return collective;
}

SA::Element& SA::CollectiveCursorEdit::GetCurrentElement() // change to & in implementation!
{
	if (cursorIndex == -1) // if we are asking (erroneously) for the element at index -1...
	{
		/* TODO (James#5#): Un-caught exception!
		                    We should never ask for the element at index -1, 
		                    because this doesn't exist (it is the head). */
		throw Exception(); // we don't need this actually, as long as we only ask for a +ve index!
	} else // if the cursor position is a valid element...
	{
		return (*collective)[cursorIndex]; // ...return a reference to it
	}
}

int SA::CollectiveCursorEdit::GetCursorPosition()
{
	// get the cursor position -1 is head element, 0 is first real element
	return cursorIndex;
}

void SA::CollectiveCursorEdit::SetCursorPosition(int newCursorIndex)
{
	if (newCursorIndex < -1) // if we tried to set the cursor to before the head 'element'...
	{
		cursorIndex = -1; // ...set cursor to head
	} else if (newCursorIndex > collective->GetNumElements()-1) // if we tried to set it to a non-existent element
	{
		cursorIndex = collective->GetNumElements()-1; // set it to the last existing element in the collective
	} else {
		cursorIndex = newCursorIndex; // is this 100% safe? Do we want to check that this element exists?
	}
}

void SA::CollectiveCursorEdit::AddLink(const PVarLink& link)
{
	// adds a link at the current location

	// check for head
	if(cursorIndex == -1) {
		Insert();
	}
	(*collective)[cursorIndex].AddLink(link);
}

void SA::CollectiveCursorEdit::ClearLinks()
{
	wxMessageBox(_T("CollectiveCursorEdit::ClearLinks() - Not implemented yet!"));
}

void SA::CollectiveCursorEdit::Next()
{
	// moves to the next element
	int numElements = collective->GetNumElements();
	++cursorIndex;
	if(cursorIndex >= numElements) {
		cursorIndex = numElements-1;
	}
}
void SA::CollectiveCursorEdit::Previous()
{
	// moves to the previous element
	--cursorIndex;
	if(cursorIndex < -1) {
		cursorIndex = -1;
	}
}
void SA::CollectiveCursorEdit::Insert(const Element& element)
{
	// insert element after current element // cursor is set to the new element
	collective->PrependElement(cursorIndex + 1, element);
	cursorIndex++;
}

void SA::CollectiveCursorEdit::ShiftLeft()
{
	if (cursorIndex >= 1) // shift left cannot apply to the first element or head!
	{
		Element elementToBeShifted = GetCurrentElement(); // make a copy of the current element
		Remove(); // remove it
		Previous(); // shift the cursor back...
		Insert(elementToBeShifted);
	}
}

void SA::CollectiveCursorEdit::ShiftRight()
{
	if (cursorIndex > -1 && cursorIndex < collective->GetNumElements()-1) // shift right cannot apply to last element or head!
	{
		Element elementToBeShifted = GetCurrentElement(); // make a copy of the current element
		Remove(); // remove it
		Next(); // shift cursor forwards
		Insert(elementToBeShifted);
	}
}

/// Removes the current element as indicated by the cursor position
/// Cursor is then set to the previous element or head
void SA::CollectiveCursorEdit::Remove()
{
	if (cursorIndex > -1) // as long as we are not at the collective head...
	{
		collective->RemoveElement(cursorIndex); // ...remove element from collective...
		Previous(); // ...and call own function to decrement the cursor
	}
	// Do we need a check to see if the cursor is BEYOND the last valid element???
}

void SA::CollectiveCursorEdit::SetHead()
{
	// move to the head element // move to element -1
	cursorIndex = -1;
}
void SA::CollectiveCursorEdit::SetFirst()
{
	// move to the first element
	cursorIndex = 0;
}
void SA::CollectiveCursorEdit::SetLast()
{
	// move to the last element
	cursorIndex = collective->GetNumElements() - 1;
}
