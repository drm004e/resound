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

#include "osc_manager.h"
#include <cstdio>
#include <iostream>

Resound::OSCManager::OSCManager(const char* port){
	// init OSC
	std::cout << "Initialising OSC listen thread... \n";
	m_loServerThread = lo_server_thread_new(port, Resound::OSCManager::lo_cb_error);

	// add the generic handler
	std::cout << "Adding OSC methods... \n";
    lo_server_thread_add_method(m_loServerThread, NULL, NULL, lo_cb_generic, this);
	lo_server_thread_add_method(m_loServerThread, "/syn", NULL, lo_cb_syn, this);
	lo_server_thread_add_method(m_loServerThread, "/ack", NULL, lo_cb_ack, this);
	// start
	lo_server_thread_start(m_loServerThread);


	// send myself a test message
	std::cout << "Sending self-test OSC messages... \n";
	lo_address t = lo_address_new(NULL, port);
	lo_send(t, "/syn", "s", "resound_server selftest syn/ack");
}
Resound::OSCManager::~OSCManager(){
	if(m_loServerThread) {lo_server_thread_free(m_loServerThread);}
}
	// liblo callbacks

void Resound::OSCManager::lo_cb_error(int num, const char *msg, const char *path){
    std::printf("liblo server error %d in path %s: %s\n", num, path, msg);
    std::fflush(stdout);
}
int Resound::OSCManager::lo_cb_generic(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
    int i;
	std::cout<< "OSC recvfrom " << lo_address_get_url(lo_message_get_source(data))
			 << " to " << path << " Args(";

    for (i=0; i<argc; i++) {
	std::cout << "[";
	lo_arg_pp((lo_type)types[i], argv[i]);
	std::cout << "] ";
    }
    std::cout << ")\n";

    return 1;
}
int Resound::OSCManager::lo_cb_syn(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	((OSCManager*)user_data)->recv_syn(path,types,argv,argc,data,user_data);
    return 1;
}
int Resound::OSCManager::lo_cb_ack(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	((OSCManager*)user_data)->recv_ack(path,types,argv,argc,data,user_data);
    return 1;
}

void Resound::OSCManager::recv_syn(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	std::string url(lo_address_get_url(lo_message_get_source(data)));
	ActiveClientMap::iterator it = m_clients.find(url);
	if(it == m_clients.end()){
		// this is a new client tel the world
		std::cout << "OSC new client detected at " << url << "\n";
	}
	m_clients[url] = 3; // three strikes and your out!

	// send back an ack
	lo_send(lo_message_get_source(data), "/ack", "s", "resound_server build 1.0");
}

void Resound::OSCManager::recv_ack(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	std::cout << "recv_ack" << "\n";
}

void Resound::OSCManager::update_clients(){
	ActiveClientMap::iterator it;
	for(it=m_clients.begin(); it != m_clients.end(); /*no increment because of removal see later*/){
		--it->second;
		int f = it->second;
		if(it->second <= 0){
			std::cout << "OSC client " << it->first << " has disconnected. Removing from active client list.\n";
			// remove him cause we havent heard from him for a while
			ActiveClientMap::iterator prev = it;
			++it; // increment iterator
			m_clients.erase(prev); // remove from behind
		} else {
			++it;
		}
	}
	
}