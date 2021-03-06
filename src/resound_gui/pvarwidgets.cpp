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
#include "pvarwidgets.h"
#include "resound_client/performance.h"
#include "amclient_widgets.h"
#include "behaviourview.h"
// -------------------------------- ParameterAddressWidgetBase --------------------
Resound::ParameterAddressWidgetBase::ParameterAddressWidgetBase(wxWindow* parent, int id, ParameterAddress _addr)
		: wxWindow(parent,id)
{
	addr = _addr;
}
Resound::ParameterAddressWidgetBase::~ParameterAddressWidgetBase()
{}

Resound::ParameterAddress Resound::ParameterAddressWidgetBase::ParameterAddressWidgetBase::GetAddress()
{
	return addr;
};

// -------------------------------- PVSSettingsPanel -------------------------

Resound::PVSSettingsPanel::PVSSettingsPanel(wxWindow* parent, ParameterNamespace* _subSystem, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxPanel(parent, id, pos, size, style, name)
{
	subSystem = _subSystem; // store the related sub system

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add(new wxStaticText(this,-1,name));
	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

// -------------------------------- PVSSelectPanel -------------------------
Resound::PVSSelectPanel::PVSSelectPanel(wxWindow* parent, ParameterNamespace* _subSystem, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxPanel(parent, id, pos, size, style, name)
{
	subSystem = _subSystem; // store the related subsystem
}

// -------------------------------- Collective widget --------------------------------
// #include "vumeterwidget.h"
// event table
BEGIN_EVENT_TABLE(Resound::CollectiveWidget, wxPanel)
EVT_BUTTON(PVLW_ASSIGN, Resound::CollectiveWidget::OnAssign)
EVT_CONTEXT_MENU(Resound::CollectiveWidget::OnContextMenu)
EVT_MENU(PVLW_CM_MONITOR, Resound::CollectiveWidget::OnContextMonitor)
EVT_MENU(PVLW_CM_EDIT, Resound::CollectiveWidget::OnContextEdit)
EVT_MENU(PVLW_CM_DISPLAY, Resound::CollectiveWidget::OnContextDisplay)
EVT_MENU(PVLW_CM_UNASSIGN, Resound::CollectiveWidget::OnContextUnassign)
END_EVENT_TABLE()

Resound::CollectiveWidget::CollectiveWidget(wxWindow* parent, int id, wxString _name, Collective* _collective,
                                       bool _showMeters, int _maxMeters)
		: wxPanel(parent,id)
{
	// store passed arguments
	name = _name;
	collective = _collective;
	showMeters = _showMeters;
	maxMeters = _maxMeters;

	SetWindowStyle(wxNO_BORDER | wxCLIP_CHILDREN);
	SetBackgroundColour(wxColour(30,100,150));
	SetForegroundColour(wxColour(255,255,255));

	// construct the sub objects and sizer
	topSizer = new wxBoxSizer( wxVERTICAL );

	// add the collective assigner name // NOT USING THIS FOR NOW
	//  topSizer->Add(new wxStaticText(this,PVLW_LABEL,name, wxPoint(0,0),wxSize(50,20),wxALIGN_CENTRE),wxSizerFlags(0).Align(0).Border(wxALL,0));


	// add the build collective button
	topSizer->Add(new wxButton(this,PVLW_ASSIGN,_T("Assign"), wxPoint(0,0),wxSize(55,30),wxBU_EXACTFIT),wxSizerFlags(0).Align(0).Border(wxALL,0));

	// add the meter bridge sizer
	meterSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(meterSizer);



	// add the assigned collective name label // NOT USING THIS FOR NOW
	//targetLabel = new wxStaticText(this,PVLW_TARGET,collective->GetName(), wxPoint(0,0),wxSize(50,20),wxALIGN_CENTRE);
	//topSizer->Add(targetLabel,wxSizerFlags(0).Align(0).Border(wxALL,0));
	
	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

void Resound::CollectiveWidget::SetCollective(Collective* _coll)
{
	collective = _coll;
	UpdateLink();
}

void Resound::CollectiveWidget::UpdateLink()
{ // update the link
	// targetLabel->SetLabel(collective->GetName());// NOT USING THIS FOR NOW
	meterSizer->Remove(0);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	if(showMeters) {
		for(int n = 0; n < pVarVUMeterWidgetArray.size(); n++) {
			pVarVUMeterWidgetArray[n]->Destroy();
		}

		pVarVUMeterWidgetArray.clear();

		// calculate the number of mini VU-meters to display based on the number of elements
		// in the collective and the maximum number given when the CollectiveWidget was
		// instantiated...
		int numMeters;
		// this could possibly be done more elegantly
		int numElements = collective->get_num_elements();
		if (maxMeters == 0) // ie if we are specifying display as many as possible
		{
			numMeters = numElements;
		} else if ( numElements >= maxMeters) {
			numMeters = maxMeters;
		} else {
			numMeters = numElements;
		}

		// Build the ParameterVUMeterWidget array
		for(int n = 0; n < numMeters; n++) {
			ParameterVUMeterWidget* widget = new ParameterVUMeterWidget(this,-1,0,0,128,_(RESOURCE_DIR "/image/tinyMeterOff.png"), _(RESOURCE_DIR "/image/tinyMeterOn.png"));
			widget->SetTarget((*collective)[n][0].get_target_address()); // assuming always one link per element
			sizer->Add(widget,wxSizerFlags(0).Align(0).Border(wxRIGHT|wxTOP|wxBOTTOM,1));
			pVarVUMeterWidgetArray.push_back(widget);
		}
	}
	meterSizer->Add(sizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
	topSizer->Fit(this);
	wxWindow* parent = GetParent();
	parent->GetSizer()->SetSizeHints(parent);
}

void Resound::CollectiveWidget::Assign()
{
	Resound::Collective newCollective = *collective;
	Resound::CBDialog dlg(this, &newCollective);
	if(dlg.ShowModal() == wxID_OK) {
		*collective = newCollective;
		UpdateLink();
	}
}

void Resound::CollectiveWidget::OnAssign(wxCommandEvent &event)
{
	Assign();
}

void Resound::CollectiveWidget::OnContextMenu(wxContextMenuEvent &event)
{
	wxMenu contextMenu;
	contextMenu.Append(PVLW_CM_EDIT, _T("Assign / Edit Collective"));
	//contextMenu.Append(PVLW_CM_MONITOR, _T("Monitor Collective")); // ( _CM_ in id = context help )
	//contextMenu.Append(PVLW_CM_DISPLAY, _T("Display Collective"));
	contextMenu.Append(PVLW_CM_UNASSIGN, _T("Unassign All"));
	PopupMenu(&contextMenu);
}

void Resound::CollectiveWidget::OnContextMonitor(wxCommandEvent &event)
{
	wxMessageBox(_T("Monitor the collective assigned to this VPC - not implemented yet"));
}

void Resound::CollectiveWidget::OnContextEdit(wxCommandEvent &event)
{
	Assign();
}

void Resound::CollectiveWidget::OnContextDisplay(wxCommandEvent &event)
{
	collective->display();
}

void Resound::CollectiveWidget::OnContextUnassign(wxCommandEvent &event)
{
	wxMessageDialog* confirm = new wxMessageDialog(this,
	                           _T("Are you sure you want to clear all assignments made to this control?"),
	                           _T("Unassign Control"), wxYES_NO | wxNO_DEFAULT);
	if (confirm->ShowModal() == wxID_YES) {
		Resound::Collective newCollective;
		*collective = newCollective;
		UpdateLink();
	}
	confirm->Destroy(); // IS THIS STRICTLY NEEDED?
}

// ----------------------------------- gui CollectiveEditor : wxPanel -------------------------------------

Resound::CollectiveEditor::CollectiveEditor(CollectiveBuilder* _parent, int id, CollectiveCursorEdit* _collectiveWrapper)
		: wxPanel (_parent, id)
{
	SetMinSize(wxSize(100,100));

	parent = _parent;
	collectiveWrapper = _collectiveWrapper; // get the wrapper to edit the appropriate collective with...
	scWin = 0;

	//topSizer = new wxBoxSizer(wxHORIZONTAL); // create top sizer for the collective editor
	wxStaticBox* colBox = new wxStaticBox(this,-1,_T("Parameter Collective"));
	topSizer = new wxStaticBoxSizer(colBox,wxHORIZONTAL); // create top sizer for the collective editor

	UpdateFromCollective();
}

Resound::CollectiveEditor::~CollectiveEditor()
{}

void Resound::CollectiveEditor::UpdateFromCollective()
{
	if (scWin) {
		scWin->Destroy();
		scWin = 0;
	}

	scWin = new wxScrolledWindow(this, -1); // create scrolling window for collective editor

	Collective& col = *collectiveWrapper->get_collective(); // get reference to the collective via wrapper
	int numElements = col.get_num_elements(); // find out how many elements are in that collective.
	int cursorPos = collectiveWrapper->get_cursor_position(); // get the current cursor position within the collective


	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL); // sizer for whole collective visualisation

	/*/////////////////// ONLY FOR DEBUGGING PURPOSES! ////////////////////
	wxString str = wxString::Format(_T("Cursor: %d"),cursorPos);        //
	wxStaticText* displayCursorPos = new wxStaticText(scWin, -1, str);  //
	mainSizer->Add(displayCursorPos, wxSizerFlags(0).Border(wxALL,5));  //
	/////////////////////////////////////////////////////////////////////*/

	// Make collective 'head element' widget:
	// This is a fake element that cannot contain any links;
	// It has an ID of -1, meaning that wxWidgets will assign it a unique ID less than 0...
	// NB:- THIS MIGHT NOT ACTUALLY WANT TO BE A COLLECTIVE ELEMENT WIDGET!!!!
	mainSizer->Add(new CollectiveElementWidget(scWin, -1, cursorPos<0), wxSizerFlags(0).Border(wxRIGHT,10));

	if (numElements == 0) {
		wxStaticText* emptyCollectiveMessage = new wxStaticText(this, -1, _T("No parameters assigned"));
		mainSizer->Add(emptyCollectiveMessage, wxSizerFlags(0).Centre());
	} else {
		for (int r = 0; r < numElements; r++) {
			wxBoxSizer* elementSizer = new wxBoxSizer(wxVERTICAL); // sizer for each element + links
			int numLinks = col[r].get_num_links();
			CollectiveElementWidget* eW = new CollectiveElementWidget(scWin, r, cursorPos==r); // notice ID is r !!
			elementSizer->Add(eW);
			elementSizer->AddSpacer(10);
			for (int c = 0; c < numLinks; c++) {
				// get reference to each link in the element and pass to CollectiveLinkWidget
				wxString linkName(wxConvertMB2WX(col[r][c].get_target_address().get_address().c_str())); // FIXME string conversion
				CollectiveLinkWidget* linkW = new CollectiveLinkWidget(scWin, c, linkName, &col[r][c]); // notice ID is c !!
				elementSizer->Add(linkW, wxSizerFlags(0).Centre());
			}
			mainSizer->Add(elementSizer);
		}
	}

	// for scroll
	// set minimum size to represent real size of window
	// set sizer
	// set virtual size hints
	// layout the sizer

	scWin->SetMinSize(wxSize(400,200)); // this is not good! Scrolling window not working particularly well...
	scWin->SetSizer(mainSizer);
	mainSizer->SetVirtualSizeHints(scWin);
	mainSizer->Layout();

	topSizer->Add(scWin, wxSizerFlags(0).Border(wxALL,5));
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);
	topSizer->Layout();
	scWin->SetScrollbars(20, 20, 50, 50);
}

// ----------------------------------- CollectiveElementWidget -------------------------------------------

// Declare custom events for this class.
DEFINE_EVENT_TYPE(Resound::saEVT_ELEMENT_SELECT)

BEGIN_EVENT_TABLE(Resound::CollectiveElementWidget, wxWindow)
EVT_LEFT_UP(Resound::CollectiveElementWidget::OnLeftMouseUp)
EVT_CONTEXT_MENU(Resound::CollectiveElementWidget::OnContextMenu)
EVT_MENU(CEW_CM_SHIFT_LEFT, Resound::CollectiveElementWidget::OnContextShiftLeft)
EVT_MENU(CEW_CM_SHIFT_RIGHT, Resound::CollectiveElementWidget::OnContextShiftRight)
EVT_MENU(CEW_CM_UNASSIGN, Resound::CollectiveElementWidget::OnContextUnassign)
EVT_MENU(CEW_CM_REMOVE, Resound::CollectiveElementWidget::OnContextRemove)
END_EVENT_TABLE()

Resound::CollectiveElementWidget::CollectiveElementWidget(wxWindow* parent, int id, bool highlight)
		: wxWindow(parent, id, wxDefaultPosition, wxSize(15,15), wxRAISED_BORDER)
{
	if (highlight) {
		SetBackgroundColour(wxColour(255,0,0));
	}
}

Resound::CollectiveElementWidget::~CollectiveElementWidget()
{}

void Resound::CollectiveElementWidget::OnLeftMouseUp(wxMouseEvent& event)
{
	// create and send an saEVT_ELEMENT_SELECT event...
	SendElementSelectedEvent(CEW_CM_LEFT_CLICK); // ... i.e. select the element that was left-clicked
}

void Resound::CollectiveElementWidget::OnContextMenu(wxContextMenuEvent &event)
{
	wxMenu contextMenu;
	if ( event.GetId() >= 0 ) // if it was an actual element right-clicked
	{
		contextMenu.Append(CEW_CM_SHIFT_LEFT, _T("<-- Shift Element Left")); // ( CEW_CM  = CollectiveElementWidget context menu )
		contextMenu.Append(CEW_CM_SHIFT_RIGHT, _T("--> Shift Element Right"));
		//contextMenu.Append(CEW_CM_UNASSIGN, _T("Unassign Parameters from this Element"));
		contextMenu.Append(CEW_CM_REMOVE, _T("Remove Element from Collective"));
	} else // it was the 'master' (head) collective element widget right clicked
	{
		/* TODO (James#1#): 'Head Element' context menu not implemented yet */
		//contextMenu.Append(-1, _T("Remove Links from All Elements"));
		//contextMenu.Append(-1, _T("Remove All Elements from Collective"));
		//contextMenu.Append(-1, _T("Add to Collectives Palette"));
		//contextMenu.Append(-1, _T("-OTHER 'COLLECTIVE' OPTIONS-"));
		//contextMenu.Append(-1, _T("(This menu not implemented yet)"));
	}
	PopupMenu(&contextMenu);
}

void Resound::CollectiveElementWidget::OnContextShiftLeft(wxCommandEvent& event)
{
	SendElementSelectedEvent(CEW_CM_SHIFT_LEFT);
	//wxMessageBox(_T("CollectiveElementWidget::OnContextShiftLeft function. To Do!"));
	// all of these need to call back to the CollectiveBuilder.
	// Suggest using SendElementSelectedEvent, adding an 'action' enumed int,
	// E.g. SendElementSelectedEvent(CEW_CM_SHIFT_LEFT) etc.
	// This would require changes to SendElementSelectedEvent that currently takes no parameter...
}

void Resound::CollectiveElementWidget::OnContextShiftRight(wxCommandEvent& event)
{
	//wxMessageBox(_T("CollectiveElementWidget::OnContextShiftRight function. To Do!"));
	SendElementSelectedEvent(CEW_CM_SHIFT_RIGHT);
}

void Resound::CollectiveElementWidget::OnContextUnassign(wxCommandEvent& event)
{
	wxMessageBox(_T("CollectiveElementWidget::OnContextUnassign function. To Do!"));
	SendElementSelectedEvent(CEW_CM_UNASSIGN);
}

void Resound::CollectiveElementWidget::OnContextRemove(wxCommandEvent& event)
{
	SendElementSelectedEvent(CEW_CM_REMOVE);
}

void Resound::CollectiveElementWidget::SendElementSelectedEvent(CEW_CM action)
{
	int id = GetId(); // get the ID of the CollectiveElementWidget that was clicked.
	wxCommandEvent event( saEVT_ELEMENT_SELECT, id ); // build an event of the custom type with the ID
	event.SetEventObject( this ); // set the object associated with the event to this CollectiveElementWidget
	event.SetInt(action); // bundle the requested action with the event...
	GetEventHandler()->ProcessEvent( event ); // Send the event
}

// ----------------------------------- CollectiveLinkWidget -------------------------------------------

BEGIN_EVENT_TABLE(Resound::CollectiveLinkWidget, wxControl)
EVT_LEFT_DOWN(Resound::CollectiveLinkWidget::OnLeftMouseDown)
EVT_MOTION(Resound::CollectiveLinkWidget::OnLeftMouseMove)
EVT_LEFT_UP(Resound::CollectiveLinkWidget::OnLeftMouseUp)
EVT_PAINT(Resound::CollectiveLinkWidget::OnPaint)
END_EVENT_TABLE()

Resound::CollectiveLinkWidget::CollectiveLinkWidget(wxWindow* parent, int id, wxString linkName, ParameterLink* parameterLink)
		: wxControl(parent, id, wxDefaultPosition, wxSize(75,40), wxSUNKEN_BORDER), // size used to be (15,8)
		m_parameterLink(parameterLink)
{
	SetWindowStyleFlag(wxNO_BORDER);
	wxSize size(80,40);
	SetSize(size);
	SetMinSize(size);
	SetMaxSize(size);
}

Resound::CollectiveLinkWidget::~CollectiveLinkWidget()
{}

void Resound::CollectiveLinkWidget::OnLeftMouseDown(wxMouseEvent& event)
{	
	
	m_lastY = event.GetY();
	CaptureMouse();
	if(m_parameterLink){
		m_lastS = m_parameterLink->get_scaling_factor();
	}
	event.Skip();
}
void Resound::CollectiveLinkWidget::OnLeftMouseMove(wxMouseEvent& event)
{	
	if(m_parameterLink && event.LeftIsDown()){
		
		int dY = event.GetY() - m_lastY;
		float f = m_lastS + float(dY) * -0.01;
		if(f > 2.) f = 2.;
		if(f < -2.) f = -2;
		m_parameterLink->set_scaling_factor(f);
		Refresh();
	}
		
}
void Resound::CollectiveLinkWidget::OnLeftMouseUp(wxMouseEvent& event)
{	
	ReleaseMouse();
}
// paint handling
void Resound::CollectiveLinkWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	if(m_parameterLink){
		wxSize size = GetSize();
		dc.SetBrush(wxBrush(GetBackgroundColour()));
		dc.SetTextForeground(GetForegroundColour());
		dc.DrawRectangle(0,0,size.x,size.y);
		float f = m_parameterLink->get_scaling_factor();
		if(f >= 0){
			dc.SetBrush(wxBrush(wxColour(0,200,0)));
		} else {
			dc.SetBrush(wxBrush(wxColour(200,0,0)));
		}
		dc.DrawRectangle(0,size.y/2,4,-f*10);
		dc.SetFont(*wxSMALL_FONT);
		dc.DrawText(wxConvertMB2WX(m_parameterLink->get_target_address().get_address().c_str()),6,3);
		dc.DrawText(wxString::Format(_("%.2f"),m_parameterLink->get_scaling_factor()),6,20);
	}
}


// ------------------------------------ CBDialog ------------------------------------------
Resound::CBDialog::CBDialog(wxWindow* parent, Collective* _collective)
		: wxDialog(parent,-1,_T("Collective Builder Dialog"), wxDefaultPosition)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new CollectiveBuilder(this, _collective));
	// make the dialog standard controls
	sizer->Add(CreateButtonSizer(wxOK|wxCANCEL), wxSizerFlags(0).Border(wxALL,10).Centre());

	SetSizer(sizer);
	sizer->SetSizeHints(this);
	sizer->Layout();
}

