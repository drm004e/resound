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

#ifndef __OSC_MANAGER_H_
#define __OSC_MANAGER_H_

#include <lo/lo.h> // liblo OSC
#include <string>
#include <map>

namespace Resound{

/// class for managing an osc server and providing functions for clients to send osc messages.
/// acts as a wrapper layer around liblo
class OSCManager{
public:
	OSCManager(const char* port);
	virtual ~OSCManager();
	// liblo callbacks
	/// liblo error callback
	static void lo_cb_error(int num, const char *msg, const char *path);
	static int lo_cb_generic(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
	static int lo_cb_syn(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
	static int lo_cb_ack(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
	/// call this to every second or so to update the client list
	/// any clients that have failed to send a ping recently will be removed
	void update_clients();

	/// have received a ping from the osc url specified
	/// overriders should call the base class first
	virtual void recv_syn(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
	virtual void recv_ack(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
private:
	// OSC
	lo_server_thread m_loServerThread; ///< the liblo server thread

	typedef std::map<std::string, int> ActiveClientMap;
	ActiveClientMap m_clients; ///< managed map of active clients .. ie ones that are pinging
};
}

#endif