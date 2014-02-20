#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

#include "mainframe.hpp"
#include "glview.hpp"

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

using std::cout;
using std::endl;

class Sim: public wxApp
{
public:
    bool OnInit();
private:
    MainFrame* frame;
    void OnAbout(wxCommandEvent& WXUNUSED(event));
    void OnOpen(wxCommandEvent& WXUNUSED(event));
};

IMPLEMENT_APP(Sim)

bool Sim::OnInit()
{
    frame = new MainFrame(wxT("KnuthCAM"));
    wxMenuBar *menuBar = new wxMenuBar;
    wxMenu *menuFile = new wxMenu;
    menuBar->Append(menuFile, "&File");
    menuFile->Append(wxID_OPEN);
    Connect(wxID_OPEN,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(Sim::OnOpen));
    
    wxMenu *menuHelp = new wxMenu;
    menuBar->Append(menuHelp, "&Help" );
    menuHelp->Append(wxID_ABOUT);
    Connect(wxID_ABOUT,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(Sim::OnAbout));

    frame->SetMenuBar( menuBar );

    frame->CreateStatusBar();
    frame->SetStatusText("Statuskram und so");
 
    frame->Show(TRUE);
    return TRUE;
}

void Sim::OnOpen(wxCommandEvent& WXUNUSED(event)){
    /*
    if (...current content has not been saved...)
    {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
            return;
        //else: proceed asking to the user the new file to open
    }
    */
    wxFileDialog openFileDialog(frame, wxEmptyString, SRCDIR"/..", "","*.dxf", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    cout << "Open " << openFileDialog.GetPath() << endl;
    frame->SetStatusText(openFileDialog.GetPath());
    /*
    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
     */
}

void Sim::OnAbout(wxCommandEvent& WXUNUSED(event)){
    //wxMessageBox(wxT("KnuthCAM"),wxT("This is KnuthCAM"),wxOK|wxICON_INFORMATION);
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName("KnuthCAM");
    aboutInfo.SetVersion("git");
    aboutInfo.SetDescription(_("KnuthCAM"));
    aboutInfo.SetCopyright("(C) 2010-2014");
    aboutInfo.SetWebSite("https://github.com/rene-dev/knuthcam");
    aboutInfo.AddDeveloper("Crinq");
    aboutInfo.AddDeveloper("Rene-dev");
    aboutInfo.AddDeveloper("Ands");
    wxAboutBox(aboutInfo);
}