// ------------------------------------ CollectiveBuilder ---------------------------------

// event table
BEGIN_EVENT_TABLE(Resound::CollectiveBuilder, wxPanel)
EVT_BUTTON(CBID_ADD, Resound::CollectiveBuilder::OnAddToPalette)
EVT_BUTTON(CBID_CLEAR, Resound::CollectiveBuilder::OnClearSelection)
EVT_COMMAND  (wxID_ANY, saEVT_PVARADDRESS_SELECT, Resound::CollectiveBuilder::OnAddressSelected)
EVT_COMMAND  (wxID_ANY, saEVT_ELEMENT_SELECT, Resound::CollectiveBuilder::OnElementSelected)
END_EVENT_TABLE()

Resound::CollectiveBuilder::CollectiveBuilder(wxWindow* parent, Collective* _collective)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	collective = _collective; // get the collective to be edited...
	collectiveWrapper = new CollectiveCursorEdit(collective); // ...wrap it up

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

	// construct the sub objects and sizer
	wxBoxSizer *selectionSizer = new wxBoxSizer( wxVERTICAL );

	/* TODO (James#2#): This code can all come out: all of the functionality can
	                    be done by the 'head' CollectiveElementWidget... 
	                    Although some buttons might be useful as well... */
	//wxBoxSizer *selButtonSizer = new wxBoxSizer( wxHORIZONTAL );
	//selButtonSizer->Add(new wxButton(this, CBID_ADD,_T("Add to Palette")));
	//selButtonSizer->Add(new wxButton(this, CBID_CLEAR,_T("Clear Selection")));
	//selectionSizer->Add(selButtonSizer);

	collectiveEditor = new CollectiveEditor(this, -1, collectiveWrapper);
	topSizer->Add(collectiveEditor, wxSizerFlags(0).Border(wxLEFT|wxRIGHT|wxTOP,10));
	//topSizer->Add(selectionSizer, wxSizerFlags(0).Border(wxALL,10));

	// add the notebook
	wxNotebook* noteBook = new wxNotebook(this,-1);

