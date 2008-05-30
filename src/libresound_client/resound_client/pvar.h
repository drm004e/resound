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

#ifndef SA_PVAR_H
#define SA_PVAR_H

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/version.hpp>

#include <boost/serialization/export.hpp>

namespace Resound
{
/// a name storage type
typedef std::string EntityName;


class Entity{
public:
	Entity(){};
	Entity(const EntityName& name); ///< public constructor, requires a name
	virtual ~Entity(); /// destructor
	virtual void set_name(const EntityName& name);
	virtual const EntityName& get_name();	///< reutrn the entity name
private:
	EntityName m_name; ///< the name of this entity

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_name); //
    }
};

/// Parameters are targets for automation and control
/// Parameters use integer arithmetic to eliminate rounding errors
class Parameter
{
public:
	Parameter() : m_value(0), m_lockedValue(0), m_isLocked(false) {};
	Parameter(const EntityName& name);
	virtual ~Parameter();
	//values

	virtual int get_value(); ///< obtain the value
	virtual void set_value(int oldVal, int newVal); ///< set using previous value
	virtual void set_value_direct(int newVal); ///< set directly used to initialize
	virtual void on_value_changed(); ///<a virtual function called directly by SetValue and SetValue Direct
	virtual void lock(int lockedValue); ///< locks the value - the real value will still be changed but GetValue will return the locked value
	virtual void unlock(); ///< get value will return the real value;
	bool is_locked() const {return m_isLocked;}///<return the lock status of the parameter

	void set_name(const EntityName& name) {};
	const EntityName& get_name(){return m_name;};

	virtual void dummy()=0;
	
private:
	std::string m_name;
	int m_value;
	int m_lockedValue;
	bool m_isLocked;

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_NVP(m_name);
        	//ar & BOOST_SERIALIZATION_NVP(m_value); // this doesnt want saving
		ar & BOOST_SERIALIZATION_NVP(m_lockedValue);
		ar & BOOST_SERIALIZATION_NVP(m_isLocked);
    }
};
}
BOOST_IS_ABSTRACT(Resound::Parameter)

namespace Resound{
class BasicParameter : public Parameter{
public:
	BasicParameter(){};
	BasicParameter(const EntityName& name) : Parameter(name){}
	virtual void dummy(){}
private:
	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Parameter);
    }
};
}

namespace Resound{
typedef boost::shared_ptr<Parameter> ParameterPtr;
typedef std::vector<ParameterPtr> ParameterArray;

/// Parameters are stored in various locations - behaviours, sub systems etc
/// a Parameter address is used to get hold of a Parameter
class ParameterAddress
{

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

private:
	std::string m_address; ///< the url/osc style address

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_address);
    }
};

/// exception thrown when parameter addresses are invalid
class ParameterAddressException : public std::exception{
private:
	std::string m_addr;
public:
	ParameterAddressException(const ParameterAddress& op) : m_addr(op.get_address()){}
	virtual const char* what() const throw(){ return m_addr.c_str(); }
	~ParameterAddressException() throw() {}
};

/// A class for mapping Parameter addresses to real Parameters
/// the subsystem also deals with Parameter -> real world variable translation
/// typically this is inherited from.
/// eg. the audio matrix is a ParameterNamespace
class ParameterNamespace : public Entity
{
public:
	ParameterNamespace(){}
	ParameterNamespace(EntityName name) : Entity(name) {}

	/// register a parameter at the sub address specified,
	/// the sub system address will be prepended to the address specified
	/// and the parameter will be registered with the global system
	void register_parameter(std::string address, ParameterPtr param);
	virtual void dummy()=0;

private:
	std::string m_name;

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Entity);
       // ar & BOOST_SERIALIZATION_NVP(m_address);
    }
};
}
BOOST_IS_ABSTRACT(Resound::ParameterNamespace)
namespace Resound {
/// smart pointer for namespaces
typedef boost::shared_ptr<ParameterNamespace> ParameterNamespacePtr;


/// map of parameter addresses
typedef std::map<ParameterAddress, ParameterPtr> ParameterAddressMap;
/// vector of parameter namespaces
typedef std::vector<ParameterNamespacePtr> ParameterNamespaceList;

/// a manager class for Parameter subsystems
/// manager will not deal with them
class ParameterNamespaceManager
{
public:
	ParameterNamespaceManager(); ///< construct
	~ParameterNamespaceManager(); ///< destruct
	/// register the existence of a parameter namespace
	void register_parameter_namespace(ParameterNamespacePtr parameterNamespace);
	/// get the number of namespaces registered
	int get_num_parameter_namespaces(){return m_parameterNamespaceList.size();}
	/// get a namespace by index
	ParameterNamespace& get_parameter_namespace(int index){return *m_parameterNamespaceList[index];}

	// parameter related
	/// return a parameter specified by a given address
	ParameterPtr get_parameter(const ParameterAddress &addr);

	/// lookup a parameter and return its value directly
	int peek_parameter_value(const ParameterAddress &addr); // TODO consider removal? now we are using smart ptr

	/// register a given parameter at the global osc/url path specified
	void register_parameter(std::string address, ParameterPtr param);

	/// remove a given parameter at the global osc/url path specified
	void remove_parameter(std::string address);

