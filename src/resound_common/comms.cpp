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
// cant have precompiled header becuase of shared nature between projects
#include "comms.h"

// ---------------------- Packets -----------------------------
void SA::SendPacket(wxSocketBase* socket, Packet* packet)
{
	// packet should contain everything so
	// cast packet into void*
	// use size as data transmit size

	int size = packet->size;
	socket->Write(packet, size);

	if(socket->LastCount() != size) {
		// packet send did not complete
		throw(AMCommsError());
	}
}
SA::Packet SA::PeekPacket(wxSocketBase* socket)
{
	// peek a Packet from the input buffer
	// return it so users can construct correct type

	Packet p;
	socket->Peek(&p,sizeof(Packet));
	return p;
}
void SA::RecvPacket(wxSocketBase* socket, Packet* packet)
{
	// Packet type should be correct
	// read directly into the object

	socket->Read(packet,packet->size);
	if(socket->LastCount() != packet->size) {
		throw(AMCommsError());
	}
}

// ------------------------------------   AudioMatrix ---------------------

SA::AudioMatrix::AudioMatrix(int _numInputs, int _numOutputs)
{
	numInputs = _numInputs;
	numOutputs = _numOutputs;
	matrix.Create(numInputs+1, numOutputs+1);
}

SA::AMNode& SA::AudioMatrix::GetNode(const AMNodeAddr& addr)
{
	// check address is valid

	if( addr.row >= 0 &&
	        addr.row <= numInputs &&
	        addr.col >= 0 &&
	        addr.col <= numOutputs) {
		return matrix.Index(addr.row, addr.col);
	} else {
		// throw exception here i think?
		throw(AMCommsError());
		return nullNode; // null node
	}
}

