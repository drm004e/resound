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
#ifndef __BASE_H
#define __BASE_H

class MainFrame: public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	void OnQuit(wxCommandEvent &event);

	// Functions to call when user selects 'save' or 'load' from File menu.
	// These call Save() or Load() respectively...
	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);

	// The actual save and load functions for the main frame
	// Return TRUE (successful) or FALSE (unsuccessful)
	void Save(wxDataOutputStream& stream);
	void Load(wxDataInputStream& stream);

	// Tom trying to get to grips with using widgets events
	void MainFrame::OnAbout(wxCommandEvent& event);

	void RebuildGUI(); // force a gui rebuild
private:

	SA::PerformanceView *perfView;
	SA::MonitorView* monitorView; // VIEWER - NOTHING TO SAVE/LOAD
	SA::BehaviourView* behaviourView; // VIEWER - NOTHING TO SAVE/LOAD

	SA::AMClient *amClient; // NETWORK SUB-SYSTEM - NOTHING TO SAVE/LOAD
	SA::BehaviourManager *theBehaviourManager; // REQUIRES SAVING

	DECLARE_EVENT_TABLE()
};

enum
{
    ID_MAINWIN_QUIT = wxID_HIGHEST+1,
    ID_MAINWIN_LOAD,
    ID_MAINWIN_SAVE,
    ID_MAINWIN_ABOUT
};


#endif
