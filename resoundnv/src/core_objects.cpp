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

#include "core_objects.hpp"
using namespace Resound;
// --------------- AudioStream
AudioStream::AudioStream(const std::string& id, DynamicObject* parent) : 
DynamicObject(id,parent),
m_gain("gain",1.0f)
{}
void AudioStream::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="gain") m_gain.from_string(value);
}
void AudioStream::write_attributes(std::stringstream& xml){
	DynamicObject::write_attributes(xml); 
	xml << m_gain.to_string();
};
// --------------- AudioStreamFile
AudioStreamFile::AudioStreamFile(const std::string& id, DynamicObject* parent) : 
AudioStream(id,parent),
m_path("path","")
{}
void AudioStreamFile::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="path") m_path.from_string(value);
}
void AudioStreamFile::write_attributes(std::stringstream& xml){
	AudioStream::write_attributes(xml); 
	xml << m_path.to_string();
}
// --------------- AudioStreamLive
AudioStreamLive::AudioStreamLive(const std::string& id, DynamicObject* parent) : 
AudioStream(id,parent),
m_port("port","")
{}
void AudioStreamLive::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="port") m_port.from_string(value); // at this point we reroute the jack server
}
void AudioStreamLive::write_attributes(std::stringstream& xml){
	AudioStream::write_attributes(xml); 
	xml << m_port.to_string();
}
// --------------- Loudspeaker
Loudspeaker::Loudspeaker(const std::string& id, DynamicObject* parent) : 
DynamicObject(id,parent),
m_gain("gain",1.0f),
m_port("port",""),
m_x("x",0.0f),
m_y("y",0.0f),
m_z("z",0.0f),
m_az("az",0.0f),
m_el("el",0.0f)
{}
void Loudspeaker::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="port") m_port.from_string(value); // at this point we reroute the jack server
	else if(name=="gain") m_gain.from_string(value);
	else if(name=="x") m_x.from_string(value);
	else if(name=="y") m_y.from_string(value);
	else if(name=="z") m_z.from_string(value);
	else if(name=="az") m_az.from_string(value);
	else if(name=="el") m_el.from_string(value);
}
void Loudspeaker::write_attributes(std::stringstream& xml){
	DynamicObject::write_attributes(xml); 
	xml << m_gain.to_string();
	xml << m_port.to_string();
	xml << m_x.to_string();
	xml << m_y.to_string();
	xml << m_z.to_string();
	xml << m_el.to_string();
	xml << m_az.to_string();
}
//  ------------------CoherentSet
CoherentSet::CoherentSet(const std::string& id, DynamicObject* parent):
DynamicObject(id,parent),
m_gain("gain",1.0f)
{}
void CoherentSet::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="gain") m_gain.from_string(value);
}
void CoherentSet::write_attributes(std::stringstream& xml){
	DynamicObject::write_attributes(xml); 
	xml << m_gain.to_string();
}

//  ------------------CoherentAudioStreamSet
CoherentAudioStreamSet::CoherentAudioStreamSet(const std::string& id, DynamicObject* parent):
CoherentSet(id,parent)
{}
void CoherentAudioStreamSet::on_attribute_changed(const std::string& name, const std::string& value){}
void CoherentAudioStreamSet::write_attributes(std::stringstream& xml){
	CoherentSet::write_attributes(xml); 
}



//  ------------------CoherentLoudspeakerSet
CoherentLoudspeakerSet::CoherentLoudspeakerSet(const std::string& id, DynamicObject* parent):
CoherentSet(id,parent)
{}
void CoherentLoudspeakerSet::on_attribute_changed(const std::string& name, const std::string& value){}
void CoherentLoudspeakerSet::write_attributes(std::stringstream& xml){
	CoherentSet::write_attributes(xml); 
}


//  ------------------Behaviour
Behaviour::Behaviour(const std::string& id, DynamicObject* parent):
DynamicObject(id,parent)
{}
void Behaviour::on_attribute_changed(const std::string& name, const std::string& value){}
void Behaviour::write_attributes(std::stringstream& xml){
	DynamicObject::write_attributes(xml); 
}


// ---------------ResoundRoot
ResoundRoot::ResoundRoot(const std::string& id) : DynamicObject(id){
	register_factory("audiostreamfile", AudioStreamFile::factory);
	register_factory("audiostreamlive", AudioStreamLive::factory);
	register_factory("loudspeaker", Loudspeaker::factory);
	register_factory("cass", CoherentAudioStreamSet::factory);
	register_factory("cls", CoherentLoudspeakerSet::factory);
	register_factory("behaviour", Behaviour::factory);
}
