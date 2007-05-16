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
#ifndef __SA_COMMS__
#define __SA_COMMS__

// cant use precompiled headers here because the file crosses over client and server
// -----------------
#ifndef _WX_SOCKET_H_ // redundant include guard // improves compile time
#include <wx/socket.h>
#endif

#ifndef __ARRAY2__ // redundant include guard // improves compile time
#include "array2.h"
#endif
// -----------------

namespace SA
{
class AMCommsError
	{}
; // exception class for throwing errors
//--------------------------------------------------------------
// node address descriptor for the matrix

// note: prams in rows and columns are formated thus:
// row 0 column 0   = global params
// row 0 column 1-n = input params
// row 1-n column 0 = output params
// row 1-n column 1-n = matrix params

// type always specifies the sub parameter
//--------------------------------------------------------------

enum AMNODEADDR_TYPE
{
    // id                   ( data type, description )
    NAT_NULL = -1,          // ( null type invalid default)
    NAT_MATRIX_NAME = 0,    // ( string, matrix node name)
    NAT_MATRIX_AMP,         // ( float, matrix node amplification)
    NAT_WHOLE_NODE          // special case in node dump, type is irrelevent - whole node is copied

};
struct AMNodeAddr
{
	AMNODEADDR_TYPE type; // type see above
	int row; // row address - depends on type
	int col; // column address - depends on type

	// constructors
	AMNodeAddr()
	{
		type = NAT_NULL;
		row = col = 0;
	};
	AMNodeAddr(AMNODEADDR_TYPE _type, int _row, int _col)
	{
		type = _type;
		row = _row;
		col = _col;
	};

};

// a single audio matrix parameter node block
class AMNode
{
public:
	AMNode()
	{
		value = 0.0f;
	};
	float value; // value of this node
};

// a class that manages the audio matrix parameters
class AudioMatrix
{
public:
	// construct
	AudioMatrix(int numInputs, int numOutputs);

	// get a node specified by address
	AMNode& GetNode(const AMNodeAddr& addr);
private:
	int numInputs;
	int numOutputs;
	Array2<AMNode> matrix;
	AMNode nullNode; // BETTER USING EXCEPTION!!!
};

// network packet types
enum PACKET_TYPES
{
    PT_RAW = 0,
    PT_AMNODE,
    PT_SERVER_HANDSHAKE
};

// raw packet base class
struct Packet
{
	int type; // type of message
	int size; // size of packet

	Packet()
	{
		type = PT_RAW;
		size = sizeof(*this);
	}
	; // CHECK *this VIRTUAL VALID IN CONSTRUCTOR!
};

// Matrix node packets can be sent in both directions
// clients can set values on the server and vice versa
// in this way parameters are easily added to the matrix
// the matrix is always maintained at both ends

// node integer parameter packet
struct AMNodePacket : public Packet
{
	AMNodeAddr addr;
	AMNode node;
	AMNodePacket()
	{
		type = PT_AMNODE;
		size = sizeof(*this);
	};
}
;
;

// server handshake is sent by the server upon accepting a client
struct AMServerHandshakePacket : public Packet
{
	int numInputs;
	int numOutputs;
	AMServerHandshakePacket()
	{
		type = PT_SERVER_HANDSHAKE;
		size = sizeof(*this);
	};
};

// send and recieve packets-----
// send packet takes any packet derived object
// recv packet takes the approritae packet see peekpacket()
void SendPacket(wxSocketBase* socket, Packet* packet);
Packet PeekPacket(wxSocketBase* socket); // peeks the next packet so that callers can create the right packet for RecvPacket
void RecvPacket(wxSocketBase* socket, Packet* packet);

}
#endif
