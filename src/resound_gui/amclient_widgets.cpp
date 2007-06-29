
#include "amclient_widgets.h"
#include "amclient.h"
#include <sstream>

// ----------------------------------------- AudioMatrixSelectPanel -----------------
Resound::AudioMatrixSelectPanel::AudioMatrixSelectPanel(wxWindow* parent, ParameterNamespace* _subSystem)
		: PVSSelectPanel(parent,_subSystem)
{
	// cast subsystem to amClient - theoretically it can only be one of these
	AMClient* amClient = (AMClient*) subSystem;

	// construct the sub objects and sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxGridSizer* gridSizer = new wxGridSizer(0,amClient->get_num_outputs()+1,1,1);

	wxScrolledWindow *scroll = new wxScrolledWindow(this,-1);



	topSizer->Add(new wxStaticText(this,-1,_("Select panel")));
	for(int r = 0; r <= amClient->get_num_inputs(); r++) {
		for(int c = 0; c <= amClient->get_num_outputs(); c++) {
			std::stringstream s;
			s << "/" << amClient->get_name() <<"/att/" << r << "/" << c; // generate the name
			Resound::AddressSelectWidget *panel = new Resound::AddressSelectWidget(scroll,-1,ParameterAddress(s.str())); // FIXME drastic pvar addressing changes // FIXED, pending testing

			wxColour bkColour;

			if(r == 0 && c == 0) // global
			{
				bkColour = wxColour(50,50,50);
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

			panel->SetBackgroundColour(bkColour);

			gridSizer->Add(panel);
		}
	}

	// for scroll
	// set minimum size to represent real size of window
	// set sizer
	// set virtual size hints
	// layout the sizer

	scroll->SetMinSize(wxSize(400,300));
	scroll->SetSizer(gridSizer);
	gridSizer->SetVirtualSizeHints(scroll);   // set size hints to honour minimum size
	gridSizer->Layout();

	scroll->SetScrollbars(20, 20, 50, 50);

	topSizer->Add(scroll);

	SetSizer(topSizer);
	topSizer->Layout();

}