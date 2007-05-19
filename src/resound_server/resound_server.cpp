/**********************************************************************************
    Resound
    Copyright 2007 David Moore and James Mooney 

    This file is part of Resound.

    Resound is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Resound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Resound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

************************************************************************************/
#include "pch.h"

#include <iostream>
#include <boost/program_options.hpp>
#include "dsp_manager.h"
#include <string>

Resound::DSPManager* s_dsp = 0;

int inputs = 2;
int outputs = 2;
std::string port("4567");

/// command line options
bool parse(int argc, char** argv){
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("inputs", po::value<int>(), "set number of audio inputs")
		("outputs", po::value<int>(), "set number of audio outputs")
		("port", po::value<std::string>(), "set number OSC server port")
	;
	
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
	
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	if (vm.count("inputs")) {
		inputs = vm["inputs"].as<int>();
	}
	if (vm.count("outputs")) {
		outputs = vm["outputs"].as<int>();
	}
	if(inputs < 0 || inputs > 128) { std::cout << "Inputs should be in the range 1-128\n"; return 1;}
	if(outputs < 0 || outputs > 128) { std::cout << "Outputs should be in the range 1-128\n"; return 1;}

	if (vm.count("port")) {
		port = vm["port"].as<std::string>();
	}

	return 0;
}

/// run the daemon
int run(){
	// enter the daemon loop
	for(;;)
	{
		try{
			usleep(1000);
		}
		catch(...){
			return 1;
		}	
	}
	return 0;
}
/// entry point
int main(int argc, char** argv){
	// parse command line
	// options
	if(parse(argc,argv)) return 1;
	s_dsp = new Resound::DSPManager("Resound",inputs,outputs,port.c_str());
	return run();
}
