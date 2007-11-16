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
// Class automatically generated by Dev-C++ New Class wizard

#include "pch.h" // precompiler header

#include "automation.h"
#include "pvar.h"
#include "behaviour.h"
#include "basebehaviour.h"

// view classes
#include "vumeterwidget.h"
#include "faderwidget.h"
#include "pvarwidgets.h"
#include "masterfader.h"
#include "performanceview.h"

#include "amclient.h"
#include "monitorview.h"

#include "behaviourview.h"

#include "base.h"

#include "app.h" // class's header file

#include <wx/image.h>

#include <boost/program_options.hpp>
IMPLEMENT_APP(ResoundClientApp)



/// command line options
bool ResoundClientApp::parse(int argc, char** argv){
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("inputs,i", po::value<int>()->default_value(2), "set number of audio inputs")
		("outputs,o", po::value<int>()->default_value(8), "set number of audio outputs")
		("clientport,c", po::value<std::string>()->default_value("8000"), "set the client gui fader osc port")
		("serverip,s", po::value<std::string>()->default_value("127.0.0.1"), "set the server osc ip address")
		("serverport,p", po::value<std::string>()->default_value("4567"), "set the server osc port")
		("verbose,v", "verbose output")
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

	if (vm.count("clientport")) {
		clientPort = vm["clientport"].as<std::string>();
	}
	if (vm.count("serverport")) {
		serverPort = vm["serverport"].as<std::string>();
	}
	if (vm.count("serverip")) {
		serverIp = vm["serverip"].as<std::string>();
	}
	if(vm.count("verbose")) {
		Resound::useVerbose = true;
	}
	return 0;
}

bool ResoundClientApp::OnInit()
{
	char** nargv = new char*[argc];
	int nargc = argc;
	for(int n=0; n < nargc; n++){
		nargv[n] = new char[strlen(wxConvertWX2MB(argv[n]))];
		strcpy(nargv[n],wxConvertWX2MB(argv[n]));
	}	
	if(parse(argc,nargv)) return false;
	std::cout << "Starting client:\nInputs " << inputs << "\nOutputs " << outputs << "\nClient OSC Port " << clientPort << "\nServer OSC IP " << serverIp << "\nServer OSC Port " << serverPort<< std::endl;
	wxInitAllImageHandlers();
	try {
		MainFrame *win = new MainFrame(_T("Resound Client"), wxPoint (100, 100),wxSize(0, 0));
		
		win->Show(TRUE);
		SetTopWindow(win);

		return true;
	} catch(std::exception& e) {
		wxMessageBox(wxString(e.what(), *wxConvCurrent),_T("Unhandled Exception"));
	} catch(...) {
		wxMessageBox(_T("Resound has encountered an unexpected exception\nand has been forced to shutdown."),_T("Unhandled Exception"));
	}
	return false;
}

int ResoundClientApp::OnRun()
{
	try {
		wxApp::OnRun();
	} catch(std::exception& e) {
		wxMessageBox(wxString(e.what(), *wxConvCurrent),_T("Unhandled Exception"));
	} catch(...) {
		wxMessageBox(_T("Resound has encountered an unexpected exception\nand has been forced to shutdown."),_T("Unhandled Exception"));
	}
}

int ResoundClientApp::OnExit()
{
	Resound::AutomationManager::destroy_instance();
	return wxApp::OnExit();
}

void ResoundClientApp::OnUnhandledException()
{
}



void ResoundClientApp::RebuildGUI()
{
	((MainFrame*)GetTopWindow())->RebuildGUI();
}
