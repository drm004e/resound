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

#ifndef SA_MASTERFADER_H
#define SA_MASTERFADER_H

#ifndef __MManager__
    #include "mmanager.h"
#endif

namespace SA
{
enum MFG_ID
{
    MFG_FADER1 = 0,
    MFG_METER1,
    MFG_METER2,
    MFG_ASSIGN1,
    MFG_ASSIGN2,
    MFG_LABEL
};
// GUI panel fader block section
class MasterFader : public wxPanel,  public MListener
{
public:
	// class constructor
	MasterFader(wxWindow* parent, int id);
	// class destructor
	~MasterFader();

	void OnFaderChanged(wxCommandEvent &event);
	void SetValue(int value);


private:

	void OnMidiMessage(MIDI_BYTE status, MIDI_BYTE dataA, MIDI_BYTE dataB, MIDI_TIME_STAMP timeStamp);

	FaderWidget *fader1;
	StaticVUMeterWidget *meter1; // no saving implemented
	VUMeterWidget *meter2; // no saving implemented
	Collective collective;
	CollectiveWidget* collectiveWidget;

	// event handling macro
	DECLARE_EVENT_TABLE()
};

}
#endif // SA_MASTERFADER_H
