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

#ifndef PERFORMANCEVIEW_H
#define PERFORMANCEVIEW_H

namespace SA
{
// Main performance area sub window
//  - contains main faders and preset selection
class PerformanceView : public wxScrolledWindow
{
public:
	// class constructor
	PerformanceView(wxWindow* parent);
	// class destructor
	~PerformanceView();

	void Tick(float dT); // automation update for this window // triggers redraw
	void OnFaderMove(wxCommandEvent& event);
	void Init(); // set all MasterFader values to zero



private:
	std::vector<MasterFader*> masterFaderArray; // array to store MasterFaders (VPCs)

	// event handling macro
	DECLARE_EVENT_TABLE()
};
}
#endif // PERFORMANCEVIEW_H
