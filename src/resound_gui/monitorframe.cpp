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

#include "wxincludes.h"
#include "monitorframe.h"

BEGIN_EVENT_TABLE(Resound::MonitorFrame, wxFrame)

END_EVENT_TABLE()

// class constructor
Resound::MonitorFrame::MonitorFrame(wxWindow* parent, int id, const wxPoint &pos, const wxSize &size)
		: wxFrame(parent, id, _("Monitor"), pos, size)
{
	monitorView = new Resound::MonitorView(this,-1);
	// create top sizer and fill with stuff
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add(monitorView,wxSizerFlags(0).Center().Border(wxALL,0));
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

// class destructor
Resound::MonitorFrame::~MonitorFrame()
{
	// insert your code here
}
