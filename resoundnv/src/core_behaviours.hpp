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

#ifndef __CORE_BEHAVIOURS__
#define __CORE_BEHAVIOURS__

#include "core_objects.hpp"
namespace Resound{

/// a base class for audio streams
class BAttenuator : public Behaviour{
protected:
	
public:
	BAttenuator(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new BAttenuator(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

} //namespace

#endif