/* //REMOVED DAVE 29/05/07 this is redundant until the redesign of subsystems/namespaces is complete
   // for now I will add the two namespace guis manually
	// add pages to notebooks
	// each page comes from a sub system
	int numSubSystems = RESOUND_NAMESPACE()->get_num_parameter_namespaces();
	for(int n = 0; n < numSubSystems; n++) // for each sub-system
	{
		Resound::ParameterNamespace &system = RESOUND_NAMESPACE()->get_parameter_namespace(n);
//		Resound::PVSSelectPanel* panel = (Resound::PVSSelectPanel*)system.SelectPanel(noteBook);  // FIXME removed panel ownership by PNamespaces FIXED pending testing
		//noteBook->AddPage(panel,system.get_name(),false); // add a notebook page (tab) 
	}
 END REMOVED DAVE 29/05/07*/

	// adding audio matrix manually see above REMOVED DAVE 29/05/07
	Resound::ParameterNamespace& amNamespace = RESOUND_NAMESPACE()->get_parameter_namespace(0);
	wxWindow* panel1 = (Resound::PVSSelectPanel*) new Resound::AudioMatrixSelectPanel(noteBook,&amNamespace);
	noteBook->AddPage(panel1,_T("Matrix"),false);

	// adding behaviour manually see above REMOVED DAVE 29/05/07
	Resound::ParameterNamespace& behaviourNamespace = RESOUND_NAMESPACE()->get_parameter_namespace(1);
	wxWindow* panel2 = new Resound::BehaviourSelectPanel(noteBook,&behaviourNamespace);
	noteBook->AddPage(panel2,_T("Behaviour"),false);


	// add notebook to topsizer
	//wxStaticBox* noteBookBox = new wxStaticBox(this,-1,_T("Add Parameters to Collective"));
	//wxStaticBoxSizer* noteBookSizer = new wxStaticBoxSizer(noteBookBox, wxHORIZONTAL);
	//noteBookSizer->Add(noteBook, wxSizerFlags(0).Expand().Border(wxALL,10));
	//topSizer->Add(noteBookSizer,wxSizerFlags(0).Center().Expand().Border(wxLEFT|wxRIGHT|wxTOP,10));
	topSizer->Add(noteBook,wxSizerFlags(0).Center().Expand().Border(wxLEFT|wxRIGHT|wxTOP,10));

	// layout
	SetSizer(topSizer);
	topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();
}

