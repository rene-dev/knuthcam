#include "glview.hpp"

using std::cout;
using std::endl;

BEGIN_EVENT_TABLE(GLview, wxGLCanvas)
EVT_SIZE(GLview::OnSize)
EVT_PAINT(GLview::OnPaint)
EVT_ERASE_BACKGROUND(GLview::OnEraseBackground)
EVT_LEFT_DOWN(GLview::OnLeftDown)
EVT_LEFT_UP(GLview::OnLeftUp)
EVT_MIDDLE_DOWN(GLview::OnMiddleDown)
EVT_MIDDLE_UP(GLview::OnMiddleUp)
EVT_RIGHT_DOWN(GLview::OnRightDown)
EVT_RIGHT_UP(GLview::OnRightUp)
EVT_MOTION(GLview::OnMotion)
EVT_MOUSEWHEEL(GLview::OnWheel)
END_EVENT_TABLE()

GLview::GLview(wxPanel *parent):wxGLCanvas(parent,wxID_ANY,NULL,wxDefaultPosition,wxDefaultSize,0,wxT("glview"),wxNullPalette){
    context = new wxGLContext(this);
    DxfParser parser;
    
    if(!parser.open(SRCDIR"/../hase.dxf", renderer.d)){
        cout << "cannot open file" << endl;
    }
    renderer.init();
}
GLview::~GLview()
{
}

void GLview::OnLeftDown(wxMouseEvent& e)
{
	//lastx = e.m_x;
	//lasty = e.m_y;
	renderer.drag = true;
}

void GLview::OnLeftUp(wxMouseEvent& e)
{
	renderer.drag = false;
}

void GLview::OnMiddleDown(wxMouseEvent& e)
{
}

void GLview::OnMiddleUp(wxMouseEvent& e)
{
}

void GLview::OnRightDown(wxMouseEvent& e)
{
}

void GLview::OnRightUp(wxMouseEvent& e)
{
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