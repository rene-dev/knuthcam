#include "mainframe.hpp"

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title){
    wxBoxSizer *sizermain = new wxBoxSizer(wxVERTICAL);
    wxSplitterWindow *splittermain = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(1024,768),wxSP_LIVE_UPDATE|wxSP_3DSASH);
    splittermain->SetSashGravity(0);
    splittermain->SetMinimumPaneSize(100); // Smalest size the
    sizermain->Add(splittermain, 1,wxEXPAND,0);
    
    wxPanel *pnl1 = new wxPanel(splittermain, wxID_ANY);
    
    wxBoxSizer *txt1sizer = new wxBoxSizer(wxVERTICAL);
    //wxTextCtrl *txt1 = new wxTextCtrl(pnl1, wxID_ANY);
    //txt1sizer->Add(txt1, 1,wxEXPAND,0);
    txt1sizer->Add(new SearchResultsList(pnl1), 1,wxEXPAND,0);
    pnl1->SetSizer(txt1sizer);
    
    
    wxPanel *pnl2 = new wxPanel(splittermain, wxID_ANY);
    wxBoxSizer *txt2sizer = new wxBoxSizer(wxVERTICAL);
    txt2sizer->Add(new GLview(pnl2), 1,wxEXPAND,0);
    pnl2->SetSizer(txt2sizer);
    
    splittermain->SplitVertically(pnl1, pnl2,200);
    
    this->SetSizer(sizermain);
    sizermain->SetSizeHints(this);
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