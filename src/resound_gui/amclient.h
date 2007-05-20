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

#include <resound_common/osc_manager.h>

namespace SA
{
/// An audio matrix specialized pvar.
/// This class links real float attenuation values in the
/// audio matrix to the controlable PVars.
class AMPVar : public PVar
{
private:
	bool needsUpdate; // resound_server needs update
	lo_address m_hostAddress; ///< the host address for this server
	std::string m_oscAddress; ///< the osc path for this node
public:
	/// Default constructor
	AMPVar();

	/// Destructor
	virtual ~AMPVar();

	/// Called automatically by PVar on actual change of value.
	virtual void OnValueChanged();

	/// Check if the node needs update
	bool NodeNeedsUpdate();

	/// Update the targeted node.
	/// Applies CLAMPF and converts from 0..128i to 0..1f
	void UpdateTarget();

	/// set the OSC target of this node
	void set_target(lo_address host, std::string path);
	
};

/// The client side Audio Matrix Server.
/// This class deals with the socket event handling for network communication
class AMClient : public Resound::OSCManager, public PVarSubSystem, public AutomatedObject
{
public:
	/// Constructor
	/// @param _log : a pointer to a wxTextCtrl that can be used to log events
	AMClient(wxTextCtrl* _log);
	~AMClient(); ///< Destructor

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

	wxTextCtrl* log; ///< text control for login purposes passed in constructor


	/// Null pVar for use when an invalid address is specified
	PVar nullPVar;

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
