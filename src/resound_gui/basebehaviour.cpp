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
#include "pch.h" // precompiler header

#include "automation.h"
#include "pvar.h"
#include "behaviour.h" // inheriting class's header file
#include "basebehaviour.h" // class's header file

// ----------------------------------------- Registration function -----------
void SA::RegisterBaseBehaviours(BehaviourManager* theManager)
{
	theManager->RegisterBehaviourClass(BehaviourClass(FourCharId('bpgr'),_T("Proportional Group"), BPGroup::Factory));
	theManager->RegisterBehaviourClass(BehaviourClass(FourCharId('mpcr'),_T("Multipoint Crossfade"), BMultiCrossfade::Factory));
	theManager->RegisterBehaviourClass(BehaviourClass(FourCharId('wave'),_T("Sinusoidal Wave"), BWave::Factory));
	theManager->RegisterBehaviourClass(BehaviourClass(FourCharId('mwve'),_T("Mexican Wave"), BMexicanWave::Factory));
	theManager->RegisterBehaviourClass(BehaviourClass(FourCharId('rand'),_T("Random"), BRandom::Factory));
}

// ----------------------------------------- Proportinal group ---------------

// class constructor
SA::BPGroup::BPGroup()
{
	AddPVar(_T("Level"));
	AddPVar(_T("Min"));
	AddPVar(_T("Max"));
}

// class destructor
SA::BPGroup::~BPGroup()
{
	// insert your code here
}

// ---------------------------------------- Multicrossfade ------------------

// class constructor
SA::BMultiCrossfade::BMultiCrossfade()
{
	AddPVar(_T("Position"));
}

// class destructor
SA::BMultiCrossfade::~BMultiCrossfade()
{
	// insert your code here
}

// ---------------------------------------- Wave ----------------------------
// class constructor
SA::BWave::BWave()
{
	AddPVar(_T("Amp"));
	AddPVar(_T("Freq"));

	// init
	angle = 0;
	/* TODO (dave#1#): angles have the potential to go behond float range over time
	may need better solution - this applies to all behaviours
	possible sollutions are to table lookup the sin wave and use bitwise & to cycle
	or a simple if statement garanteing range
	personnaly I would prefer the table lookup*/
}
// class destructor
SA::BWave::~BWave()
{}

// overloaded
void SA::BWave::Tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)GetPVar(0).GetValue() * (1.0f/128.0f);
	float freq = (float)GetPVar(1).GetValue() * (1.0f/128.0f) * 50.0f;

	// get the target collective
	Collective& rCol = GetCollective();

	// calculate wave function
	angle += dT * freq;
	float s = sinf(angle) * amp;
	int val = (int)(s * 128.0f);

	// apply to collective
	// this will apply to all elements and links
	rCol.SetValue(val);
}
// ---------------------------------------- Mexican Wave ----------------------------
// class constructor
SA::BMexicanWave::BMexicanWave()
{
	AddPVar(_T("Amp"));
	AddPVar(_T("Freq"));

	// init
	angle = 0;
}
// class destructor
SA::BMexicanWave::~BMexicanWave()
{}

// overloaded
void SA::BMexicanWave::Tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)GetPVar(0).GetValue() * (1.0f/128.0f);
	float freq = (float)GetPVar(1).GetValue() * (1.0f/128.0f) * 100.0f - 50.0f;

	// get the target collective
	Collective& rCol = GetCollective();

	float offset = TWOPI / (float)rCol.GetNumElements();

	// calculate wave function
	angle += dT * freq;

	// apply to collective.Set(n)
	for(int n = 0; n < rCol.GetNumElements(); n++) {
		float s = sinf(angle + (offset*n)) * amp;
		int val = (int)(s * 128.0f);
		rCol[n].SetValue(val);
	}
}

// ---------------------------------------- Random ----------------------------
// class constructor
SA::BRandom::BRandom()
{
	AddPVar(_T("Amp"));
	AddPVar(_T("Freq"));
}

// class destructor
SA::BRandom::~BRandom()
{}

// overloaded
void SA::BRandom::Tick(float dT)
{
	// get pvars and range adjust
	float amp = (float)GetPVar(0).GetValue() * (1.0f/128.0f);
	float freq = (float)GetPVar(1).GetValue() * (1.0f/128.0f) * 100.0f - 50.0f;

	// get the target collective
	Collective& rCol = GetCollective();

	// apply to collective
	for(int r = 0; r < rCol.GetNumElements(); r++) {
		for(int c = 0; c < rCol[r].GetNumLinks(); c++) {
			int val = (int)((float)((rand() % 256) - 128) * amp);
			rCol[r][c].SetValue(val);
		}
	}

}
