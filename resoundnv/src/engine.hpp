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

#ifndef __ENGINE__
#define __ENGINE__


#include <string>
#include <map>
#include <libxml++/libxml++.h>



#include "jackengine.hpp"
#include "oscmanager.hpp"
#include "audiostream.hpp"
#include "ugen.hpp"

namespace Resound{

typedef std::map<std::string, AudioSource> AudioSourceMap;
typedef std::map<std::string, CoherentSet> CoherentSetMap;
typedef std::map<std::string, UGenPtr > UGenMap;

class Engine : public OSCManager, public JackEngine, public IUGenManager{
private:
	
	AudioSourceMap m_audioSources;
	CoherentSetMap m_coherentSets; 
	UGenMap m_uGens; 
public:
	Engine(const char* port, const char* initScript);
	virtual ~Engine();
	// --------------------------- JackEngine callbacks
	virtual int process(jack_nframes_t nframes);
	virtual void on_init();
	virtual void on_start();
	virtual void on_stop();
	virtual void on_close();
	virtual void on_sample_rate_changed();

	/// start the tcp server and wait for xml requests	
	int start_tcp_server();

	/// load and then parse some xml config
	void parse_xml(const char* path, bool isFile);
private:
	/// recursive node parsing
	void parse_xml_node(const xmlpp::Node* node); ///<entry
	void parse_xml_node_source(const xmlpp::Element* node); ///<decode audioSource
	void parse_xml_node_spkr(const xmlpp::Element* node); ///<decode audioSource
	void parse_xml_node_cset(const xmlpp::Element* node); ///<decode audioSourceset
	void parse_xml_node_ugen(const xmlpp::Element* node); ///<decode ugen


};
}

#endif
