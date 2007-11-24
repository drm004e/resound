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
#include "ugen_att.hpp"

// Server code in C
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <strings.h>
  #include <unistd.h>
 



using namespace Resound;

Engine::Engine(const char* port, const char* initScript) :
OSCManager(port)
{
	std::cout << "-Starting Resound-\n";
	//init("Resoundnv");
	std::cout << "Parsing initialisation script...\n";
	//parse_xml(initScript, true);
	// start the xml tcp server
	start_tcp_server();
	
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

void Engine::parse_xml(const char* path, bool isFile){
	try
	{
		xmlpp::DomParser parser;
		parser.set_validate(false);
		parser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
		if(isFile){
			parser.parse_file(path);
		} else {
			parser.parse_memory(path);
		}
		if(parser){
			const xmlpp::Node* pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
			parse_xml_node(pNode);
		}
	}catch(const std::exception& ex){
		std::cout << "XML parsing exception: " << ex.what() << std::endl;
	}
}

void Engine::parse_xml_node(const xmlpp::Node* node){
	const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
 	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  	const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

  	if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
    		return;

	if(!nodeContent)
	{
		//Recurse through child nodes:
		xmlpp::Node::NodeList list = node->get_children();
		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter){
			Glib::ustring nodename = (*iter)->get_name();
			if(nodename == "source"){
				parse_xml_node_source(dynamic_cast<const xmlpp::Element*>(*iter));
			} else if(nodename == "cset"){
				parse_xml_node_cset(dynamic_cast<const xmlpp::Element*>(*iter));
			} else if(nodename == "ugen"){
				parse_xml_node_ugen(dynamic_cast<const xmlpp::Element*>(*iter));
			} else { // unknown
			}
		}
	}
}
void Engine::parse_xml_node_source(const xmlpp::Element* node){
	
	// figure out the type of source, make one and set it up
	const xmlpp::Attribute* id = node->get_attribute("id");
	const xmlpp::Attribute* type = node->get_attribute("type");
	if(id){
		std::string strId = id->get_value();
		std::string strType = type->get_value();
		std::cout << "Source: " << strId << " - " << strType << std::endl;
		m_audioSources[strId] = AudioSource(); 
	}
}
void Engine::parse_xml_node_cset(const xmlpp::Element* node){
	const xmlpp::Attribute* id = node->get_attribute("id");
	const xmlpp::Attribute* type = node->get_attribute("type");
	if(id){
		std::string strId = id->get_value();
		std::string strType = type->get_value();
		std::cout << "CoherentSet: " << strId << " - " << strType << std::endl;
		m_coherentSets[strId] = CoherentSet(); 
	}
}
void Engine::parse_xml_node_ugen(const xmlpp::Element* node){
	const xmlpp::Attribute* id = node->get_attribute("id");
	const xmlpp::Attribute* type = node->get_attribute("type");
	if(id){
		std::string strId = id->get_value();
		std::string strType = type->get_value();
		std::cout << "UGen:" << strId << " - " << strType << std::endl;
		m_uGens[strId] = UGenPtr(new UGenAtt(this));
	}
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
		std::cout << "Accept socket from" << inet_ntoa(clientAddr.sin_addr) << std::endl;
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

		// now send some stuff back
		const char* str="<b>Server comms is ok</b>";
		std::cout << "send: " << str << std::endl; 
		::send(clientSocket,str,strlen(str),0);

		::shutdown(clientSocket, 2);
		::close(clientSocket);
	}
	return 0;
}

