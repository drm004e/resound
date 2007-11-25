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

#include "engine.hpp"
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
 



using namespace Resound;

Engine::Engine(const char* port, const char* initScript) :
//OSCManager(port),
m_root(new DynamicObject("resound")) // an empty document
{
	
	std::cout << "-Starting Resound-\n";
	//init("Resoundnv");
	std::cout << "Parsing initialisation script...\n";
	parse_xml_file(initScript);
	std::cout << get_xml_string();
	// start the xml tcp server
	//start_tcp_server(); // initialises a thread
}
Engine::~Engine(){

	std::cout << "-Shutdown complete-\n";
}

int Engine::process(jack_nframes_t nframes){
}
void Engine::on_init(){

	add_output_mono_buss ( "MainL" );
	add_output_mono_buss ( "MainR" );
}
void Engine::on_start(){
}
void Engine::on_stop(){
}
void Engine::on_close(){
}
void Engine::on_sample_rate_changed(){
}

void Engine::parse_xml_file(const char* path){
	try
	{
		xmlpp::DomParser parser;
		parser.set_validate(false);
		parser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
		parser.parse_file(path);
		if(parser){
			const xmlpp::Node* pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
			m_root->parse_xml(pNode);
		}
	}catch(const std::exception& ex){
		std::cout << "XML parsing exception: " << ex.what() << std::endl;
	}
}

void Engine::parse_xml_string(const std::string &str){
	try
	{
		xmlpp::DomParser parser;
		parser.set_validate(false);
		parser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
		parser.parse_memory(str);
		if(parser){
			const xmlpp::Node* pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
			m_root->parse_xml(pNode);
		}
	}catch(const std::exception& ex){
		std::cout << "XML parsing exception: " << ex.what() << std::endl;
	}
}

std::string Engine::get_xml_string(){
	std::stringstream s;
	m_root->get_xml_string(s);
	return s.str();
}

int Engine::start_tcp_server(){
	int32_t listenSocket = ::socket(PF_INET, SOCK_STREAM, 0);

	if(-1 == listenSocket){
		printf("can not create socket");
		exit(-1);
	}

	sockaddr_in stSockAddr;
	bzero(&stSockAddr, sizeof(stSockAddr));

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(1100);
	stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(-1 == ::bind(listenSocket,(struct sockaddr*) &stSockAddr, sizeof(stSockAddr))){
		printf("error bind failed");
		exit(-1);
	}

	if(-1 == ::listen(listenSocket, 10)){
		printf("error listen failed");
		exit(-1);
	}

	for(;;){
		int32_t clientSocket = ::accept(listenSocket, NULL, NULL);
		sockaddr_in clientAddr;
		socklen_t s = sizeof(sockaddr_in);
		getpeername(clientSocket, (sockaddr*)&clientAddr, &s);
		std::cout << "Accept socket from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
		if(0 > clientSocket)
		{
			printf("error accept failed");
			exit(-1);
		}

		// perform read write operations ...
		char buffer[256];
		int n = recv(clientSocket,buffer,255,0);
		buffer[n]='\0'; // null terminate the string
		std::cout << "recv: "<< buffer << std::endl; 
		if(std::strncmp(buffer,"GET ",4)==0){
			const char* str="<html><head></head><body>Resound server is running</body></html>";
			::send(clientSocket,str,strlen(str),0);
		} else if (std::strncmp(buffer,"SETXML",6)==0){
			std::cout << "SETXML: " << buffer << std::endl;
			const char* str="OK"; // or we return FAIL Reason it failed
			::send(clientSocket,str,strlen(str),0);
		} else if (std::strncmp(buffer,"GETXML",6)==0){
			std::string xml=get_xml_string();
			::send(clientSocket,xml.c_str(),xml.size(),0);
		} else {
			std::cout << "Unknown header: " << buffer << std::endl; 
		}


		::shutdown(clientSocket, 2);
		::close(clientSocket);
	}
	return 0;
}

