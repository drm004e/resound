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
#include "faderwidget.h"
#include "resound_client/util.h"

// implement custom events
DEFINE_EVENT_TYPE(Resound::saEVT_FADER_CHANGED)

// event table
BEGIN_EVENT_TABLE(Resound::FaderWidget, wxControl)
EVT_PAINT(Resound::FaderWidget::OnPaint)
EVT_LEFT_DOWN(Resound::FaderWidget::OnLeftMouseDown)
EVT_LEFT_UP(Resound::FaderWidget::OnLeftMouseUp)
EVT_MOTION(Resound::FaderWidget::OnMouseMotion)
END_EVENT_TABLE()

// class constructor
Resound::FaderWidget::FaderWidget(wxWindow *parent, int id, int _val, int _min, int _max)
		: wxControl(parent, id,wxPoint(0,0), wxSize(0,0))
{
	// insert your code here
	bmFaderBack.LoadFile(_(RESOURCE_DIR "/image/faderBack.png"),wxBITMAP_TYPE_PNG);
	bmFaderKnob.LoadFile(_(RESOURCE_DIR "/image/faderKnob.png"),wxBITMAP_TYPE_PNG);
	dcFaderBack.SelectObject(bmFaderBack);
	dcFaderKnob.SelectObject(bmFaderKnob);

	sizeX = bmFaderBack.GetWidth();
	sizeY = bmFaderBack.GetHeight();
	kSizeX = bmFaderKnob.GetWidth();
	kSizeY = bmFaderKnob.GetHeight();

	trackYBegin = kSizeY/2;
	trackYSize = sizeY - (kSizeY/2);

	SetWindowStyleFlag(wxNO_BORDER);
	wxSize size(sizeX,sizeY);
	SetSize(size);
	SetMinSize(size);
	SetMaxSize(size);

	val = min = 0;
	max = 128;

	// set defaults
	SetValue(_val);
	SetRange(_min,_max);
}

// class destructor
Resound::FaderWidget::~FaderWidget()
{
	// insert your code here
}


// paint handling
void Resound::FaderWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	//int y = (sizeY - (kSizeY/2)) - RANGEMAP(val,min,max,0,sizeY);
	//This keeps the graphical fader cap 'inside' the track
	int y = (sizeY - (kSizeY/2)) - RANGEMAP(val,min,max,0+(kSizeY/2),sizeY-(kSizeY/2));
	// blit back portion // top bit
	dc.Blit(wxPoint(0,0),wxSize(sizeX,sizeY),&dcFaderBack,wxPoint(0,0));
	// blit on portion // bottom bit
	dc.Blit(wxPoint(0,y),wxSize(sizeX,sizeY),&dcFaderKnob,wxPoint(0,0));
}
// mouse handling

void Resound::FaderWidget::OnLeftMouseDown(wxMouseEvent& event)
{
	int y = event.GetY();
	int newVal = RANGEMAP(y,0,sizeY,max,min); // inverse map
	SetValue(newVal);
	// proccess default evt handler
	event.Skip();
	CaptureMouse();
}
void Resound::FaderWidget::OnLeftMouseUp(wxMouseEvent& event)
{
	ReleaseMouse();
}
void Resound::FaderWidget::OnMouseMotion(wxMouseEvent& event)
{
	if(event.Dragging()) {
		int y = event.GetY();

		int newVal = RANGEMAP(CLAMP(y,0,sizeY),0,sizeY,max,min); // inverse map
		SetValue(newVal);
	}

}


// settings
void Resound::FaderWidget::SetValue(int _val)
{
	if(val != _val) {
		// value has changed so update the value
		// send events
		// refresh the screen
		val = _val;

		// send the event
		int id = GetId();
		wxCommandEvent event( saEVT_FADER_CHANGED, id );
		event.SetEventObject( this );

		// Give it some contents ie the value we have just set
		event.SetInt(val); // pass val to listener
		// Send it
		GetEventHandler()->ProcessEvent( event );

		// refresh screen
		Refresh(false);
	}
};

void Resound::FaderWidget::SetValueNoEvent(int _val)
{
	if(val != _val) {
		val = _val;
		// refresh screen
		Refresh(false);
	}
}
int Resound::FaderWidget::GetValue()
{
	return val;
}

void Resound::FaderWidget::SetRange(int _min, int _max)
{
	if(_min < _max) {
		min = _min;
		max = _max;
		Refresh(false);
	}
};

// time elapsed function for decay and peak hold extension
void Resound::FaderWidget::Tick(float dT)
{}

