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
#ifndef __SA_AUDIOMATRIX__
#define __SA_AUDIOMATRIX__

// all classes relating to the core of the audio matrix reside here
// include for transmited data format
// and helper classes

enum ENUM_AM_CLIENT_SOCKET_IDS{
    AM_CLIENT_SOCKET_ID = 1
};

namespace SA
{
/// An audio matrix specialized pvar.
/// This class links real float attenuation values in the
/// audio matrix to the controlable PVars.
class AMPVar : public PVar
{
private:
	AMNode* node; // target
	bool needsUpdate; // target needs an update

public:
	/// Default constructor
	AMPVar();
	/// Construct and set target AMNode
	/// @param _node : an AMNode* to desired target node
	AMPVar(AMNode* _node);
	/// Destructor
	virtual ~AMPVar();

	/// Called automatically by PVar on actual change of value.
	virtual void OnValueChanged();

	/// Set the target AMNode
	/// @param _node AMNode* to desired target node
	void SetNode(AMNode* _node);

	/// Check if the node needs update
	bool NodeNeedsUpdate();

	/// Update the targeted node.
	/// Applies CLAMPF and converts from 0..128i to 0..1f
	void UpdateTargetNode();
};

/// The client side Audio Matrix Server.
/// This class deals with the socket event handling for network communication
class AMClient : public wxEvtHandler, public PVarSubSystem, public AutomatedObject
{
public:
	/// Constructor
	/// @param _log : a pointer to a wxTextCtrl that can be used to log events
	AMClient(wxTextCtrl* _log);
	~AMClient(); ///< Destructor

	/// Attempt connection to remote AMServer
	/// @param hostname : a string with a host name "localhost" for server on same machine
	/// @param port : integer port id number of service
	void Connect(wxString hostname, int port);
	void Disconnect(); ///< Disconnect from remote host

	// implement the PVarSubSystem interface

	virtual PVSSettingsPanel* SettingsPanel(wxWindow* parent); ///< Creates a sub system settings dialog
	virtual PVSSelectPanel* SelectPanel(wxWindow* parent); ///< create an appropriate dialog for PVar selection return the address or null address
	virtual PVar& GetPVar(const PVarAddress &addr); ///< get a pvar at an address - may return a fake pvar

	int GetNumInputs()
	{
		return numInputs;
	}
	; ///< Gets the number of inputs on the current server connection
	int GetNumOutputs()
	{
		return numOutputs;
	}
	; ///< Gets the number of outputs on the current server connection

	/// Automated update.
	/// Overriden from AutomatedObject,
	/// Causes the audiomatrix to be scanned, any changed matrix nodes are transmitted to the server.
	virtual void Tick(float dT);

private:

	// AudioMatrix
	int numInputs; ///< Number of inputs on the server.
	int numOutputs; ///< Number of outputs on the server.

	/// Rebuilds the audio matrix.
	/// Any previous audio matrix is destroyed.
	/// @param _numInputs : the number of inputs to build.
	/// @param _numOutputs : the number of outputs to build.
	void BuildAudioMatrix(int _numInputs, int _numOutputs);

	Array2<AMPVar> pVarMatrix; ///< AMPVars controlled by user
	AudioMatrix *audioMatrix; ///< Client side AudioMatrix pair with server version

	wxTextCtrl* log; ///< text control for login purposes passed in constructor

	// network
	wxSocketClient *socket; ///< The server connection socket
	wxIPV4address address; ///< Server address for connection, set by connect

	/// Socket event handler.
	/// Socket events are pre processed here, specific messages are handled elsewhere
	/// @see RecvServerHandshake()
	/// @see RecvAMNode()
	void OnSocketEvent(wxSocketEvent &event);

	// received data types

	/// Process server handshake
	void RecvServerHandshake();

	/// Process AMNodePacket from server
	void RecvAMNode();

	/// Null pVar for use when an invalid address is specified
	PVar nullPVar;

	DECLARE_EVENT_TABLE()

};

/// An interface panel visible in the collective builder dialog.
class AudioMatrixSelectPanel : public PVSSelectPanel
{
public:
	/// Constructor
	/// @param _subsystem : will be cast to AMClient*
	AudioMatrixSelectPanel(wxWindow* parent, PVarSubSystem* _subSystem);
};



}

#endif
