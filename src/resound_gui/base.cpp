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
#include "pch.h" // precompiler header

#include "automation.h"
#include "pvar.h"
#include "behaviour.h"
#include "basebehaviour.h"

// view classes
#include "vumeterwidget.h"
#include "faderwidget.h"
#include "pvarwidgets.h"
#include "masterfader.h"
#include "performanceview.h"

#include "amclient.h"
#include "monitorview.h"
#include "behaviourview.h"

#include "base.h"


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_MAINWIN_QUIT, MainFrame::OnQuit)
EVT_MENU(ID_MAINWIN_LOAD, MainFrame::OnLoad)
EVT_MENU(ID_MAINWIN_SAVE, MainFrame::OnSave)
EVT_MENU(ID_MAINWIN_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
		: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{

	// setup menus
	wxMenu *FileMenu = new wxMenu;
	wxMenu *HelpMenu = new wxMenu;
	wxMenuBar *MenuBar = new wxMenuBar;

	FileMenu->Append(ID_MAINWIN_QUIT, _T("&Quit"));
	FileMenu->Append(ID_MAINWIN_LOAD, _T("&Load Performance"));
	FileMenu->Append(ID_MAINWIN_SAVE, _T("&Save Performance"));

	MenuBar->Append(FileMenu, _T("&File"));

	HelpMenu->Append(ID_MAINWIN_ABOUT, _T("&About Resound"));

	MenuBar->Append(HelpMenu, _T("Help"));

	SetMenuBar(MenuBar);

	CreateStatusBar(2);
	SetStatusText(_T("Resound - The State of the Art in Live Sound Diffusion"));


	// create the notebook pane left side
	wxNotebook* leftBook = new wxNotebook(this,-1);


	wxTextCtrl *netLog = new wxTextCtrl(leftBook, wxID_ANY,
	                                    _T("Netlog...\n"),
	                                    wxPoint(0,0), wxSize(300,100),
	                                    wxTE_MULTILINE | wxTE_READONLY);


	//wxMessageBox(_T("ok"));

	// make the network client
	m_audioMatrix = new SA::AMClient();

	// make the behaviour sub system
	m_behaviourManager = new SA::BehaviourManager();

	// register sub systems

	SA::ParameterNamespaceManager::get_instance().register_parameter_namespace(SA::ParameterNamespacePtr(m_audioMatrix));
	SA::ParameterNamespaceManager::get_instance().register_parameter_namespace(SA::ParameterNamespacePtr(m_behaviourManager));

	// setup midi system
	/* this system is undefined in linux!! FIXME linux midi

	MIDIDeviceNameArray& inNames = MManager::get_instance().GetInputDeviceNames();
	MIDIDeviceNameArray& outNames = MManager::get_instance().GetOutputDeviceNames();

	for(int n = 0; n < inNames.size(); n++) {
		netLog->AppendText(wxString(_T(" MIDI input device found: ")) + wxString(inNames[n].c_str())+ wxString(_T("\n")));
		MManager::get_instance().OpenInputDevice(n);
	}
	for(int n = 0; n < outNames.size(); n++) {
		netLog->AppendText(wxString(_T(" MIDI output device found: ")) + wxString(outNames[n].c_str())+ wxString(_T("\n")));
		MManager::get_instance().OpenOutputDevice(n);
	}
	*/

	// add any behaviours
	// eventually load plugins!
	RegisterBaseBehaviours(m_behaviourManager);

	perfView = new SA::PerformanceView(leftBook);
	monitorView = new SA::MonitorView(leftBook,-1,m_audioMatrix);
	behaviourView = new SA::BehaviourView(leftBook,-1,m_behaviourManager);
	leftBook->AddPage(perfView,_T("Master"),true);
	leftBook->AddPage(monitorView,_T("Matrix"),false);
	leftBook->AddPage(behaviourView,_T("Behaviour"),false);
	leftBook->AddPage(netLog,_T("Console"),false);
	// create top sizer and fill with stuff
	wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
	topSizer->Add(leftBook,wxSizerFlags(1).Center().Border(wxALL,1));

	// neaten up sizer
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event)) // THIS SEEMS TO CAUSE AN EXCEPTION!
{
	SA::ParameterNamespaceManager::destroy_instance();
	MManager::Destroy();
	PopEventHandler();
	Close(TRUE);
}

void MainFrame::OnSave(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Save Performance to File"), _T(""), _T(""), _T("*.rpf"), wxSAVE);
	if (dlg.ShowModal() == wxID_OK) {
		// create file output stream from dialog box path
		wxFileOutputStream fileStream(dlg.GetPath());

		// create X-platform data output stream from file stream
		wxDataOutputStream stream(fileStream);

	}

}

void MainFrame::OnLoad(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Load Performance from File"), _T(""), _T(""), _T("*.rpf"), wxOPEN);
	if (dlg.ShowModal() == wxID_OK) {
		// create file output stream from dialog box path
		wxFileInputStream fileStream(dlg.GetPath());

		// create X-platform data output stream from file stream
		wxDataInputStream stream(fileStream);
	}
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(_T("Resound\n Licensed under the terms of the GNU Public Licence Version 2.0\n"), _T("About Resound"));
}


void MainFrame::RebuildGUI()
{
	// force a gui rebuild
	monitorView->Rebuild();
}
