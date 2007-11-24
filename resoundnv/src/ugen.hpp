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

#ifndef __UGEN__
#define __UGEN__
#include <boost/shared_ptr.hpp>
namespace Resound{

class UGen;

/// IUGenManager is an interface to the actual ugen manager
/// the ugen manager takes care of plug<->mainapp communication
class IUGenManager{
public:
	/// get the current sample rate
//	virtual float get_sample_rate() = 0;
	/// is dsp active
	//virtual bool dsp_is_active() = 0;
	/// obtain a stream from an id
	//virtual AudioSource* get_stream_by_id(const std::string& id) = 0;
	/// used to signal ugen dsp activation
	//virtual void set_ugen_active(UGen* pUGen, bool isActive) = 0;
};

/// a ugen is the base class for signal processing units that connect audio streams in the system
class UGen{
	const IUGenManager* m_pUGenManager;
public:
	UGen(const IUGenManager* pUGenManager);
	virtual ~UGen();
	virtual void on_init() = 0;
	virtual void on_control() = 0;
	virtual void on_dsp() = 0;
};

typedef boost::shared_ptr<UGen> UGenPtr;
}
#endif