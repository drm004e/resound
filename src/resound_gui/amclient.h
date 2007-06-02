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
#ifndef ____AUDIOMATRIX__
#define ____AUDIOMATRIX__

// all classes relating to the core of the audio matrix reside here
// include for transmited data format
// and helper classes

#include <resound_common/osc_manager.h>

#include "pvarwidgets.h"

namespace Resound
{
/// An audio matrix specialized pvar.
/// This class links real float attenuation values in the
/// audio matrix to the controlable Parameters.
class AMParameter : public Parameter
{
public:
	/// Default constructor
	AMParameter(){}; // needed for serialization
	AMParameter(const EntityName& name);

	/// Destructor
	virtual ~AMParameter();

	/// Called automatically by Parameter on actual change of value.
	virtual void on_value_changed();

	/// Check if the node needs update
	bool node_needs_update();

	/// Update the targeted node.
	/// Applies CLAMPF and converts from 0..128i to 0..1f
	void update_osc_target();

	/// set the OSC target of this node
	void set_osc_target(lo_address host, std::string path);

private:
	bool m_needsUpdate; // resound_server needs update
	lo_address m_hostAddress; ///< the host address for this server
	std::string m_oscAddress; ///< the osc path for this node

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Parameter);
        ar & BOOST_SERIALIZATION_NVP(m_needsUpdate);
		//ar & BOOST_SERIALIZATION_NVP(m_hostAddress); FIXME this wont go beacuse its a liblo class
		ar & BOOST_SERIALIZATION_NVP(m_oscAddress);
    }
};

/// The client side Audio Matrix Server.
/// This class deals with the socket event handling for network communication
class AMClient : public Resound::OSCManager, public ParameterNamespace, public AutomatedObject
{
public:
	/// Constructor
	/// @param _log : a pointer to a wxTextCtrl that can be used to log events
	AMClient(int inputs, int outputs);
	~AMClient(); ///< Destructor

	// implement the ParameterNamespace interface
	virtual void* SettingsPanel(wxWindow* parent); ///< Creates a sub system settings dialog
	virtual void* SelectPanel(wxWindow* parent); ///< create an appropriate dialog for Parameter selection return the address or null address

	int get_num_inputs()
	{
		return m_numInputs;
	}
	; ///< Gets the number of inputs on the current server connection
	int get_num_outputs()
	{
		return m_numOutputs;
	}
	; ///< Gets the number of outputs on the current server connection

	/// Automated update.
	/// Overriden from AutomatedObject,
	/// Causes the audiomatrix to be scanned, any changed matrix nodes are transmitted to the server.
	virtual void tick(float dT);

private:

	// AudioMatrix
	int m_numInputs; ///< Number of inputs on the server.
	int m_numOutputs; ///< Number of outputs on the server.

	/// Rebuilds the audio matrix.
	/// Any previous audio matrix is destroyed.
	/// @param _numInputs : the number of inputs to build.
	/// @param _numOutputs : the number of outputs to build.
	void build_parameter_matrix(int numInputs, int numOutputs);

//	Array2<AMParameterPtr> m_parameterMatrix; ///< AMParameters controlled by user
};

/// An interface panel visible in the collective builder dialog.
class AudioMatrixSelectPanel : public PVSSelectPanel
{
public:
	/// Constructor
	/// @param _subsystem : will be cast to AMClient*
	AudioMatrixSelectPanel(wxWindow* parent, ParameterNamespace* _subSystem);
};



}

#endif
