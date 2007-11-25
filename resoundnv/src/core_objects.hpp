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
public:
	AudioStream(const std::string& id, DynamicObject* parent) : DynamicObject(id,parent){}
};

/// a file stream
class AudioStreamFile : public AudioStream{
public:
	AudioStreamFile(const std::string& id, DynamicObject* parent) : AudioStream(id,parent){}
	static DynamicObjectPtr factory(const std::string& id, DynamicObject* parent){ return DynamicObjectPtr(new AudioStreamFile(id,parent));}
};

/// a live audio stream
class AudioStreamLive : public AudioStream{
};
/// loudspeaker
class Loudspeaker : public DynamicObject{
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
	ResoundRoot(const std::string& id) : DynamicObject(id){
		register_factory("audiostreamfile", AudioStreamFile::factory);
	}
};

} // end namespace

#endif
