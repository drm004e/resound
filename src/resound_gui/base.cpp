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

#include "performance.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_MAINWIN_QUIT, MainFrame::OnQuit)
EVT_MENU(ID_MAINWIN_LOAD, MainFrame::OnLoad)
EVT_MENU(ID_MAINWIN_SAVE, MainFrame::OnSave)
EVT_MENU(ID_MAINWIN_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
		: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
	// first we create an empty performance

	RESOUND_PERFORMANCE_MANAGER().new_performance();


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


	//wxTextCtrl *netLog = new wxTextCtrl(leftBook, wxID_ANY,_T("Netlog...\n"),wxPoint(0,0), wxSize(300,100),wxTE_MULTILINE | wxTE_READONLY);


	//wxMessageBox(_T("ok"));

	// make the network client
	Resound::ParameterNamespacePtr m_audioMatrix = Resound::ParameterNamespacePtr(new Resound::AMClient(8,8));

	// make the behaviour sub system
	Resound::ParameterNamespacePtr m_behaviourManager = Resound::ParameterNamespacePtr(new Resound::BehaviourManager());

	// register sub systems

	RESOUND_NAMESPACE()->register_parameter_namespace(m_audioMatrix);
	RESOUND_NAMESPACE()->register_parameter_namespace(m_behaviourManager);

	// setup midi system

	//MManager::get_instance().OpenInputDevice(1);

	// add any behaviours
	// eventually load plugins!
	register_base_behaviours(dynamic_cast<Resound::BehaviourManager*>(m_behaviourManager.get()));

	perfView = new Resound::PerformanceView(leftBook);
	monitorView = new Resound::MonitorView(leftBook,-1);
	monitorView->Rebuild(dynamic_cast<Resound::AMClient*>(m_audioMatrix.get()));
	behaviourView = new Resound::BehaviourView(leftBook,-1);
	leftBook->AddPage(perfView,_T("Master"),true);
	leftBook->AddPage(monitorView,_T("Matrix"),false);
	leftBook->AddPage(behaviourView,_T("Behaviour"),false);
	//leftBook->AddPage(netLog,_T("Console"),false);
	// create top sizer and fill with stuff
	wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
	topSizer->Add(leftBook,wxSizerFlags(1).Center().Border(wxALL,1));

	// neaten up sizer
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();

	// start the automation driver
	m_automationDriver.Start(5);

}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event)) // THIS SEEMS TO CAUSE AN EXCEPTION!
{
	MManager::destroy_instance();
	PopEventHandler();
	Close(TRUE);
}

void MainFrame::OnSave(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Save Performance to File"), _T(""), _T(""), _T("*.rpf"), wxSAVE);
	if (dlg.ShowModal() == wxID_OK) {
		// create file output stream from dialog box path
		perfView->store_to_preset(0);
		Resound::PerformanceManager::get_instance().save_performance_xml((const char*)wxConvertWX2MB(dlg.GetPath()));
	}

}

void MainFrame::OnLoad(wxCommandEvent& event)
{
	wxFileDialog dlg(this, _T("Load Performance from File"), _T(""), _T(""), _T("*.rpf"), wxOPEN);
	if (dlg.ShowModal() == wxID_OK) {
		// create file output stream from dialog box path
		Resound::PerformanceManager::get_instance().load_performance_xml((const char*)wxConvertWX2MB(dlg.GetPath()));
		perfView->recall_from_preset(0);
		monitorView->Rebuild(dynamic_cast<Resound::AMClient*>(&RESOUND_NAMESPACE()->get_parameter_namespace(0)));
		behaviourView->BuildPanel();
	}
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
wxMessageBox(_T(
"Resound: \n\n\
Copyright (c) 2007 David Moore, James Mooney, All Rights Reserved.\n\n\
This program is free software; you can redistribute it and/or \
modify it under the terms of the GNU General Public License as \
published by the Free Software Foundation; either version 2 of \
the License, or (at your option) any later version.\n\n\
This program is distributed in the hope that it will be useful, \
but WITHOUT ANY WARRANTY; without even the implied warranty of \
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \
GNU General Public License for more details.\n\n\
You should have received a copy of the GNU General Public License \
along with this program; if not, write to the Free Software \
Foundation, Inc., 59 Temple Place, Suite 330, Boston,\n\
MA 02111-1307 USA\n\
\nContributors:\n\
\tDr D. Moore\n\
\tDr J. Mooney\n\
"), _T("About Resound"));
}


void MainFrame::RebuildGUI()
{
	// force a gui rebuild
	//monitorView->Rebuild();
}
