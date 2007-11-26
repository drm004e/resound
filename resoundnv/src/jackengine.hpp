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

#ifndef __JACKENGINE_H__
#define __JACKENGINE_H__

//#include "buffer.hpp"
#include <jack.h>
#include <string>
#include <cassert>
#include <map>
#include <pthread.h>

/// an abstract base class for using jack in a class
class JackEngine {
private:

	pthread_mutex_t m_dspMutex;

	/// a list of pointers to ports
	typedef std::map<std::string, jack_port_t*> JackPortList;

	JackPortList m_inputs; ///< the actual jack input ports
	JackPortList m_outputs; ///< the actual jack output ports

	std::string m_name; ///< the jack client name
	jack_client_t* m_jc; ///< the jack client ptr

	jack_nframes_t m_bufferSize; ///< the current bufferSize
	jack_nframes_t m_sampleRate; ///< the current sample rate

	bool m_dspIsRunning;
public:
	JackEngine();
	virtual ~JackEngine();
	void init(const std::string name);
	void start();
	void stop();
	void close();
	void add_input_mono_buss(std::string name);
	void add_output_mono_buss(std::string name);
	void remove_buss(std::string name);
	float* get_output_buffer(const std::string port, jack_nframes_t nframes);
	
	/// override with your dsp code
	
	virtual void on_init(){}; // after jack has been initialised
	virtual void on_start(){}; // prior to start
	virtual void on_stop(){}; // post stoped
	virtual void on_close(){}; // post closed

	// these are virtualized versions of the callbacks
	virtual int on_buffer_size(jack_nframes_t nframes){ return 0; }
	virtual void on_freewheel(int starting){}
	virtual int on_graph_order(){ return 0; }
	virtual void on_port_registration(jack_port_id_t port){}
	virtual int on_process(jack_nframes_t nframes){ return 0;};
	virtual int on_sample_rate(jack_nframes_t nframes){ return 0;}
	virtual int on_thread_init(){ return 0;}
	virtual int on_xrun(){ return 0;}

	/// return the actual client pointer
	jack_client_t* get_client(){return m_jc;}
	
	// the dsp mutex guards the dsp code and will be locked for the duration of the process
	// you are advised to only lock this mutex for very short times in order that signal processing can continue
	void lock_dsp_mutex(){pthread_mutex_lock(&m_dspMutex);}
	void unlock_dsp_mutex(){pthread_mutex_unlock(&m_dspMutex);}

	bool dsp_is_running(){return m_dspIsRunning;}

private: 
	/// jack static callbacks
	static int jack_buffer_size_callback(jack_nframes_t nframes, void *arg);
	static void jack_freewheel_callback(int starting, void *arg);
	static int jack_graph_order_callback(void *arg);
	static void jack_port_registration_callback(jack_port_id_t port, int, void *arg);
	static int jack_process_callback(jack_nframes_t nframes, void *arg);
	static int jack_sample_rate_callback(jack_nframes_t nframes, void *arg);
	static void jack_thread_init_callback(void *arg);
	static int jack_xrun_callback(void *arg);
	
};

#endif
