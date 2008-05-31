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
// Class automatically generated by Dev-C++ New Class wizard

#ifndef __BASEBEHAVIOUR_H
#define __BASEBEHAVIOUR_H

#include "behaviour.h"
#include "automation.h"
#include "pch.h"
// A selection of basic beahviours
namespace Resound
{

// function to register the behaviours in this file
void register_base_behaviours(BehaviourManager* theManager);

/// this is a helper phasor class used by other behaviours for cyclic stuff
class Phasor{
private:
	float m_freq;
	float m_phase;
public:
	Phasor(){ m_freq = 1.0f; m_phase = 0.0f;}
	/// increment the phasor phase by dT seconds
	void tick(float dT) {
		// increment by the dT * freq
		// check bounds
		m_phase += dT * m_freq * TWOPI;
		while(m_phase > TWOPI){m_phase -= TWOPI;}
		while(m_phase < 0){m_phase += TWOPI;}
	}	
	/// set the frequecy of the phasor
	void set_freq(float v) { m_freq = v; }
	/// return the current frequency	
	float get_freq() { return m_freq; }
	/// set the phase ie. the position between 0 - 2PI
	void set_phase(float v) { m_phase = fmod(v,TWOPI); }
	/// get the phase position 0 - 2PI
	float get_phase() { return m_phase; }
	/// get the value of the phasor at its current phase
	float get_value() {
		return m_phase * 1.0f/TWOPI;
	}
};


// phasor a simple positive phoasor with amplitude and frequency
class BPhasor : public Behaviour, public AutomatedObject
{
private:
	Phasor m_phasor;
public:
	static BehaviourPtr Factory()
	{
		return BehaviourPtr(new BPhasor("mp_phasor"));
	};
	// class constructor
	BPhasor(){}
	BPhasor(std::string name);
	// class destructor
	~BPhasor();

	void on_parameter_changed();
	virtual void tick(float dT);
private:
	ParameterPtr m_amp;
	ParameterPtr m_freq;

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Behaviour);
        ar & BOOST_SERIALIZATION_NVP(m_amp);
	ar & BOOST_SERIALIZATION_NVP(m_freq);
	}
};

// multi cross fade
// uses first set of collective
class BMultiCrossfade : public Behaviour
{
public:
	static BehaviourPtr Factory()
	{
		return BehaviourPtr(new BMultiCrossfade("mp_crossfade"));
	};
	// class constructor
	BMultiCrossfade(){}
	BMultiCrossfade(std::string name);
	// class destructor
	~BMultiCrossfade();

	void on_parameter_changed();
private:
	ParameterPtr m_amp;
	ParameterPtr m_pos;
	ParameterPtr m_offset;
	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Behaviour);
        ar & BOOST_SERIALIZATION_NVP(m_amp);
    	ar & BOOST_SERIALIZATION_NVP(m_pos);
	ar & BOOST_SERIALIZATION_NVP(m_offset);
	}
};

// wave behaviour
class BWave : public Behaviour, public AutomatedObject
{
public:
	static BehaviourPtr Factory()
	{
		return BehaviourPtr(new BWave("wave"));
	};
	// class constructor
	BWave(){}
	BWave(std::string name);
	// class destructor
	~BWave();

	// overloaded
	virtual void tick(float dT);

private:
	Phasor m_phasor;
	ParameterPtr m_amp;
	ParameterPtr m_freq;
	ParameterPtr m_phase;
	ParameterPtr m_offset;
	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Behaviour);
        ar & BOOST_SERIALIZATION_NVP(m_amp);
    	ar & BOOST_SERIALIZATION_NVP(m_freq);
	ar & BOOST_SERIALIZATION_NVP(m_phase);
	ar & BOOST_SERIALIZATION_NVP(m_offset);
	}
};

// wave behaviour
class BMexicanWave : public Behaviour, public AutomatedObject
{
public:
	static BehaviourPtr Factory()
	{
		return BehaviourPtr(new BMexicanWave("mexican_wave"));
	};
	// class constructor
	BMexicanWave(){}
	BMexicanWave(std::string name);
	// class destructor
	~BMexicanWave();

	// overloaded
	virtual void tick(float dT);

private:
	Phasor m_phasor;
	ParameterPtr m_amp;
	ParameterPtr m_freq;
	ParameterPtr m_phase;
	ParameterPtr m_offset;

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Behaviour);
        ar & BOOST_SERIALIZATION_NVP(m_amp);
    	ar & BOOST_SERIALIZATION_NVP(m_freq);
	ar & BOOST_SERIALIZATION_NVP(m_phase);
	ar & BOOST_SERIALIZATION_NVP(m_offset);
	}
};

// wave behaviour
class BRandom : public Behaviour, public AutomatedObject
{
	ParameterPtr m_amp;
	ParameterPtr m_freq;
public:
	static BehaviourPtr Factory()
	{
		return BehaviourPtr(new BRandom("random"));
	};
	// class constructor
	BRandom(){}
	BRandom(std::string name);
	// class destructor
	~BRandom();

	// overloaded
	virtual void tick(float dT);

private:

	friend class boost::serialization::access; ///< allow serialization access at low level
	/// serialization definition
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Behaviour);
        }

};

}
#endif // __BASEBEHAVIOUR_H