void Resound::CollectiveBuilder::OnAddToPalette(wxCommandEvent &event)
{
	wxMessageBox(_T("Implement this")); //TODO on add to palette
}


void Resound::CollectiveBuilder::OnClearSelection(wxCommandEvent &event)
{
	/* THIS CAN PROBABLY BE TAKEN OUT IMMEDIATELY...
	for(int n=0;n < selectedWidgets.size(); n++)
	{
	    Resound::AddressSelectWidget* widget = (Resound::AddressSelectWidget*)selectedWidgets[n];
	    widget->SetSelected(false);
	}
	selectedWidgets.clear();
	selection.clear();*/
}

void Resound::CollectiveBuilder::OnAddressSelected(wxCommandEvent &event)
{
	// get the originating object
	Resound::AddressSelectWidget* widget = (Resound::AddressSelectWidget*)event.GetEventObject();
	// add selected link to temporary ParameterLink
	ParameterLink tempLink = ParameterLink(widget->GetAddress());

	int mouseAction = event.GetInt(); // get the mouse action (int) sent inside the saEVT_PVARADDRESS_SELECT event

	// collectiveWrapper is the CollectiveCursorEdit holding the collective being edited.

	switch (mouseAction) {
	case AddressSelectWidget::AS_LEFT_CLICK:
		collectiveWrapper->insert(); // whop a new element in
		collectiveWrapper->add_link(tempLink);
		break;
	case AddressSelectWidget::AS_RIGHT_CLICK:
		collectiveWrapper->add_link(tempLink);
		break;
	}

	collectiveEditor->UpdateFromCollective(); // whenever a new address is selected,
	// call the editor's update function to refresh the display
}

