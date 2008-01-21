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


#ifndef __RESOUND_CLIENT_H
#define __RESOUND_CLIENT_H
#include <string>
// A singleton class for general client data
class ResoundClient
{
public:
	int get_inputs(){return m_inputs;}
	void set_inputs(int inputs){m_inputs = inputs;}
	int get_outputs(){return m_outputs;}
	void set_outputs(int outputs){m_outputs = outputs;}
	std::string get_client_port() const {return m_clientPort;}
	void set_client_port(std::string clientPort){m_clientPort = clientPort;}
	std::string get_server_ip() const {return m_serverIp;}
	void set_server_ip(std::string serverIp){m_serverIp = serverIp;}
	std::string get_server_port() const {return m_serverPort;}
	void set_server_port(std::string serverPort){m_serverPort = serverPort;}
	
	static ResoundClient& get_instance();
private:
	// private app data
	int m_inputs;
	int m_outputs;
	std::string m_clientPort;
	std::string m_serverIp;
	std::string m_serverPort;

	static ResoundClient* m_singleton;
};

#endif // __APP_H
