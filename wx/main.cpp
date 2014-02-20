#include <wx/wx.h>

#include "mainframe.hpp"
#include "glview.hpp"

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

class Sim: public wxApp
{
    bool OnInit();
};

IMPLEMENT_APP(Sim)

bool Sim::OnInit()
{
    MainFrame *frame = new MainFrame(wxT("KnuthCAM"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuHelp, "&Help" );
    frame->SetMenuBar( menuBar );
    
    frame->CreateStatusBar();
    frame->SetStatusText("Statuskram und so");
 
    frame->Show(TRUE);
    return TRUE;
}
