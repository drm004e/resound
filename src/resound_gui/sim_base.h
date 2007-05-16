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
#ifndef __BASE_H
#define __BASE_H

#include "audiomatrix.h"

class MainApp: public wxApp
{
public:
	virtual bool OnInit();
};

class MainFrame: public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	void OnQuit(wxCommandEvent &event);
private:

	SA::AMServer* server;
	DECLARE_EVENT_TABLE()
};

enum
{
    ID_MAINWIN_QUIT = wxID_HIGHEST+1
};


#endif
