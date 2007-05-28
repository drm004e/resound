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

#ifndef SA_PVAR_H
#define SA_PVAR_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

namespace SA
{
// Parameters are targets for automation and control
// Parameters use integer arithmetic to eliminate rounding errors
class Parameter
{
public:
	Parameter();
	virtual ~Parameter();

	//values
	virtual int get_value(); ///< obtain the value
	virtual void set_value(int oldVal, int newVal); ///< set using previous value
	virtual void set_value_direct(int newVal); ///< set directly used to initialize
	virtual void on_value_changed(); ///<a virtual function called directly by SetValue and SetValue Direct
	virtual void lock(int lockedValue); ///< locks the value - the real value will still be changed but GetValue will return the locked value
	virtual void unlock(); ///< get value will return the real value;

	//names
	void set_name(std::string name); // name this Pvar
	std::string get_name();

private:
	std::string m_name;
	int m_value;
	int m_lockedValue;
	bool m_isLocked;
};

typedef boost::shared_ptr<Parameter> ParameterPtr;

// Parameters are stored in various locations - behaviours, sub systems etc
// a Parameter address is used to get hold of a Parameter
// the ParameterManager deals with this
class ParameterAddress
{
private:
	std::string m_address; ///< the url/osc style address
public:
	///construct
	ParameterAddress() : m_address("default"){};
	/// construct from an address string
	ParameterAddress(std::string address) : m_address(address){}
	/// set the address
	void set_address(std::string address) {m_address=address;}
	/// return the address
	std::string get_address() const {return m_address;}
	/// get sub address
	std::string get_address_part(int index) const{}; // incomplete
	/// get number of address parts
	int get_num_address_parts() const {return 0;}; // incomplete

	bool operator < (const ParameterAddress& op) const { return m_address < op.m_address;}
	bool operator == (const ParameterAddress& op) const { return m_address == op.m_address;}
};

class ParameterAddressException : public std::exception{
private:
	std::string m_addr;
public:
	ParameterAddressException(const ParameterAddress& op) : m_addr(op.get_address()){}
	virtual const char* what() const throw(){ return m_addr.c_str(); }
	~ParameterAddressException() throw() {}
};

// Parameter subsystem - A system for mapping Parameter addresses to real Parameters
// the subsystem also deals with Parameter -> real world variable translation
// typically this is inherited from.
// the audio matrix is a Parameter sub system
class ParameterNamespace
{
private:
	std::string m_name; ///< subsystem name forms the first part of any mapped addresses
	// so in the address /behaviour/mexican1/freq. /behaviour is the subsystem 
public:
	ParameterNamespace(std::string name) : m_name(name) {}
	/// get the name of this sub system
	std::string get_name(){return m_name;}

	/// register a parameter at the sub address specified,
	/// the sub system address will be prepended to the address specified
	/// and the parameter will be registered with the global system
	void register_parameter(std::string address, ParameterPtr param);
};


typedef std::map<ParameterAddress, ParameterPtr> ParameterAddressMap;
typedef std::vector<ParameterNamespace*> ParameterNamespaceList;
// a manager class for Parameter subsystems
// subsystems must be manually delete by the creator
// manager will not deal with them
class ParameterNamespaceManager
{
private:
	// singleton
	ParameterNamespaceManager();
	~ParameterNamespaceManager();
	static ParameterNamespaceManager* singleton;///<the singleton object ptr
	ParameterAddressMap m_parameterAddressMap; ///< the global address space
public:

	static ParameterNamespaceManager& GetSingleton();
	static void Destroy();

	void RegisterParameterNamespace(ParameterNamespace* subSystem);
	
	int GetNumSubSystems(){return subSystemList.size();}
	ParameterNamespace& GetSubSystem(int index){return *subSystemList[index];}

	// parameter related
	ParameterPtr GetParameter(const ParameterAddress &addr);
	int PeekParameterValue(const ParameterAddress &addr);

