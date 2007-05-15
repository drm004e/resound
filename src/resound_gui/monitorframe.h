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

#ifndef SA_MONITORFRAME_H
#define SA_MONITORFRAME_H

#include <wx/wx.h> // inheriting class's header file
#include "sa_monitorView.h"

namespace SA
{

// Frame window for holding the monitorView class
class MonitorFrame : public wxFrame
{
public:
	// class constructor
	MonitorFrame(wxWindow* parent, int id, const wxPoint &pos, const wxSize &size);
	// class destructor
	~MonitorFrame();
private:

	SA::MonitorView* monitorView;

	DECLARE_EVENT_TABLE()
};
}

#endif // SA_MONITORFRAME_H
