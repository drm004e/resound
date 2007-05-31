//   
//   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
//   
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//   MA 02111-1307 USA
//   

#ifndef __COHERENCY_H
#define __COHERENCY_H

#include "pvar.h"
#include <set>
#include <map>
#include <string>

namespace Resound{

typedef std::string Metatag;
typedef std::set<Metatag> MetatagSet;

typedef std::string AudioChannelId; ///< will be a port id in jack or an int?

class CoherencyObject{
public:
	void add_tag(Metatag tag){ m_metatags.insert(tag); }
	void set_name(const std::string& name){m_name=name;}
	std::string get_name(){return m_name;}
	/// search for a metatag, return true if found
	bool contains_metatag(Metatag tag);
protected:
	std::string m_name;
	MetatagSet m_metatags;
};

/// an alias for an audio channel
class AudioChannelAlias : public CoherencyObject{
public:
	AudioChannelId get_audio_channel_id(){return m_audioChannelId;}
	void set_audio_channel_id(AudioChannelId channelId){m_audioChannelId = channelId;}
private:
	AudioChannelId m_audioChannelId;
};

class AudioChannelAliasRef : public CoherencyObject{
public:
private:
};

/// a class representing a physical loudpeaker
class Loudspeaker : public AudioChannelAlias{
public:
	Loudspeaker(){};
	//Loudspeaker(std::string name, OutputChannelId outputChannelId);
private:
	// float m_x,m_y,m_z; ///< position // not yet done , will be vector based
};
typedef std::map<std::string, Loudspeaker> LoudspeakerMap;


/// a class representing a reference to a Loudspeaker and associated meta data
class LoudspeakerRef : public AudioChannelAliasRef{
public:
private:

};
typedef std::map<std::string,LoudspeakerRef> LoudspeakerRefMap;

/// a class representing a single monophonic physical audio input channel
class AudioSource : public AudioChannelAlias{
public:
private:
};
typedef std::map<std::string,AudioSource> AudioSourceMap;

/// a class representing a reference to an AudioSource and associated meta data
class AudioSourceRef : public AudioChannelAliasRef{
public:
private:

};
typedef std::map<std::string,AudioSourceRef> AudioSourceRefMap;

/// a coherent loudpspeaker set
class CLS : public CoherencyObject{
public:
private:

};
typedef std::map<std::string,CLS> CLSMap;

/// a coherent audio source set
class CASS : public CoherencyObject{
public:
private:
};
typedef std::map<std::string,CASS> CASSMap;

/// a management class for the above classes
/// provides functionality to produce collectives from metatags
/// maintains lists of coherency objects etc
class CoherencyManager {
public:
	/// adding loudspeakers
	void add(const Loudspeaker& ob);
	/// adding sources
	void add(const AudioSource& ob);
	/// adding coherent loudspeaker sets
	void add(const CLS& ob);
	/// adding coherent source sets
	void add(const CASS& ob);

	/// load a config file and populate all the coherency manager data from it
	void load_configuration(const std::string& path);

	/// map cass to cls and return a collective containing that mapping
	/// the first meta tag matches will be mapped, others will be ignored
	Collective map_cass_to_cls_first_match_only(std::string cassName, CLS clsName);

	/// map cass to cls and return a collective containing that mapping
	/// all matches will be mapped
	Collective map_cass_to_cls_match_all(std::string cassName, CLS clsName);

	/// priority to CLS CASS metatag mapping is performed starting 

private:
	LoudspeakerMap m_loudspeakers;
	AudioSourceMap m_audiosources;
	CASSMap m_cass;
	CLSMap m_cls;
};

}

#endif
