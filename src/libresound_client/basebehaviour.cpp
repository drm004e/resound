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
m_freq(new BehaviourParameter("freq", this)),
m_phase(new BehaviourParameter("phase", this))
{
	register_parameter(m_amp);
	register_parameter(m_freq);
	register_parameter(m_phase);
}

// class destructor
Resound::BPhasor::~BPhasor()
{
	// insert your code here
}

void Resound::BPhasor::on_parameter_changed(){
	// get pvars and range adjust
	//float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	//float pos = (float)m_pos->get_value() * (1.0f/128.0f) * TWOPI;

	// get the target collective
	//Collective& rCol = get_collective();
	//float offset = TWOPI / (float)rCol.get_num_elements();

	// calculate wave function
	//float angle = -pos;

	// apply to collective.Set(n)
	//for(int n = 0; n < rCol.get_num_elements(); n++) {
	//	float s = cosf(angle + (offset*n)) * amp;
	//	int val = (int)(s * 128.0f);
	//	rCol[n].set_value(val);
	//}
}

void Resound::BPhasor::tick(float dT)
{
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
	// init
	angle = 0;
	/* TODO (dave#1#): angles have the potential to go behond float range over time
	may need better solution - this applies to all behaviours
	possible sollutions are to table lookup the sin wave and use bitwise & to cycle
	or a simple if statement garanteing range
	personnaly I would prefer the table lookup*/
}
// class destructor
Resound::BWave::~BWave()
{}

// overloaded
void Resound::BWave::tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 50.0f;
	float offset = (float)m_offset->get_value();

	// get the target collective
	Collective& rCol = get_collective();

	// calculate wave function
	angle += dT * freq;
	while(angle > TWOPI) angle -= TWOPI;
	while(angle < -TWOPI) angle += TWOPI;

	float s = sinf(angle) * amp;
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

	// init
	angle = 0;
}
// class destructor
Resound::BMexicanWave::~BMexicanWave()
{}

// overloaded
void Resound::BMexicanWave::tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)m_amp->get_value() * (1.0f/128.0f);
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 100.0f - 50.0f;
	float offset = (float)m_offset->get_value();

	// get the target collective
	Collective& rCol = get_collective();

	float phaseOffset = TWOPI / (float)rCol.get_num_elements();

	// calculate wave function
	angle += dT * freq;
	while(angle > TWOPI) angle -= TWOPI;
	while(angle < -TWOPI) angle += TWOPI;

	// apply to collective.Set(n)
	for(int n = 0; n < rCol.get_num_elements(); n++) {
		float s = sinf(angle + (phaseOffset*n)) * amp;
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
	float freq = (float)m_freq->get_value() * (1.0f/128.0f) * 100.0f - 50.0f;

	// get the target collective
	Collective& rCol = get_collective();

	// apply to collective
	for(int r = 0; r < rCol.get_num_elements(); r++) {
		int val = (int)((float)((rand() % 256) - 128) * amp);
		rCol[r].set_value(val);
	}

}
