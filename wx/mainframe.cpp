#include "mainframe.hpp"
#include <wx/bitmap.h>
#include <wx/artprov.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
    wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    wxImage::AddHandler(new wxGIFHandler);
    wxBitmap axisxbitmap = wxBitmap(wxImage(SRCDIR"/images/tool_axis_x.gif"));
    wxBitmap axisybitmap = wxBitmap(wxImage(SRCDIR"/images/tool_axis_y.gif"));
    wxBitmap axiszbitmap = wxBitmap(wxImage(SRCDIR"/images/tool_axis_z.gif"));
    wxBitmap axisz2bitmap = wxBitmap(wxImage(SRCDIR"/images/tool_axis_z2.gif"));
    wxBitmap axispbitmap = wxBitmap(wxImage(SRCDIR"/images/tool_axis_p.gif"));
    
    wxToolBar *toolbar = CreateToolBar();
    toolbar->SetToolBitmapSize(wxSize(24, 24));
    toolbar->AddTool(wxID_OPEN, wxT("open"),wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(24, 24)));
    toolbar->AddStretchableSpace();
    zviewID = toolbar->AddTool(wxID_ANY, wxEmptyString,axiszbitmap)->GetId();
    z2viewID = toolbar->AddTool(wxID_ANY, wxEmptyString,axisz2bitmap)->GetId();
    xviewID = toolbar->AddTool(wxID_ANY, wxEmptyString,axisxbitmap)->GetId();
    yviewID = toolbar->AddTool(wxID_ANY, wxEmptyString,axisybitmap)->GetId();
    pviewID = toolbar->AddTool(wxID_ANY, wxEmptyString,axispbitmap)->GetId();
    toolbar->Realize();
    
    mainsplitter->SetSashGravity(0);
    mainsplitter->SetMinimumPaneSize(100);
    mainsizer->Add(mainsplitter, 1,wxEXPAND,0);
    
    //left
    wxPanel *leftpanel = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *leftsizer = new wxBoxSizer(wxVERTICAL);
    leftsizer->Add(new SearchResultsList(leftpanel), 1,wxEXPAND,0);
    leftpanel->SetSizer(leftsizer);
    
    //right
    wxPanel *rightpanel = new wxPanel(mainsplitter, wxID_ANY);
    wxBoxSizer *rightsizer = new wxBoxSizer(wxVERTICAL);
    glview = new GLview(rightpanel);
    rightsizer->Add(glview, 1,wxEXPAND,0);
    rightpanel->SetSizer(rightsizer);
    
    mainsplitter->SplitVertically(leftpanel, rightpanel,200);
    this->SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);
    
    Connect(zviewID,wxEVT_COMMAND_TOOL_CLICKED,wxCommandEventHandler(GLview::OnToolbarZ),glview);
    Connect(z2viewID,wxEVT_COMMAND_TOOL_CLICKED,wxCommandEventHandler(GLview::OnToolbarZ2));
    Connect(xviewID,wxEVT_COMMAND_TOOL_CLICKED,wxCommandEventHandler(GLview::OnToolbarX));
    Connect(yviewID,wxEVT_COMMAND_TOOL_CLICKED,wxCommandEventHandler(GLview::OnToolbarY));
    Connect(pviewID,wxEVT_COMMAND_TOOL_CLICKED,wxCommandEventHandler(GLview::OnToolbarP));
}

//Constructor, sets up virtual report list with 3 columns
SearchResultsList::SearchResultsList(wxWindow* parent):
wxListCtrl(parent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_VIRTUAL){
	// Add first column
	wxListItem col0;
	col0.SetId(0);
	col0.SetText( _("Layer") );
	col0.SetWidth(200);
	InsertColumn(0, col0);
    
	//This should reflect your data
	SetItemCount(3);
}

//Overload virtual method of wxListCtrl to provide text data for virtual list
wxString SearchResultsList::OnGetItemText(long item, long column) const{
	//Use item and column to return the correct data for that particular cell. This example just returns "bawls" no matter what
	return wxT("blupp");
}