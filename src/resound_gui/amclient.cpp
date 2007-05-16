/*
 *   
 *   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
 *   
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *   
*/
#include "pch.h" // precompiler header

#include <resound_common/comms.h>
#include "automation.h"
#include "pvar.h"
#include "vumeterwidget.h"
#include "faderwidget.h"
#include "pvarwidgets.h"
#include "util.h"

#include "app.h"
#include "amclient.h" // classes header

// AMPVar -------------------------------------------------------------------------------------
SA::AMPVar::AMPVar()
{
	node = 0;
	needsUpdate = false;
}
SA::AMPVar::AMPVar(AMNode* _node)
{
	node = _node;
	needsUpdate = false;
}
SA::AMPVar::~AMPVar()
{
	node = 0;
}

void SA::AMPVar::OnValueChanged()
{
	// gets called on actual change of value
	needsUpdate = true;
}

void SA::AMPVar::SetNode(AMNode* _node)
{
	node = _node;
	needsUpdate = false;
}
bool SA::AMPVar::NodeNeedsUpdate()
{
	return needsUpdate;
}

// called by amclient when updates should be done
void SA::AMPVar::UpdateTargetNode()
{
	// update the target value
	if(node) {
		node->value = CLAMPF((float)GetValue() * (1.0f/128.0f), 0.0f, 1.0f); // set the value of the node and clamp it
	}
	needsUpdate = false;
}

// AM Client -------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SA::AMClient, wxEvtHandler)
EVT_SOCKET(AM_CLIENT_SOCKET_ID, SA::AMClient::OnSocketEvent)
END_EVENT_TABLE()

SA::AMClient::AMClient(wxTextCtrl* _log)
{

	SetName(_("Audio Matrix"));
	log = _log;
	socket = new wxSocketClient();
	// Setup the event handler and subscribe to events
	socket->SetFlags(wxSOCKET_WAITALL);
	socket->SetEventHandler(*this, AM_CLIENT_SOCKET_ID);
	socket->SetNotify(wxSOCKET_CONNECTION_FLAG |
	                  wxSOCKET_INPUT_FLAG |
	                  wxSOCKET_LOST_FLAG);
	socket->Notify(true);

	// null the matrix
	audioMatrix = 0;

	BuildAudioMatrix(10,10); // fake matrix
}
SA::AMClient::~AMClient()
{
	if(socket)
		delete socket;
}
// operations

void SA::AMClient::Connect(wxString hostname, int port)
{

	if(address.Hostname(hostname) && address.Service(port)) {
		socket->Connect(address,false);
	} else {
		wxMessageBox(_("Badly formatted host address!"));
	}
}
void SA::AMClient::Disconnect()
{}

// socket events
void SA::AMClient::OnSocketEvent(wxSocketEvent &event)
{
	SA::Packet t;

	switch(event.GetSocketEvent()) {
	case wxSOCKET_INPUT:

		// see whats in the buffer
		t = SA::PeekPacket(socket);
		//log->AppendText(wxString::Format(_("PeekPacket() - Type: %d Size: %d\n"),t.type,t.size));

		switch(t.type) {
			// known types
		case SA::PT_SERVER_HANDSHAKE:
			RecvServerHandshake();
			break;
		case SA::PT_AMNODE:
			RecvAMNode();
			break;

			// unknown or fault
		default:
			log->AppendText(_("Unknown packet type or malformed!\n"));
			break;
		}
		// p should now contain a valid pointer or null;


		break;
	case wxSOCKET_LOST:
		log->AppendText(_("Disconnect\n"));
		break;
	case wxSOCKET_CONNECTION:
		log->AppendText(_("Connect\n"));
		break;
	default:
		log->AppendText(_("Other message\n"));
		break;
	}

}

//called by event system for a server handshake event
void SA::AMClient::RecvServerHandshake()
{
	// validate message
	SA::AMServerHandshakePacket p;
	SA::RecvPacket(socket,&p);
	log->AppendText(wxString::Format(_("Server handshake... I/O: %d / %d \n"),p.numInputs,p.numOutputs));

	BuildAudioMatrix(p.numInputs,p.numOutputs);

	wxGetApp().RebuildGUI(); // cause the whole gui to be rebuilt - to reflect the change of server
}

//called by event system when data is available
void SA::AMClient::RecvAMNode()
{
	// node data received
	// decode and update matrix
	SA::AMNodePacket p;
	SA::RecvPacket(socket,&p);
	log->AppendText(wxString::Format(_("AMNodePacket Address: (%d, %d) Type: &d \n"),p.addr.row,p.addr.col,p.addr.type));

	//update matrix
	if(audioMatrix) // check ptr
	{
		audioMatrix->GetNode(p.addr) = p.node; // ref so should update as left side of assign
	}
}

