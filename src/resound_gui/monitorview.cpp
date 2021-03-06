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
#include "monitorview.h"
#include "monitornodewidget.h"
#include <sstream>

BEGIN_EVENT_TABLE(Resound::MonitorView, wxScrolledWindow)

END_EVENT_TABLE()

// class constructor
Resound::MonitorView::MonitorView(wxWindow* parent, int id)
		: wxScrolledWindow(parent, id, wxPoint(0,0), wxSize(320,240))/* TODO (dave#1#): not sure this should be a scrolled window */
{

	scroll = 0;
	SetWindowStyle(wxCLIP_CHILDREN);

}

// class destructor
Resound::MonitorView::~MonitorView()
{
	// insert your code here

}

void Resound::MonitorView::Rebuild(AMClient* amClient)
{
	wxSizer* oldSizer = GetSizer();
	if(scroll) {
		scroll->Destroy();
	}
	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxGridSizer* gridSizer = new wxGridSizer(0,amClient->get_num_outputs()+1,1,1);

	scroll = new wxScrolledWindow(this,-1);

	int pId = 0;
	for(int r = 0; r <= amClient->get_num_inputs(); r++) {
		for(int c = 0; c <= amClient->get_num_outputs(); c++) {
			std::stringstream s;
			s << "/" << amClient->get_name() <<"/att/" << r << "/" << c; // generate the name
			Resound::MonitorNodeWidget *widget = new Resound::MonitorNodeWidget(scroll,pId++,ParameterAddress(s.str())); // FIXME pvar address // FIXED, pending testing

			wxColour bkColour;

			if(r == 0 && c == 0) // global
			{
				bkColour = wxColour(50,50,50); // TODO consider constants or setings for these colors
			} else if(r > 0 && c ==0) // input
			{
				bkColour = wxColour(50,150,50);
			} else if(r == 0 && c > 0) // output
			{
				bkColour = wxColour(150,50,50);
			} else // matrix node
			{
				bkColour = wxColour(50,50,150);
			}

			widget->SetBackgroundColour(bkColour);

			gridSizer->Add(widget);
		}
	}

	// for scroll
	// set minimum size to represent real size of window
	// set sizer
	// set virtual size hints
	// layout the sizer


	scroll->SetSizerAndFit(gridSizer);
	gridSizer->SetVirtualSizeHints(scroll);   // set size hints to honour minimum size
	gridSizer->Layout();

	scroll->SetScrollbars(20, 20, 50, 50);

	topSizer->Add(scroll,wxCENTER);
	topSizer->SetSizeHints(this);
	SetSizerAndFit(topSizer);
}