	/// register a given parameter at the global path specified
	void register_parameter(std::string address, ParameterPtr param);
private:
	ParameterNamespaceList subSystemList;
};

// Parameters are linked using a Parameterlink
// the link is indirect - ie not with a pointer -
// this attempts to avoid possible bugs with invalid pointers due to object creation/deletion
// the link also deals with Parameter summing and takes care of previously sent values
class ParameterLink
{
public:
	ParameterLink();
	ParameterLink(const ParameterAddress &t);
	ParameterLink(const ParameterLink& p); // copy construct
	ParameterLink& operator=(const ParameterLink& p); // assignment
	virtual ~ParameterLink();

	void SetTarget(const ParameterAddress &addr); // set the target
	ParameterAddress GetTarget(){return m_targetAddress;}
	void SetValue(int val); // uses the suming system to set a new value
	int GetValue(); // get the current value involves a lookup
	ParameterPtr GetParameter(); // get the target pvar


private:
	ParameterAddress m_targetAddress; /// the global address to the parameter
	int m_lastValue; ///< the last value sent to the target Parameter
	ParameterPtr m_targetPtr; ///< the actual targeted parameter
};

// elements are the sub groups of collectives
// they contain an array of pvar links, with unique targets within the element
class Element
{
private:
	std::string name;
	typedef std::vector<ParameterLink> ParameterLinkArray;
	ParameterLinkArray pVarLinkArray;

public:
	Element();
	~Element();

	// set the value of all pvar targets in this element
	void SetValue(int value);

	// collective name
	std::string GetName()
	{
		return name;
	};
	void SetName(std::string _name)
	{
		name = _name;
	};



	// ParameterLink insert and add
	void AddLink(const ParameterLink& link = ParameterLink());
	void InsertLink(int index, const ParameterLink& link = ParameterLink());
	void PrependLink(int index, const ParameterLink& link = ParameterLink());

	int GetNumLinks()
	{
		return pVarLinkArray.size();
	};
	// overloaded indexing
	ParameterLink& operator[](int index)
	{
		return pVarLinkArray[index];
	};
};

// collectives form the basis of grouped assignment
// collectives are simply a structured set of links
// behaviours operate
class Collective
{
private:
	std::string name;
	// the array of elements
	typedef std::vector<Element> ElementArray;
	ElementArray elementArray;

public:
	Collective();
	~Collective();

	// set the value for every link in every element
	void SetValue(int value);

	// collective name
	std::string GetName()
	{
		return name;
	};
	void SetName(std::string _name)
	{
		name = _name;
	};



	// gui helpers
	void Display();

	// element insertion
	void AddElement(const Element& element = Element()); // add element at end
	void InsertElement(int index, const Element& element = Element()); // insert at index
	void PrependElement(int index, const Element& element = Element()); // insert before element at index
	void RemoveElement(int index); // remove element at index
	int GetNumElements()
	{
		return elementArray.size();
	};
	// overloaded indexing
	Element& operator[](int index)
	{
		return elementArray[index];
	};
};

// collectivecursoredit is a utility class for manipulating a collective
// using a current position cursor
class CollectiveCursorEdit
{
private:
	Collective* collective; // the collective being edited
	int cursorIndex;
public:
	CollectiveCursorEdit(Collective* _collective);// construct passing collective to be edited

	Collective* GetCollective();  // return pointer to the collective being edited
	Element& GetCurrentElement(); // return reference to the element indicated by current cursor position
	int GetCursorPosition(); // get the cursor position -1 is head element, 0 is first real element
	void SetCursorPosition(int newCursorIndex); // set the cursor position

	void AddLink(const ParameterLink& link = ParameterLink()); // adds a link at the current location
	void ClearLinks(); // clear all of the ParameterLinks at the current cursor location but keep the element itself
	void Next(); // moves to the next element
	void Previous(); // moves to the previous element
	void Insert(const Element& element = Element()); // insert element after current element // cursor is set to the new element
	void ShiftLeft(); // not implemented yet
	void ShiftRight(); // not implemented yet
	void Remove(); // removes the current element
	/* TODO (James#1#): At present Remove() decrements cursor ; it would be better if it
	                    didn't but this would mean checking if its pointing to
	                    an element beyond the end of the collective (i.e.
	                    if user deleted the last element) */

	void SetHead(); // move to the head element // move to element -1
	void SetFirst(); // move to the last element
	void SetLast(); // move to the last element
};

}

#endif // SA_PVAR_H
