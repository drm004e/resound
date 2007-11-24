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

#include <iostream>
#include <boost/program_options.hpp>
#include "main.hpp"
#include "engine.hpp"

/// usefull tip: sometimes the liblo wont reconnect because the jackd process seems to take ownership of the network address
/// in this event:
/// lsof -i
/// this displays the list of processes by port id
/// kill the jackd process that is on the same id
/// this was usefull http://www.redhat.com/docs/manuals/linux/RHL-8.0-Manual/security-guide/s1-server-ports.html


Resound::Engine* theEngine = 0;

std::string port;
std::string initScript;

/// command line options
bool parse(int argc, char** argv){
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("port", po::value<std::string>()->default_value("21057"), "set number OSC server port")
		("init", po::value<std::string>()->default_value("init.xml"), "set the initialisation script")
	;
	
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
	
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	if (vm.count("port")) {
		port = vm["port"].as<std::string>();
	}

	if (vm.count("init")) {
		initScript = vm["init"].as<std::string>();
	}
	return 0;
}

/// run the daemon
int run(){
	// enter the daemon loop

		for(;;)
		{
			usleep(1000000); // one second
			theEngine->update_clients(); // cause the client list to be updated
			std::cout.flush(); //flush the output stream so we get updated display in KDevelop
		}


	return 0;
}
/// entry point
int main(int argc, char** argv){
	// parse command line
	if(parse(argc,argv)) return 1;
	// init the dsp manager
	theEngine = new Resound::Engine(port.c_str(), initScript.c_str());
	// run for ever
	//return run();
}
