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

#include "jackengine.hpp"

JackEngine::JackEngine()
{	
	m_dspIsRunning = false;
	pthread_mutex_init(&m_dspMutex,0);
}

JackEngine::~JackEngine(){
	close();
	pthread_mutex_destroy(&m_dspMutex);
}
void JackEngine::init(const std::string name){
	m_jc = jack_client_open(name.c_str(),JackNullOption,0);

	// set all the callbacks
	jack_set_buffer_size_callback(m_jc,JackEngine::jack_buffer_size_callback,this);
	jack_set_freewheel_callback(m_jc,JackEngine::jack_freewheel_callback,this);
	jack_set_graph_order_callback(m_jc,JackEngine::jack_graph_order_callback,this);
	jack_set_port_registration_callback(m_jc,JackEngine::jack_port_registration_callback,this);
	jack_set_sample_rate_callback(m_jc,JackEngine::jack_sample_rate_callback,this);
	jack_set_process_callback(m_jc,JackEngine::jack_process_callback,this);
	jack_set_thread_init_callback(m_jc,JackEngine::jack_thread_init_callback,this);
	jack_set_xrun_callback(m_jc,JackEngine::jack_xrun_callback,this);
	// get some info from jackd about current SR and bufferSize;
	m_bufferSize = jack_get_buffer_size(m_jc);
	m_sampleRate = jack_get_sample_rate(m_jc);

	this->on_init();
}
void JackEngine::start(){
	assert(m_jc);
	jack_activate(m_jc);
	m_dspIsRunning = true;
	this->on_start();
}
void JackEngine::stop(){
	assert(m_jc);
	jack_deactivate(m_jc);
	m_dspIsRunning = false;
	this->on_stop();
}
void JackEngine::close(){
	if(m_jc) jack_client_close(m_jc);
	m_jc=0;
	this->on_close();
}

void JackEngine::add_input_mono_buss(std::string name){
	m_inputs[name] = (jack_port_register(m_jc, name.c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput,0));
}
void JackEngine::add_output_mono_buss(std::string name){
	m_outputs[name] = (jack_port_register(m_jc, name.c_str(),JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput,0));
}
void JackEngine::remove_buss(std::string name){
}

float* JackEngine::get_output_buffer(const std::string port, jack_nframes_t nframes){
	jack_port_t* p = m_outputs[port];
	return (float*)jack_port_get_buffer(m_outputs[port],nframes);
}


// jack callbacks ---------------- these are all static
int JackEngine::jack_buffer_size_callback(jack_nframes_t nframes, void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	return ptr->on_buffer_size(nframes);
}

void JackEngine::jack_freewheel_callback(int starting, void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	ptr->on_freewheel(starting);
}

int JackEngine::jack_graph_order_callback(void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	return ptr->on_graph_order();
}

void JackEngine::jack_port_registration_callback(jack_port_id_t port, int, void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	ptr->on_port_registration(port);
}

int JackEngine::jack_process_callback(jack_nframes_t nframes, void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	ptr->lock_dsp_mutex();
	int ret = ptr->on_process(nframes);
	ptr->unlock_dsp_mutex();
	return ret;
}

int JackEngine::jack_sample_rate_callback(jack_nframes_t nframes, void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	return ptr->on_sample_rate(nframes);
}

void JackEngine::jack_thread_init_callback(void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	ptr->on_thread_init();
}

int JackEngine::jack_xrun_callback(void *arg){
	JackEngine* ptr = static_cast<JackEngine*>(arg);
	assert(ptr);
	return ptr->on_xrun();
}