	/// cause all registered parameters to update and thus push data to server etc.
	void force_update();
private:
	ParameterNamespaceList m_parameterNamespaceList; ///< the namespace list
	ParameterAddressMap m_parameterAddressMap; ///< the global address space

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {	
	ar & BOOST_SERIALIZATION_NVP(m_parameterNamespaceList);
        ar & BOOST_SERIALIZATION_NVP(m_parameterAddressMap);
    }
};

typedef boost::shared_ptr<ParameterNamespaceManager> ParameterNamespaceManagerPtr;

/// Parameters are linked using a Parameterlink
/// the link also deals with Parameter summing and takes care of previously sent values
/// the link is cached in a smart pointer, but is set via the parameter addressing system
class ParameterLink
{
public:
	ParameterLink(); ///< default construct
	ParameterLink(const ParameterAddress &t); ///< construct from address
	ParameterLink(const ParameterLink& p); ///< copy construct
	ParameterLink& operator=(const ParameterLink& p); ///< assignment from other link, resets m_lastValue
	virtual ~ParameterLink(); ///< destruct

	/// set the target for this link
	void set_target(const ParameterAddress &addr);

	/// return the address of the target
	ParameterAddress get_target_address(){return m_targetAddress;}

	/// use the value summing system and set the value of the targeted parameter
	void set_value(int val); 

	/// get the current value of the targeted parareter
	int get_value(); 

	/// set the scaling factor
	void set_scaling_factor(float scalingFactor); 

	/// get the current scaling factor
	float get_scaling_factor(); 

	/// return a pointer to the targeted parameter
	ParameterPtr get_parameter(); // get the target pvar

	/// cause the link to reacquire a new smartptr to the original address, if the address is now null (removed) null ptr and handle parameter setting
	void reacquire();
	/// scan all parameter links in system
	static void reacquire_all();
private:
	static std::list<ParameterLink*> s_links; ///< static containing all links created 
	ParameterAddress m_targetAddress; /// the global address to the parameter
	int m_lastValue; ///< the last value set
	int m_lastScaledValue; ///< the last value sent to the target Parameter
	ParameterPtr m_targetPtr; ///< the actual targeted parameter
	float m_scalingFactor; ///< the scaling factor applied
	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_targetAddress);
		//ar & BOOST_SERIALIZATION_NVP(m_lastValue);
		//ar & BOOST_SERIALIZATION_NVP(m_lastScaledValue);
		ar & BOOST_SERIALIZATION_NVP(m_scalingFactor);
		ar & BOOST_SERIALIZATION_NVP(m_targetPtr);
    }
};

/// elements are the sub groups of collectives
/// they contain an array of pvar links, with unique targets within the element
class Element
{
public:
	Element();
	virtual ~Element();

	// set the value of all pvar targets in this element
	void set_value(int value);

	// collective name
	std::string get_name(){return m_name;}
	void set_name(std::string name){m_name = name;}

	// ParameterLink insert and add
	void add_link(const ParameterLink& link = ParameterLink());
	void insert_link(int index, const ParameterLink& link = ParameterLink());
	void prepend_link(int index, const ParameterLink& link = ParameterLink());

	int get_num_links(){return m_parameterLinkArray.size();}
	// overloaded indexing
	ParameterLink& operator[](int index){return m_parameterLinkArray[index];}

private:
	std::string m_name;
	typedef std::vector<ParameterLink> ParameterLinkArray;
	ParameterLinkArray m_parameterLinkArray;

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_name);
		ar & BOOST_SERIALIZATION_NVP(m_parameterLinkArray);
    }
};

/// collectives form the basis of grouped assignment
/// collectives are simply a structured set of links
/// on which behaviours operate
class Collective
{

public:
	Collective();
	virtual ~Collective();

	// set the value for every link in every element
	void set_value(int value);

	// collective name
	std::string get_name(){return m_name;}
	void set_name(std::string name){m_name = name;}

	// gui helpers
	void display();

	// element insertion
	void add_element(const Element& element = Element()); // add element at end
	void insert_element(int index, const Element& element = Element()); // insert at index
	void prepend_element(int index, const Element& element = Element()); // insert before element at index
	void remove_element(int index); // remove element at index
	int get_num_elements(){return m_elementArray.size();}
	// overloaded indexing
	Element& operator[](int index){return m_elementArray[index];}

private:
	std::string m_name;
	// the array of elements
	typedef std::vector<Element> ElementArray;
	ElementArray m_elementArray;

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(m_name);
		ar & BOOST_SERIALIZATION_NVP(m_elementArray);
    }

};

/// collectivecursoredit is a utility class for manipulating a collective
/// using a current position cursor
class CollectiveCursorEdit
{
private:
	Collective* m_collective; // the collective being edited
	int m_cursorIndex;
public:
	CollectiveCursorEdit(Collective* collective);// construct passing collective to be edited

	Collective* get_collective();  // return pointer to the collective being edited
	Element& get_current_element(); // return reference to the element indicated by current cursor position
	int get_cursor_position(); // get the cursor position -1 is head element, 0 is first real element
	void set_cursor_position(int newCursorIndex); // set the cursor position

	void add_link(const ParameterLink& link = ParameterLink()); // adds a link at the current location
	void clear_links(); // clear all of the ParameterLinks at the current cursor location but keep the element itself
	void next(); // moves to the next element
	void previous(); // moves to the previous element
	void insert(const Element& element = Element()); // insert element after current element // cursor is set to the new element
	void shift_left(); // not implemented yet
	void shift_right(); // not implemented yet
	void remove(); // removes the current element
	/* TODO (James#1#): At present Remove() decrements cursor ; it would be better if it
	                    didn't but this would mean checking if its pointing to
	                    an element beyond the end of the collective (i.e.
	                    if user deleted the last element) */

	void set_head(); // move to the head element // move to element -1
	void set_first(); // move to the last element
	void set_last(); // move to the last element
};




}

#endif // SA_PVAR_H
