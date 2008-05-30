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

#include "resound_client/automation.h"
#ifdef USE_MIDI
	#include "resound_client/mmanager.h"
#endif
#include <sys/time.h>

#include "performanceview.h"
#include "monitorview.h"
#include "behaviourview.h"

class AutomationDriver : public wxTimer{
	double prevT;
public:
	AutomationDriver(): prevT(gettime()){}
	void Notify(){
#ifdef USE_MIDI
		MManager::get_instance().tick();
#endif
		double t = gettime();
		Resound::AutomationManager::get_instance().tick(t-prevT); 
		prevT = t;
	}
	double gettime(){		
		timeval tv;
		gettimeofday(&tv, 0 );
		return double(tv.tv_sec) + double(tv.tv_usec) / 1000000.;
	}
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	void OnQuit(wxCommandEvent &event);

	// Functions to call when user selects 'save' or 'load' from File menu.
	// These call Save() or Load() respectively...
	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);

	// About box menu handler
	void OnAbout(wxCommandEvent& event);

	// force update menu handler
	void OnForceUpdate(wxCommandEvent& event);

	void RebuildGUI(); // force a gui rebuild
private:
	AutomationDriver m_automationDriver;
	Resound::PerformanceView *perfView;
	Resound::MonitorView* monitorView; // VIEWER - NOTHING TO SAVE/LOAD
	Resound::BehaviourView* behaviourView; // VIEWER - NOTHING TO SAVE/LOAD

	DECLARE_EVENT_TABLE()
};

enum
{
    ID_MAINWIN_QUIT = wxID_HIGHEST+1,
    ID_MAINWIN_LOAD,
    ID_MAINWIN_SAVE,
    ID_MAINWIN_ABOUT,
    ID_MAINWIN_FORCE_UPDATE
};


#endif
