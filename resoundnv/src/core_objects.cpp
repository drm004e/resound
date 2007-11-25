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
void AudioStream::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="gain") m_gain = atof(value.c_str());
}
void AudioStream::write_attributes(std::stringstream& xml){
	DynamicObject::write_attributes(xml); 
	xml << MAKE_ATTRIBUTE_STRING("gain",m_gain);
};
// --------------- AudioStreamFile
void AudioStreamFile::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="path") m_path = value;
}
void AudioStreamFile::write_attributes(std::stringstream& xml){
	AudioStream::write_attributes(xml); 
	xml << MAKE_ATTRIBUTE_STRING("path",m_path);
}
// --------------- AudioStreamLive
void AudioStreamLive::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="port") m_port = value; // at this point we reroute the jack server
}
void AudioStreamLive::write_attributes(std::stringstream& xml){
	AudioStream::write_attributes(xml); 
	xml << MAKE_ATTRIBUTE_STRING("port",m_port);
}
// --------------- Loudspeaker
void Loudspeaker::on_attribute_changed(const std::string& name, const std::string& value){
	if(name=="port") m_port = value; // at this point we reroute the jack server
	else if(name=="gain") m_gain = atof(value.c_str());
	else if(name=="x") m_x = atof(value.c_str());
	else if(name=="y") m_y = atof(value.c_str());
	else if(name=="z") m_z = atof(value.c_str());
	else if(name=="az") m_az = atof(value.c_str());
	else if(name=="el") m_el = atof(value.c_str());
}
void Loudspeaker::write_attributes(std::stringstream& xml){
	DynamicObject::write_attributes(xml); 
	xml << MAKE_ATTRIBUTE_STRING("gain",m_gain);
	xml << MAKE_ATTRIBUTE_STRING("port",m_port);
	xml << MAKE_ATTRIBUTE_STRING("x",m_x);
	xml << MAKE_ATTRIBUTE_STRING("y",m_y);
	xml << MAKE_ATTRIBUTE_STRING("z",m_z);
	xml << MAKE_ATTRIBUTE_STRING("az",m_y);
	xml << MAKE_ATTRIBUTE_STRING("el",m_z);
}
// ---------------ResoundRoot
ResoundRoot::ResoundRoot(const std::string& id) : DynamicObject(id){
	register_factory("audiostreamfile", AudioStreamFile::factory);
	register_factory("audiostreamlive", AudioStreamLive::factory);
	register_factory("loudspeaker", Loudspeaker::factory);
}