void Resound::CollectiveBuilder::OnElementSelected(wxCommandEvent &event)
{
	collectiveWrapper->set_cursor_position(event.GetId()); // just set cursor position
	int action = event.GetInt(); // get the action to be performed on the collective from the event
	// -- see CollectiveElementWidget::SendElementSelectedEvent(CEW_CM action)

	// Do the requested action
	switch (action) {
	case CollectiveElementWidget::CEW_CM_LEFT_CLICK:
		// Nothing else to do here; cursor position has already been set
		break;
	case CollectiveElementWidget::CEW_CM_SHIFT_LEFT:
		collectiveWrapper->shift_left();
		break;
	case CollectiveElementWidget::CEW_CM_SHIFT_RIGHT:
		collectiveWrapper->shift_right();
		break;
	case CollectiveElementWidget::CEW_CM_UNASSIGN:
		// collectiveWrapper->ClearLinks(); // I HAVEN'T PUT THIS IN CollectiveCursorEdit CLASS YET!!!
		break;
	case CollectiveElementWidget::CEW_CM_REMOVE:
		collectiveWrapper->remove();
		collectiveWrapper->next(); // increment the cursor
		break;
	}
	collectiveEditor->UpdateFromCollective(); // update the editor to reflect changes to the collective
}

// ------------------------------------ AddressSelectWidget --------------------

