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
	AttributeHelper<float> m_gain;
public:
	AudioStream(const std::string& id, DynamicObject* parent);
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// a file stream
class AudioStreamFile : public AudioStream{
protected:
	AttributeHelper<std::string> m_path;
public:
	AudioStreamFile(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new AudioStreamFile(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// a live audio stream
class AudioStreamLive : public AudioStream{
protected:
	AttributeHelper<std::string> m_port;
public:
	AudioStreamLive(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new AudioStreamLive(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};
/// loudspeaker
class Loudspeaker : public DynamicObject{
protected:
	AttributeHelper<float> m_gain;
	AttributeHelper<std::string> m_port;
	AttributeHelper<float> m_x;
	AttributeHelper<float> m_y;
	AttributeHelper<float> m_z;
	AttributeHelper<float> m_az;
	AttributeHelper<float> m_el;
public:
	Loudspeaker(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new Loudspeaker(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// a base class for coherent sets
class CoherentSet : public DynamicObject{
protected:
	AttributeHelper<float> m_gain;
public:
	CoherentSet(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new CoherentSet(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};
/// cass
class CoherentAudioStreamSet : public CoherentSet{
public:
	CoherentAudioStreamSet(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new CoherentAudioStreamSet(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};
/// cls
class CoherentLoudspeakerSet : public CoherentSet{
public:
	CoherentLoudspeakerSet(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new CoherentLoudspeakerSet(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};
/// behaviour
class Behaviour: public DynamicObject{
public:
	Behaviour(const std::string& id, DynamicObject* parent);
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new Behaviour(id,parent));}
	/// overload for attribute handling
	virtual void on_attribute_changed(const std::string& name, const std::string& value);
	/// overload this for attribute printing
	virtual void write_attributes(std::stringstream& xml);
};

/// the root level object
class ResoundRoot : public DynamicObject{
public:
	ResoundRoot(const std::string& id);
};

} // end namespace

#endif
