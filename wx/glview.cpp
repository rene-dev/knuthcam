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

GLview::GLview(wxPanel *parent):wxGLCanvas(parent,wxID_ANY,NULL,wxDefaultPosition,wxDefaultSize,0,wxT("das"),wxNullPalette){
    context = new wxGLContext(this);
}
GLview::~GLview()
{
}

void GLview::OnLeftDown(wxMouseEvent& e)
{
	lastx = e.m_x;
	lasty = e.m_y;
	down = true;
}

void GLview::OnLeftUp(wxMouseEvent& e)
{
	down = false;
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
	if(down)
	{
		float dx = lastx - e.m_x;
		float dy = lasty - e.m_y;
        
		rotX -= dx;
		rotY -= dy;
        
		lastx = e.m_x;
		lasty = e.m_y;
        cout << "motion" << rotX << " " << rotY << endl;
		Invalidate();
	}
}

void GLview::Invalidate()
{
	Refresh(false);
}

void GLview::OnWheel(wxMouseEvent& e)
{
	const float move = (float)e.m_wheelRotation / e.m_wheelDelta;
	distance -= move;
    cout << "scroll " << distance << endl;
	Invalidate();
}


void GLview::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    cout << "paint" << endl;
    SetCurrent(*context);
    wxPaintDC(this);
    //renderer.viewportSize.x = (GLint)GetSize().x;
    //renderer.viewportSize.y = (GLint)GetSize().y;
    //renderer.draw(1);

	// Init OpenGL once, but after SetCurrent
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int width = 0;
	int height = 0;
	GetClientSize(&width, &height);
	if(height==0) height = 1;
	const double aspectRatio = (float) width / height;
	const double fieldOfGLview = 45.0;
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const double zNear = 1.0;
	const double zFar = 1000.0;
	GLfloat aspect = float(width)/float(height);
	GLfloat fH = tan( float(fieldOfGLview / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );
    
	
	//glMatrixMode(GL_MODELGLview);
	glLoadIdentity();
    
	// position GLviewer
	glTranslatef(0.0f, 0.0f, -distance);
	glRotatef(rotY, 1.0f, 0.0f, 0.0f);
	glRotatef(rotX, 0.0f, 1.0f, 0.0f);
    
	/* clear color and depth buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
	glFlush();
	SwapBuffers();
}

void GLview::OnSize(wxSizeEvent& event)
{
	int w, h;
	GetClientSize(&w, &h);
    cout << "resize " << w << " " << h << endl;
    SetCurrent(*context);
    //glGLviewport(0, 0, (GLint) w, (GLint) h);
}

void GLview::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}