/// #include <wx/dcbuffer.h>

// implement custom events
DEFINE_EVENT_TYPE(Resound::saEVT_PVARADDRESS_SELECT)

// event table
BEGIN_EVENT_TABLE(Resound::AddressSelectWidget, Resound::ParameterAddressWidgetBase)
EVT_LEFT_UP(Resound::AddressSelectWidget::OnLeftMouseUp)
EVT_RIGHT_UP(Resound::AddressSelectWidget::OnRightMouseUp)
EVT_PAINT(Resound::AddressSelectWidget::OnPaint)
END_EVENT_TABLE()

Resound::AddressSelectWidget::AddressSelectWidget(wxWindow* parent, int id, ParameterAddress _addr)
		: ParameterAddressWidgetBase(parent,id,_addr)
{

	SetWindowStyle(wxNO_BORDER | wxCLIP_CHILDREN);
	wxSize size(35,35);
	SetSize(size);
	SetMinSize(size);
	SetMaxSize(size);

	SetToolTip(wxConvertMB2WX(addr.get_address().c_str())); 
	//name = ParameterNamespaceManager::GetSingleton().GetParameter(addr).get_name(); // FIXME string conversion

	SetBackgroundColour(wxColour(100,100,100)); // off white so you can see it // parent should set color
	SetForegroundColour(wxColour(200,200,200)); // off white so you can see it // parent should set color

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );

	// layout
	SetSizer(topSizer);
	//topSizer->SetSizeHints(this);   // set size hints to honour minimum size
	topSizer->Layout();

}

