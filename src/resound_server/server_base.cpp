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
#include "sa_spch.h"

#include "sa_comms.h"
#include "sa_amserver.h"
#include "sa_server_base.h" // class header

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	MainFrame *win = new MainFrame(_("Resound - Server"), wxPoint (100, 100),
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
	netlog = new wxTextCtrl(this, wxID_ANY,
	                        _T("Log begins\n"),
	                        wxPoint(0,0), wxSize(220,175),
	                        wxTE_MULTILINE | wxTE_READONLY);
	topSizer->Add(netlog,wxSizerFlags(1).Center().Expand().Border(wxALL,0));

	// make the server
	server = new SA::AMServer(netlog); // construct and give the server access to the logging screen
	PushEventHandler(server); // allows the server to handle events


	// neaten up sizer
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();

	// setup dsp
	server->InitDSP();
	server->Connect(12345); // connect on this port

}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
	server->CloseDSP();
	delete server;

	Close(TRUE);
}

