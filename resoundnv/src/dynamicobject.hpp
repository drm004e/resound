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

#include <string>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

namespace Resound{

class DynamicObject;

/// a shared pointer to a dynamic object
typedef boost::shared_ptr<DynamicObject> DynamicObjectPtr;

/// a factory function pointer
typedef DynamicObjectPtr (*DynamicObjectFactory)(const std::string& id);

/// a map of factory ptrs again class ids
typedef std::map<std::string, DynamicObjectFactory> DynamicObjectFactoryMap;

/// a map of ids against object ptrs
typedef std::map<std::string, DynamicObjectPtr> DynamicObjectMap;

class DynamicObjectUnknownClassException : public std::exception{
	public: const char* what(){return "An object class was specified that doesnt exist in the current scope";}
};
class DynamicObjectDuplicateClassException : public std::exception{
	public: const char* what(){return "A factory name with this name already exists";}
};
class DynamicObjectDuplicateObjectException : public std::exception{
	public: const char* what(){return "An object with this name already exists";}
};
class DynamicObjectBadIdException : public std::exception
{
	public: const char* what(){return "A DynamicObject was not found at this scope";}
};

/// dynamic objects are objects that are configured and contructed from a libxml++ parser
/// dynamic objects are stored by the engine against unique ids
/// the dynamic object system acts as a factory for nested objects
class DynamicObject{
	DynamicObjectFactoryMap m_factories;
	DynamicObjectMap m_children;
	DynamicObjectPtr m_parent;
	std::string m_id; ///< the id of the object
	std::string m_classId; /// the rtti class string as it appears in xml
public:
	/// construct
	DynamicObject();
	/// construct
	DynamicObject(const std::string& id);
	/// destruct
	virtual ~DynamicObject();

	/// create a child node, in response to the 'new' xml command
	/// pass in object class id to look up in factory and id
	virtual void create_child(const std::string& objectClass, const std::string& id);

	/// destroys a child node by name
	virtual void destroy_child(const std::string& id);

	/// rename a child node
	virtual void rename_child(const std::string& id, const std::string& newName);

	/// parse node using the various DynamicObject rules i.e. new delete and rename
	/// the node will have come from an external document so it should be parsed only not stored
	/// recursive traversal per sub node
	virtual void parse_xml(const xmlpp::Node* node);

	/// this is a recursive call that parses a sectional element name and returns an object or throws if not found
	DynamicObjectPtr get_object_by_id(const std::string& id);

	/// recursive stream output, writes the xml for self and all children outputing attributes
	void get_xml_string(std::stringstream& xml,int indentation=0);

	/// register factory, registers a factory function. typically called by a constructor
	void register_factory(const std::string& classId, DynamicObjectFactory factory);

	/// a static factory for empty DynamicObjects
	static DynamicObjectPtr factory(const std::string& id);
};


}