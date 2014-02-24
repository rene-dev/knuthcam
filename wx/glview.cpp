#include "glview.hpp"

using std::cout;
using std::endl;

GLview::GLview(wxPanel *parent):wxGLCanvas(parent,wxID_ANY,NULL,wxDefaultPosition,wxDefaultSize,0,wxT("glview"),wxNullPalette){
    context = new wxGLContext(this);
    DxfParser parser;
    if(!parser.open(SRCDIR"/../hase.dxf", renderer.d)){
        cout << "cannot open file" << endl;
    }
    renderer.init();

    Bind(wxEVT_SIZE, &GLview::OnSize, this);
    Bind(wxEVT_PAINT, &GLview::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &GLview::OnEraseBackground, this);
    Bind(wxEVT_LEFT_DOWN, &GLview::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &GLview::OnLeftUp, this);
    Bind(wxEVT_MIDDLE_DOWN, &GLview::OnMiddleDown, this);
    Bind(wxEVT_MIDDLE_UP, &GLview::OnMiddleUp, this);
    Bind(wxEVT_RIGHT_DOWN, &GLview::OnRightDown, this);
    Bind(wxEVT_RIGHT_UP, &GLview::OnRightUp, this);
    Bind(wxEVT_MOTION, &GLview::OnMotion, this);
    Bind(wxEVT_MOUSEWHEEL, &GLview::OnWheel, this);
}

GLview::~GLview()
{
    delete context;
}

void GLview::OnToolbarZ(wxCommandEvent& WXUNUSED(event))
{
    renderer.viewz();
    Invalidate();
}

void GLview::OnToolbarZ2(wxCommandEvent& WXUNUSED(event))
{
    renderer.viewz2();
    Invalidate();
}

void GLview::OnToolbarX(wxCommandEvent& WXUNUSED(event))
{
    renderer.viewx();
    Invalidate();
}

void GLview::OnToolbarY(wxCommandEvent& WXUNUSED(event))
{
    renderer.viewy();
    Invalidate();
}

void GLview::OnToolbarP(wxCommandEvent& WXUNUSED(event))
{
    renderer.viewp();
    Invalidate();
}

void GLview::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
    renderer.zoomin();
    Invalidate();
}

void GLview::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
    renderer.zoomout();
    Invalidate();
}

void GLview::OnLeftDown(wxMouseEvent& e)
{
	renderer.dragl = true;
}

void GLview::OnLeftUp(wxMouseEvent& e)
{
	renderer.dragl = false;
}

void GLview::OnMiddleDown(wxMouseEvent& e)
{
}

void GLview::OnMiddleUp(wxMouseEvent& e)
{
}

void GLview::OnRightDown(wxMouseEvent& e)
{
    renderer.dragr = true;
}

void GLview::OnRightUp(wxMouseEvent& e)
{
    renderer.dragr = false;
}

void GLview::OnMotion(wxMouseEvent& e)
{
    SetCurrent(*context);
    renderer.movemouse(e.m_x, e.m_y);
    Invalidate();
}

void GLview::Invalidate()
{
	Refresh(false);
}

void GLview::OnWheel(wxMouseEvent& e)
{
	const float move = (float)e.m_wheelRotation / e.m_wheelDelta;
	//distance -= move;
    //cout << "scroll " << distance << endl;
    renderer.scroll(move);
	Invalidate();
}


void GLview::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    //cout << "paint" << endl;
    SetCurrent(*context);
    wxPaintDC(this);
    GetClientSize(&renderer.viewportSize.x, &renderer.viewportSize.y);
    renderer.draw();
    debugtext->SetLabel(glm::to_string(renderer.target));
	glFlush();
	SwapBuffers();
}

void GLview::OnSize(wxSizeEvent& event)
{
	//int w, h;
	//GetClientSize(&w, &h);
    //cout << "resize " << w << " " << h << endl;
    //SetCurrent(*context);
    //glGLviewport(0, 0, (GLint) w, (GLint) h);
}

void GLview::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}