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
#include "monitornodewidget.h"
#include "resound_client/performance.h"

// event table
BEGIN_EVENT_TABLE(Resound::MonitorNodeWidget, Resound::ParameterAddressWidgetBase)
EVT_TOGGLEBUTTON(MNW_LOCKBUTTON, Resound::MonitorNodeWidget::OnLockToggle)
EVT_LEFT_DOWN(Resound::MonitorNodeWidget::OnLeftMouseDown)
EVT_MOTION(Resound::MonitorNodeWidget::OnLeftMouseMove)
EVT_LEFT_UP(Resound::MonitorNodeWidget::OnLeftMouseUp)
EVT_RIGHT_UP(Resound::MonitorNodeWidget::OnRightMouseUp)
END_EVENT_TABLE()

// class constructor
Resound::MonitorNodeWidget::MonitorNodeWidget(wxWindow* parent, int id, ParameterAddress _addr)
		: Resound::ParameterAddressWidgetBase(parent,id,_addr)
{
	SetWindowStyle(wxSIMPLE_BORDER | wxCLIP_CHILDREN);
	SetBackgroundColour(wxColour(30,100,150));
	SetForegroundColour(wxColour(255,255,255));

	wxSize size(45,45);
	SetSize(size);
	SetMinSize(size);
	SetMaxSize(size);

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *leftSizer = new wxBoxSizer( wxVERTICAL );
	SetToolTip(wxConvertMB2WX(addr.get_address().c_str())); 
	//leftSizer->Add(new wxStaticText(this,MNW_LABEL,label, wxPoint(0,0),wxSize(40,18),wxALIGN_CENTRE),wxSizerFlags(0).Align(0).Border(wxALL,0));
	//leftSizer->Add(new wxToggleButton(this,MNW_LOCKBUTTON,_("L"), wxPoint(0,0),wxSize(20,20)),wxSizerFlags(0).Center().Border(wxALL,0));
	topSizer->Add(leftSizer);
	meter = new Resound::ParameterVUMeterWidget(this,MNW_METER,rand() % 128,0,128,_(RESOURCE_DIR "/image/smMeterOff.png"),_(RESOURCE_DIR "/image/smMeterOn.png"));
	meter->SetTarget(addr);
	topSizer->Add(meter,wxSizerFlags(0).Align(0).Border(wxALL,0)); //meter1
	SetSizer(topSizer);
	//topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

// class destructor
Resound::MonitorNodeWidget::~MonitorNodeWidget()
{
	// insert your code here
}

void Resound::MonitorNodeWidget::OnLockToggle(wxCommandEvent& event)
{

	bool isSelected = event.IsChecked();
	try{
		if(isSelected) {
			RESOUND_NAMESPACE()->get_parameter(GetAddress())->lock(127);
		} else {
			RESOUND_NAMESPACE()->get_parameter(GetAddress())->unlock();
		}
	} catch(ParameterAddressException& e){
		
	}
}
void Resound::MonitorNodeWidget::OnLeftMouseDown(wxMouseEvent& event)
{	
	
	m_lastY = event.GetY();
	CaptureMouse();
	event.Skip();
}
void Resound::MonitorNodeWidget::OnLeftMouseMove(wxMouseEvent& event)
{	
	if(event.LeftIsDown()){
		
		int dY = event.GetY() - m_lastY;
		m_lastY=event.GetY();
		int lockValue = RESOUND_NAMESPACE()->get_parameter(GetAddress())->get_value() - dY;
		if(lockValue <= 127 & lockValue >= 0){
			
			RESOUND_NAMESPACE()->get_parameter(GetAddress())->lock(lockValue);
			Refresh();
		}
	}
		
}
void Resound::MonitorNodeWidget::OnLeftMouseUp(wxMouseEvent& event)
{	
	ReleaseMouse();
}
void Resound::MonitorNodeWidget::OnRightMouseUp(wxMouseEvent& event)
{	
	RESOUND_NAMESPACE()->get_parameter(GetAddress())->unlock();
	ReleaseMouse();
}
