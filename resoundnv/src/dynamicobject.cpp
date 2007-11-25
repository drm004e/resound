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

using namespace Resound;

DynamicObject::DynamicObject(){}

DynamicObject::DynamicObject(const char* id) :
	m_id(id){
}
DynamicObject::~DynamicObject(){}

void DynamicObject::create_child(const std::string& objectClass, const std::string& id){
	std::cout << "new class=" << objectClass << " id=" << id << "\n";
}

void DynamicObject::destroy_child(const std::string& id){
}

void DynamicObject::rename_child(const std::string& id, const std::string& newName){
}

void DynamicObject::parse_xml(const xmlpp::Node* node){
	// when we enter here the attributes and elements in the node should relate to this object

	// inform the inheriting class about any attributes via the atribute virtual function
	// for each attribute call the virtual function
	//for(;;){
		//this->set_attribute(attributeName, attributeValue);
	//}
  	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  	const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);
 	Glib::ustring nodename = node->get_name();
	if(nodeText || nodeComment || nodename.empty()){ //ignore the indenting and comments
		return;
	}
	const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
	if(!nodeContent)
	{
		//Recurse through child nodes:
		xmlpp::Node::NodeList list = node->get_children();
		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
			if(const xmlpp::Element* el = dynamic_cast<const xmlpp::Element*>(*iter)){
				Glib::ustring id = el->get_name();
				if(id=="new"){
					// get id and class attributes from the node
					// attempt to construct
					create_child(el->get_attribute("class")->get_value(),el->get_attribute("id")->get_value());
				} else if (id=="delete"){
					// get id from attribute
					// lookup object, delete it... take care for other threads here and refferencing pointers
					std::cout << "delete object\n";
				} else if (id=="rename"){
					// get id and newname from attributes
					// rename the object but take care with references
					std::cout << "rename object\n";
				} else {
					// its a direct reference to a child object
					try{
						get_object_by_id(id)->parse_xml(el);
					} catch(...){
						
					}
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
	std::cout << "sub id lookup " << childId;
	DynamicObjectMap::iterator it = m_children.find(childId);
	if(it != m_children.end()){
		std::cout << " OK recursing\n";
		// recurse here
		return it->second->get_object_by_id(id.substr(n));
	} else {
		std::cout << " FAIL not found\n";
		// probably throw here
		throw std::exception();
	}
}

void  DynamicObject::get_xml_string(std::stringstream& xml){
	xml << "<" << m_id << ">\n";
	// foreach child
	DynamicObjectMap::iterator it = m_children.begin();
	for(;it!= m_children.end(); it++){
		it->second->get_xml_string(xml);
	}
	xml << "</" << m_id << ">\n";
}