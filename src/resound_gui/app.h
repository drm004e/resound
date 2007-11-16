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
// Class automatically g8enerated by Dev-C++ New Class wizard

#ifndef __APP_H
#define __APP_H
#include <string>
// The main application class
class ResoundClientApp: public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnRun();
	virtual int OnExit();
	virtual void OnUnhandledException();

	// application level operations
	void RebuildGUI();
	bool parse(int argc, char** argv);
	
	int get_inputs(){return inputs;}
	int get_outputs(){return outputs;}
	std::string get_client_port(){return clientPort;}
		std::string get_server_ip(){return serverIp;}
	std::string get_server_port(){return serverPort;}
private:
	// private app data
	int inputs;
	int outputs;
	std::string clientPort;
	std::string serverIp;
	std::string serverPort;
	bool verbose;
};

DECLARE_APP(ResoundClientApp)

#endif // __APP_H