void Resound::AddressSelectWidget::SetSelected(bool b)
{
	if(b) {
		selected = true;
		oldColour = GetBackgroundColour();
		SetBackgroundColour(wxColour(0,0,0));
	} else {
		selected = false;
		SetBackgroundColour(oldColour);
	}
	Refresh();
}

// this function triggers the custom event:-
void Resound::AddressSelectWidget::SendAddressSelectEvent(AS_MOUSE_ACTION mouseAction)
{
	// build the event
	int id = GetId();
	wxCommandEvent event( saEVT_PVARADDRESS_SELECT, id );
	event.SetEventObject( this );

	event.SetInt(mouseAction); // set event's int to represent the mouse action

	// Send it
	GetEventHandler()->ProcessEvent( event );
}
void Resound::AddressSelectWidget::OnLeftMouseUp(wxMouseEvent& event)
{
	SendAddressSelectEvent (AS_LEFT_CLICK);
	//wxMessageBox(_T("Left Mouse Up"));
}


void Resound::AddressSelectWidget::OnRightMouseUp(wxMouseEvent& event)
{
	SendAddressSelectEvent (AS_RIGHT_CLICK);
	//wxMessageBox(_T("Right Mouse Up"));
}


void Resound::AddressSelectWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	wxSize size = GetSize();
	dc.SetBrush(wxBrush(GetBackgroundColour()));
	dc.SetTextForeground(GetForegroundColour());
	dc.DrawRectangle(0,0,size.x,size.y);
	//dc.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	dc.SetFont(*wxSMALL_FONT);
	dc.DrawText(name,5,5);
}

