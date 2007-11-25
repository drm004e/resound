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
#include "core_objects.hpp"

namespace Resound{

/// the engine class is the main workhorse of the server. It sets up OSC, TCP comms, JACK and xml DOM parsing and brings them all together
class Engine :  public JackEngine{
private:
	DynamicObjectPtr m_root; ///< the main DOM object
public:
	/// constructor
	Engine(const char* port, const char* initScript);

	/// destructor	
	virtual ~Engine();

	// JackEngine callbacks
	/// The main dsp process callback	
	virtual int process(jack_nframes_t nframes);
	/// called on initikalisation of jackd
	virtual void on_init();
	/// called when dsp processing begins
	virtual void on_start();
	/// called when dsp processing stops
	virtual void on_stop();
	/// called when jack is closed
	virtual void on_close();
	/// called when the sample rate has changed
	virtual void on_sample_rate_changed();

	/// start the tcp server and wait for xml requests	
	int start_tcp_server();

	/// parse and merge xml data
	/// merge an xml file into the current tree
	/// form a temporary DOM and parse it importing nodes into the main tree
	void parse_xml_file(const char* path);
	
	/// parse and merge xml data
	/// merge an xml string into the current tree
	/// form a temporary DOM and parse it importing nodes into the main tree
	void parse_xml_string(const std::string &str);

	/// get a string containing the current DOM in xml form
	std::string get_xml_string();
	
private:

};
}

#endif
