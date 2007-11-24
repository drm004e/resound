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

namespace Resound{



/// dynamic objects are objects that are configured and contructed based on the XML merging system
/// dynamic objects are completely managed and form the base class for things like streams, loudspeakers and behaviours
/// dynamic objects are stored by the engine against unique ids
class DynamicObject{
	xmlpp::Node* m_node; ///< the xml node that this dynamic object is paired with
public:
	// construct
	DynamicObject();
	// construct from a node
	DynamicObject(xmlpp::Node* node);
	// destruct
	virtual ~DynamicObject();
	// attach to a node, this will cause an update from the node
	virtual void attach_to_node(xmlpp::Node* node);
	// detach from a node
	virtual void detach_from_node();
	// virtual update call, called when the object must update itself from the node
	virtual void update_from_node() = 0;	
	// get a reference to the paired node
	xmlpp::Node& get_node();
};

/// a base class for audio streams
class AudioStream : public DynamicObject{
};

/// a file stream
class AudioStreamFile : public AudioStream{
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


} // end namespace

#endif
