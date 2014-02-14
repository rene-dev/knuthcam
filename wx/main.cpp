#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

class SearchResultsList: public wxListCtrl{
	public:
		SearchResultsList(wxWindow* parent);
		wxString OnGetItemText(long item, long column) const;
};
 
//Constructor, sets up virtual report list with 3 columns
SearchResultsList::SearchResultsList(wxWindow* parent):
					wxListCtrl(parent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_VIRTUAL){
	// Add first column        
	wxListItem col0; 
	col0.SetId(0); 
	col0.SetText( _("Track") ); 
	col0.SetWidth(300);
	InsertColumn(0, col0); 
 
	//This should reflect your data
	SetItemCount(3);
}
 
//Overload virtual method of wxListCtrl to provide text data for virtual list
wxString SearchResultsList::OnGetItemText(long item, long column) const{
	//Use item and column to return the correct data for that particular cell. This example just returns "bawls" no matter what
	return wxT("bawls");
}

class glview: public wxGLCanvas {
        void Render();
public:
    glview(wxPanel* parent);
    //easygl renderer;
    void Paintit(wxPaintEvent& event);
    void init();
protected:
    DECLARE_EVENT_TABLE()
};
//      wxSplitterWindow *splittermain = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(100,100),wxSP_LIVE_UPDATE|wxSP_3DSASH);
class MyFrame : public wxFrame
{
  public:
      MyFrame(const wxString& title)
             : wxFrame(NULL, wxID_ANY, title)
      {
            
        wxBoxSizer *sizermain = new wxBoxSizer(wxVERTICAL);
        wxSplitterWindow *splittermain = new wxSplitterWindow(this,wxID_ANY,wxDefaultPosition, wxSize(800,600),wxSP_LIVE_UPDATE|wxSP_3DSASH);
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
        txt2sizer->Add(new glview(pnl2), 1,wxEXPAND,0);
        pnl2->SetSizer(txt2sizer);
 
        splittermain->SplitVertically(pnl1, pnl2,200);
 
        this->SetSizer(sizermain);
        sizermain->SetSizeHints(this);
      }
};
 
BEGIN_EVENT_TABLE(glview, wxGLCanvas)
    EVT_PAINT(glview::Paintit)
END_EVENT_TABLE()
 
glview::glview(wxPanel *parent):wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxSize(100,100), 0, wxT("GLCanvas")){
    init();
    int argc = 1;
    char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
}
 
 
void glview::Paintit(wxPaintEvent& WXUNUSED(event)){
    Render();
}

void glview::init(){
    //renderer.init();
    //renderer.currentPath = gcode("/Users/rene/dev/kinsim/kinsim/kinsim/gcode.ngc");
    //interpol(renderer.currentPath);
    //renderer.robotState = &renderer.currentPath->pos;
}
 
void glview::Render()
{
    SetCurrent();
    wxPaintDC(this);
    //renderer.viewportSize.x = (GLint)GetSize().x;
    //renderer.viewportSize.y = (GLint)GetSize().y;
    //renderer.draw(1);
    glFlush();
    SwapBuffers();
}


class Sim: public wxApp
{
    virtual bool OnInit();
    wxGLCanvas * glcanvas;
};
IMPLEMENT_APP(Sim)


bool Sim::OnInit()
{        
    //wxBoxSizer * box = new wxBoxSizer(wxHORIZONTAL);
    //wxFrame *frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Sim"), wxDefaultPosition, wxSize(800,600));
    MyFrame *frame = new MyFrame(wxT("Splitter-Sizer App"));
    //glcanvas = new glview(frame);
    //wxSplitterWindow *splittermain = new wxSplitterWindow(frame,wxID_ANY);
    //splittermain->Add(glcanvas1,wxEXPAND,0);
    //box->Add(new wxButton(frame, 0, wxT("Hello 1"),wxDefaultPosition, wxDefaultSize, 0),wxSizerFlags(1).Border(wxALL, 3));
    //box->Add(glcanvas,wxSizerFlags(1).Expand().Border(wxALL, 3));
    //frame->SetSizer(box);
    //frame->Layout();

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