// build the audio matrix and associated PVars
void SA::AMClient::BuildAudioMatrix(int _numInputs, int _numOutputs)
{
	// clear old matrix
	// construct new client side matrix
	if(audioMatrix)
		delete audioMatrix;

	numInputs = _numInputs;
	numOutputs = _numOutputs;

	audioMatrix = new AudioMatrix(numInputs,numOutputs);

	// make pvar matrix
	pVarMatrix.Create(numInputs+1,numOutputs+1);

	// fill in pvar details
	int r,c;
	for(r = 0; r < pVarMatrix.SizeX(); r++) {
		for(c = 0; c < pVarMatrix.SizeY(); c++) {
			wxString name;
			if(r == 0 && c == 0) // global
			{
				name = _("Global : Attenuate");
			} else if(r > 0 && c ==0) // input
			{
				name = wxString::Format(_("In %d Lvl"),r);
			} else if(r == 0 && c > 0) // output
			{
				name = wxString::Format(_("Out %d Lvl"),c);
			} else // matrix node
			{
				name = wxString::Format(_("M %d/%d Lvl"),r,c);
			}
			pVarMatrix.Index(r,c).SetName(name);
			AMNode* node = &audioMatrix->GetNode(AMNodeAddr(NAT_MATRIX_AMP,r,c));
			pVarMatrix.Index(r,c).SetNode(node);
		}
	}
}


// implement the PVarSubSystem interface
SA::PVSSettingsPanel* SA::AMClient::SettingsPanel(wxWindow* parent)
{
	// open a sub system settings dialog

	return 0;
}

SA::PVSSelectPanel* SA::AMClient::SelectPanel(wxWindow* parent)
{
	// open an appropriate dialog for PVar selection return the address or null address
	return new SA::AudioMatrixSelectPanel(parent,this);
}

SA::PVar& SA::AMClient::GetPVar(const PVarAddress &addr)
{
	// get a pvar at an address - may return a fake pvar
	// validate
	if(addr.type != 0)
		return nullPVar; // oooh i donty like this much

	int r = addr.row;
	int c = addr.col;
	int rows = pVarMatrix.SizeX();
	int cols = pVarMatrix.SizeY();
	if(r >= 0 && r < rows && c >= 0 && c < cols) {
		return pVarMatrix.Index(r,c);
	} else {
		return nullPVar; // oooh i donty like this much
	}
}

// tick
void SA::AMClient::Tick(float dT)
{
	// check pvars against matrix parameters transmit if required
	// maintain server and client copies
	if(socket->IsConnected()) {
		int r,c;
		for(r = 0; r < pVarMatrix.SizeX(); r++) {
			for(c = 0; c < pVarMatrix.SizeY(); c++) {
				AMPVar& t = pVarMatrix.Index(r,c);
				if(t.NodeNeedsUpdate()) // updates the node if required
				{
					log->AppendText(wxString::Format(_("Node %d, %d\n"),r,c));
					t.UpdateTargetNode();

					// updated the node - now post it to server

					AMNodePacket packet;
					packet.addr = AMNodeAddr(NAT_MATRIX_AMP,r,c);
					packet.node = audioMatrix->GetNode(packet.addr);
					SendPacket(socket,&packet);
				}
			}
		}
	}

}

// ----------------------------------------- AudioMatrixSelectPanel -----------------
SA::AudioMatrixSelectPanel::AudioMatrixSelectPanel(wxWindow* parent, PVarSubSystem* _subSystem)
		: PVSSelectPanel(parent,_subSystem)
{
	// cast subsystem to amClient - theoretically it can only be one of these
	AMClient* amClient = (AMClient*) subSystem;

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxGridSizer* gridSizer = new wxGridSizer(0,amClient->GetNumOutputs()+1,1,1);

	wxScrolledWindow *scroll = new wxScrolledWindow(this,-1);



	topSizer->Add(new wxStaticText(this,-1,_("Select panel")));
	for(int r = 0; r <= amClient->GetNumInputs(); r++) {
		for(int c = 0; c <= amClient->GetNumOutputs(); c++) {
			SA::AddressSelectWidget *panel = new SA::AddressSelectWidget(scroll,-1,PVarAddress(subSystem->GetId(),0,r,c));

			wxColour bkColour;

			if(r == 0 && c == 0) // global
			{
				bkColour = wxColour(50,50,50);
			} else if(r > 0 && c ==0) // input
			{
				bkColour = wxColour(50,150,50);
			} else if(r == 0 && c > 0) // output
			{
				bkColour = wxColour(150,50,50);
			} else // matrix node
			{
				bkColour = wxColour(50,50,150);
			}

			panel->SetBackgroundColour(bkColour);

			gridSizer->Add(panel);
		}
	}

	// for scroll
	// set minimum size to represent real size of window
	// set sizer
	// set virtual size hints
	// layout the sizer

	scroll->SetMinSize(wxSize(400,300));
	scroll->SetSizer(gridSizer);
	gridSizer->SetVirtualSizeHints(scroll);   // set size hints to honour minimum size
	gridSizer->Layout();

	scroll->SetScrollbars(20, 20, 50, 50);

	topSizer->Add(scroll);

	SetSizer(topSizer);
	topSizer->Layout();

}


