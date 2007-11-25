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

#ifndef __CORE_OBJECTS__
#define __CORE_OBJECTS__

#include "dynamicobject.hpp"
namespace Resound{


/// a base class for audio streams
class AudioStream : public DynamicObject{
protected:
	float m_gain;
public:
	AudioStream(const std::string& id, DynamicObject* parent) : DynamicObject(id,parent){}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// a file stream
class AudioStreamFile : public AudioStream{
protected:
	std::string m_path;
public:
	AudioStreamFile(const std::string& id, DynamicObject* parent) : AudioStream(id,parent){}
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new AudioStreamFile(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// a live audio stream
class AudioStreamLive : public AudioStream{
protected:
	std::string m_port;
public:
	AudioStreamLive(const std::string& id, DynamicObject* parent) : AudioStream(id,parent){}
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new AudioStreamLive(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};
/// loudspeaker
class Loudspeaker : public DynamicObject{
protected:
	float m_gain;
	std::string m_port;
	float m_x;
	float m_y;
	float m_z;
	float m_az;
	float m_el;
public:
	Loudspeaker(const std::string& id, DynamicObject* parent) : DynamicObject(id,parent){}
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new Loudspeaker(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// a base class for coherent sets
class CoherentSet : public DynamicObject{
};
/// cass
class CoherentAudioStreamSet : public CoherentSet{
};
/// cls
class CoherentLoudspeakerSet : public CoherentSet{
};
/// behaviour
class Behaviour: public DynamicObject{
};

/// the root level object
class ResoundRoot : public DynamicObject{
public:
	ResoundRoot(const std::string& id);
};

} // end namespace

#endif
