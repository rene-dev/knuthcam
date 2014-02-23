#pragma once

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>

#include "glview.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

    int zviewID;
    int z2viewID;
    int xviewID;
    int yviewID;
    int pviewID;
    GLview* glview;
};

class SearchResultsList: public wxListCtrl{
public:
    SearchResultsList(wxWindow* parent);
    wxString OnGetItemText(long item, long column) const;
};