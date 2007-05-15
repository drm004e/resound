/*
 *   
 *   Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.
 *   
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *   
*/
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "sim_base.h"

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	MainFrame *win = new MainFrame(_("Server"), wxPoint (100, 100),
	                               wxSize(450, 340));
	win->Show(TRUE);
	SetTopWindow(win);

	return TRUE;
}


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_MAINWIN_QUIT, MainFrame::OnQuit)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
		: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
	wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);

	// add the log control
	wxTextCtrl *netLog = new wxTextCtrl(this, wxID_ANY,
	                                    _("AMServer...\n"),
	                                    wxPoint(0,0), wxSize(200,450),
	                                    wxTE_MULTILINE | wxTE_READONLY);
	topSizer->Add(netLog,wxSizerFlags(0).Center().Border(wxALL,0));

	// make the server
	server = new SA::AMServer(netLog);
	PushEventHandler(server);
	server->Connect(12345);

	// neaten up sizer
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
	Close(TRUE);
}
