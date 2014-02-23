#pragma once
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "../easygl.hpp"
#include "../dxfparser.hpp"

//#include "mainframe.hpp"

class GLview : public wxGLCanvas
{
public:
    GLview(wxPanel* parent);
    ~GLview();
    
    wxGLContext* context;
    easygl* renderer;
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
    
	void OnLeftDown(wxMouseEvent& e);
	void OnLeftUp(wxMouseEvent& e);
	void OnMiddleDown(wxMouseEvent& e);
	void OnMiddleUp(wxMouseEvent& e);
	void OnRightDown(wxMouseEvent& e);
	void OnRightUp(wxMouseEvent& e);
	void OnMotion(wxMouseEvent& e);
	void OnWheel(wxMouseEvent& e);
    void OnToolbarZ(wxCommandEvent& WXUNUSED(event));
    void OnToolbarZ2(wxCommandEvent& WXUNUSED(event));
    void OnToolbarX(wxCommandEvent& WXUNUSED(event));
    void OnToolbarY(wxCommandEvent& WXUNUSED(event));
    void OnToolbarP(wxCommandEvent& WXUNUSED(event));
    
	void Invalidate();
private:
	DECLARE_EVENT_TABLE()
};