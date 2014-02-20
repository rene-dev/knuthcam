#include "mainframe.hpp"
#include <wx/bitmap.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
    wxSplitterWindow *mainsplitter = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    wxImage::AddHandler(new wxGIFHandler);
    wxBitmap openbitmap = wxBitmap(wxImage(SRCDIR"/images/tool_open.gif"));
    
    wxToolBar *toolbar = CreateToolBar();
    toolbar->AddTool(wxID_OPEN, wxEmptyString,openbitmap);
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
    rightsizer->Add(new GLview(rightpanel), 1,wxEXPAND,0);
    rightpanel->SetSizer(rightsizer);
    
    mainsplitter->SplitVertically(leftpanel, rightpanel,200);
    this->SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);
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