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

#include "resound_client/basebehaviour.h"
#include "resound_client/pch.h"
#include <iostream>
// ----------------------------------------- Registration function -----------
void Resound::register_base_behaviours(BehaviourManager* theManager)
{
	theManager->register_behaviour_class_factory(BehaviourClassFactory("phsr", "Phasor", BPhasor::Factory));
	theManager->register_behaviour_class_factory(BehaviourClassFactory("mpcr", "Multipoint Crossfade", BMultiCrossfade::Factory));
	theManager->register_behaviour_class_factory(BehaviourClassFactory("wave", "Sinusoidal Wave", BWave::Factory));
	theManager->register_behaviour_class_factory(BehaviourClassFactory("mwve", "Mexican Wave", BMexicanWave::Factory));
	theManager->register_behaviour_class_factory(BehaviourClassFactory("rand", "Random", BRandom::Factory));
}

// ---------------------------------------- BPhasor ------------------

BOOST_CLASS_EXPORT(Resound::BPhasor);

// class constructor
Resound::BPhasor::BPhasor(std::string name) :
Resound::Behaviour(name),
m_amp(new BehaviourParameter("amp", this)),
m_freq(new BehaviourParameter("freq", this))
{
	register_parameter(m_amp);
	register_parameter(m_freq);
}

// class destructor
Resound::BPhasor::~BPhasor()
{
	// insert your code here
}

void Resound::BPhasor::on_parameter_changed(){

}

void Resound::BPhasor::tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 32.0f - 16.0;
	//std::cout << "B DT "<< dT << std::endl;
	// get the target collective
	Collective& rCol = get_collective();
	m_phasor.set_freq(freq);
	m_phasor.tick(dT);
	// apply to collective.Set(n)
	rCol.set_value(int(m_phasor.get_value() * 128.0f * amp));
}
// ---------------------------------------- Multicrossfade ------------------

BOOST_CLASS_EXPORT(Resound::BMultiCrossfade);

// class constructor
Resound::BMultiCrossfade::BMultiCrossfade(std::string name) :
Resound::Behaviour(name),
m_amp(new BehaviourParameter("amp", this)),
m_pos(new BehaviourParameter("pos", this)),
m_offset(new BehaviourParameter("offset", this))
{
	register_parameter(m_amp);
	register_parameter(m_pos);
	register_parameter(m_offset);
	// this 
}

// class destructor
Resound::BMultiCrossfade::~BMultiCrossfade()
{
	// insert your code here
}

void Resound::BMultiCrossfade::on_parameter_changed(){
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float pos = (float)m_pos->get_value() * (1.0f/128.0f) * TWOPI;
	float offset = (float)m_offset->get_value();
	// get the target collective
	Collective& rCol = get_collective();
	float phaseOffset = TWOPI / (float)rCol.get_num_elements();

	// calculate wave function
	float angle = -pos;

	// apply to collective.Set(n)
	for(int n = 0; n < rCol.get_num_elements(); n++) {
		float s = cosf(angle + (phaseOffset*n)) * amp;
		int val = (int)((s * 128.0f) + offset);
		rCol[n].set_value(val);
	}
}
// ---------------------------------------- Wave ----------------------------

BOOST_CLASS_EXPORT(Resound::BWave);

// class constructor
Resound::BWave::BWave(std::string name) :
Resound::Behaviour(name),
m_amp(new BehaviourParameter("amp", this)),
m_freq(new BehaviourParameter("freq", this)),
//m_phase(new BehaviourParameter("phase", this)),
m_offset(new BehaviourParameter("offset", this))
{
	register_parameter(m_amp);
	register_parameter(m_freq);
	//register_parameter(m_phase);
	register_parameter(m_offset);
}
// class destructor
Resound::BWave::~BWave()
{}

// overloaded
void Resound::BWave::tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 32.0f - 16.0;
	float offset = (float)m_offset->get_value();

	// get the target collective
	Collective& rCol = get_collective();

	m_phasor.set_freq(freq);
	m_phasor.tick(dT);

	float s = sinf(m_phasor.get_value() * TWOPI) * amp;
	int val = (int)((s * 128.0f) + offset);

	// apply to collective
	// this will apply to all elements and links
	rCol.set_value(val);
}
// ---------------------------------------- Mexican Wave ----------------------------

BOOST_CLASS_EXPORT(Resound::BMexicanWave);

// class constructor
Resound::BMexicanWave::BMexicanWave(std::string name) :
Resound::Behaviour(name),
m_amp(new BehaviourParameter("amp", this)),
m_freq(new BehaviourParameter("freq", this)),
//m_phase(new BehaviourParameter("phase", this)),
m_offset(new BehaviourParameter("offset", this))
{
	register_parameter(m_amp);
	register_parameter(m_freq);
//	register_parameter(m_phase);
	register_parameter(m_offset);

}
// class destructor
Resound::BMexicanWave::~BMexicanWave()
{}

// overloaded
void Resound::BMexicanWave::tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 32.0f - 16.0;
	float offset = (float)m_offset->get_value();

	// get the target collective
	Collective& rCol = get_collective();

	float phaseOffset = TWOPI / (float)rCol.get_num_elements();

	m_phasor.set_freq(freq);
	m_phasor.tick(dT);

	// apply to collective.Set(n)
	for(int n = 0; n < rCol.get_num_elements(); n++) {
		float s = sinf((m_phasor.get_value() * TWOPI) + (phaseOffset*n)) * amp;
		int val = (int)((s * 128.0f) + offset);
		rCol[n].set_value(val);
	}
}

// ---------------------------------------- Random ----------------------------

BOOST_CLASS_EXPORT(Resound::BRandom);

// class constructor
Resound::BRandom::BRandom(std::string name) :
Resound::Behaviour(name),
m_amp(new BehaviourParameter("amp", this)),
m_freq(new BehaviourParameter("freq", this))
{
	register_parameter(m_amp);
	register_parameter(m_freq);
}

// class destructor
Resound::BRandom::~BRandom()
{}

// overloaded
void Resound::BRandom::tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 32.0f - 16.0;

	// get the target collective
	Collective& rCol = get_collective();

	// apply to collective
	for(int r = 0; r < rCol.get_num_elements(); r++) {
		int val = (int)((float)((rand() % 256) - 128) * amp);
		rCol[r].set_value(val);
	}

}
