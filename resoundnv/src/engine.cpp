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


using namespace Resound;

Engine::Engine(const char* port, const char* initScript) :
OSCManager(port)
{
	std::cout << "-Starting Resound-\n";
	//init("Resoundnv");
	std::cout << "Parsing initialisation script...\n";
	parse_xml(initScript, true);
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

