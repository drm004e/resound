//    Resound
//    Copyright 2007 David Moore and James Mooney
//
//    This file is part of Resound.
//
//    Resound is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Resound is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Resound; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "dynamicobject.hpp"
#include <iostream>

#ifdef DEBUG
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x)
#endif

using namespace Resound;

DynamicObject::DynamicObject(){
	register_factory("DynamicObject", DynamicObject::factory);
}

DynamicObject::DynamicObject(const std::string& id, DynamicObject* parent) :
	m_id(id),
	m_parent(parent){
	register_factory("DynamicObject", DynamicObject::factory);
}
DynamicObject::~DynamicObject(){
	DEBUG_ONLY(std::cout << "~DynamicObject():" << m_id << std::endl;)
}
void DynamicObject::attach_child(DynamicObjectPtr object, const std::string& id){
	DynamicObjectMap::iterator it = m_children.find(id);
	if(it == m_children.end()) {
		object->m_id=id; // just to be sure
		m_children[id]=object;
	} else {
		throw DynamicObjectException("A child with this name already exists.");
	}
}
void DynamicObject::create_child(const std::string& objectClass, const std::string& id){
	std::cout << "NEW: class=" << objectClass << " id=" << id << "\n";
	DynamicObjectFactoryMap::iterator it = m_factories.find(objectClass);
	if(it != m_factories.end()) {
		DynamicObjectPtr ob = (it->second)(id,this);
		ob->m_classId = objectClass; // because the class name may be independant of the actual C++ class
		attach_child(ob,id);
	} else {
		throw DynamicObjectException("A factory with this name could name be found in this scope.");
	}
}

void DynamicObject::destroy_child(const std::string& id){
	std::cout << "DELETE: " << id << std::endl;
	// need to think about how this works
}

void DynamicObject::rename_child(const std::string& id, const std::string& newName){
	std::cout << "RENAME: " << id << " to " << newName << std::endl;
	// need to think about how this works
}

void DynamicObject::parse_xml(const xmlpp::Node* node){
	// when we enter here the attributes and elements in the node should relate to this object

	const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
	if(nodeElement)
	{
		// inform the inheriting class about any attributes via the atribute virtual function
		// for each attribute call the virtual function
		const xmlpp::Element::AttributeList& atList( nodeElement->get_attributes()); 
		for(xmlpp::Element::AttributeList::const_iterator it = atList.begin();
			it != atList.end();
			it++){
			const xmlpp::Attribute* attribute = *it;
			Glib::ustring attributeName = attribute->get_name();
			Glib::ustring attributeValue = attribute->get_value();
			this->on_attribute_changed(attributeName, attributeValue);
		}
		
		//Recurse through child nodes:
		xmlpp::Node::NodeList list = node->get_children();
		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
			if(const xmlpp::Element* el = dynamic_cast<const xmlpp::Element*>(*iter)){
				Glib::ustring id = el->get_name();
				if(id=="new"){
					// get id and class attributes from the node
					// attempt to construct
					std::string newId=el->get_attribute("id")->get_value();
					std::string newClass=el->get_attribute("class")->get_value();
					create_child(newClass,newId);
					// now forward the node to the newly created object
					get_object_by_id(newId)->parse_xml(el);
				} else if (id=="delete"){
					// get id from attribute
					std::string delId=el->get_attribute("id")->get_value();
					// lookup object, delete it... take care for other threads here and refferencing pointers
					destroy_child(delId);
				} else if (id=="rename"){
					// get id and newname from attributes
					std::string rnId=el->get_attribute("id")->get_value();
					std::string rnNewId=el->get_attribute("newid")->get_value();
					// rename the object but take care with references
					rename_child(rnId,rnNewId);
				} else {
					// its a direct reference to a child object
					get_object_by_id(id)->parse_xml(el);
				}
			}
		}
	}
	
}
DynamicObjectPtr DynamicObject::get_object_by_id(const std::string& id){
	// look up the first section of the id and pass the remainder of the id recursively until we get the object
	int n = id.find_first_of('.');
	std::string childId;
	childId=id.substr(0,n);
	DEBUG_ONLY(std::cout << "lookup " << childId;)
	DynamicObjectMap::iterator it = m_children.find(childId);
	if(it != m_children.end()){
		
		// recurse here
		if(n==std::string::npos){
			DEBUG_ONLY(std::cout << " OK\n";)
			return it->second;
		} else {
			DEBUG_ONLY(std::cout << " recursing, ";)
			return it->second->get_object_by_id(id.substr(n+1));
		}
		
	} else {
		DEBUG_ONLY(std::cout << " FAIL\n";)
		// probably throw here
		throw DynamicObjectException("An object with this id could not be found in this scope");
	}
}

void  DynamicObject::get_xml_string(std::stringstream& xml,int indentation){
	for(int i = 0; i < indentation; ++i)
    		xml << " ";
	xml << "<" << m_id;
	this->write_attributes(xml);
	xml << ">\n";
	// foreach child
	DynamicObjectMap::iterator it = m_children.begin();
	for(;it!= m_children.end(); it++){
		it->second->get_xml_string(xml,indentation+5);
	}
	for(unsigned int i = 0; i < indentation; ++i)
    		xml << " ";
	xml << "</" << m_id << ">\n";
}

void DynamicObject::register_factory(const std::string& classId, DynamicObjectFactory factory){
	assert(factory);
	DynamicObjectFactoryMap::iterator it = m_factories.find(classId);
	if(it != m_factories.end()) { throw DynamicObjectException("A factory with this classname already exists."); }
	m_factories[classId] = factory;
}

/*static*/DynamicObjectPtr DynamicObject::factory(const std::string& id, DynamicObject* parent){
	return DynamicObjectPtr(new DynamicObject(id,parent));
}

void DynamicObject::write_attributes(std::stringstream& xml){
	// write the class
	xml << MAKE_ATTRIBUTE_STRING("class",m_classId);
}