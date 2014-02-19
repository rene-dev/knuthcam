#include <iostream>
#include <string>
#include <vector>
//#include <dxflib/dl_dxf.h>
//#include <dxflib/dl_creationadapter.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

//#include "importexport.hpp"
#include "DxfParser.hpp"
//#include "svg.hpp"
#include "contour.hpp"
#include "easygl.hpp"
#include "knuthcam.hpp"

using std::cout;
using std::endl;
using std::reverse;
using glm::length;
using glm::to_string;

const float sensitivity = 0.01f; // mouse sensitivity

bool w = false, s = false, a = false, d = false, q = false, e = false;
int drag;
glm::ivec2 mouse, lastMouse;
glm::vec2 glmouse, gllastmouse;
glm::mat4 modelview;
GLfloat zbuf = 1;
//glm::mat4 projection;
easygl renderer;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_REPEAT)
		return;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	else if(key == GLFW_KEY_W)
		w = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_S)
		s = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_A)
		a = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_D)
		d = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_Q)
		q = action == GLFW_PRESS ? true : false;
	else if(key == GLFW_KEY_E)
		e = action == GLFW_PRESS ? true : false;

	//renderer.movement.x = d - a;
	//renderer.movement.y = q - e;
	//renderer.movement.z = s - w;
}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
        drag = true;
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
        drag = false;
}

static void mousepos_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse = glm::ivec2(xpos, ypos);
    glm::vec3 screen=glm::vec3(xpos,renderer.viewportSize.y - ypos,zbuf);
    glm::vec3 pos= glm::unProject(screen,modelview,renderer.projection,glm::vec4(0,0, renderer.viewportSize.x, renderer.viewportSize.y));
    glmouse = glm::vec2(pos.x,pos.y);

	if(drag && screen.z != 1)
	{
		//glm::ivec2 diff = lastMouse - mouse;
        glm::vec2 gldiff = (gllastmouse - glmouse);
        //cout << to_string(screen) << endl;
    	renderer.movement -= glm::vec3(gldiff,0);
	}else{
        modelview = renderer.modelview;
        glReadPixels(xpos,renderer.viewportSize.y - ypos,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&zbuf);
    }
    gllastmouse = glmouse;
	lastMouse = mouse;
    
}

static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    glReadPixels(mouse.x,renderer.viewportSize.y - mouse.y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&zbuf);
    renderer.scroll(yoffset);
}

int main(int argc, char *argv[]){
    DxfParser parser;
    //svg svg_backend;
    
    drawing d;
    
    if(argc < 2){
        cout << "no file" << endl;
        return 0;
    }
    
    if(!parser.open(argv[1], d)){
        cout << "cannot open file" << endl;
    }

    for(layer &l : d.layers){
        l.findcontours();
        l.offset(1);
        l.show();
    }
    
    double newTime;
    double delta;
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "KnuthCAM", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window,mousebutton_callback);
    glfwSetCursorPosCallback(window,mousepos_callback);

    renderer.init();

	double time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
    	newTime = glfwGetTime();
    	delta = newTime - time;
    	time = newTime;
        

        glfwGetFramebufferSize(window, &renderer.viewportSize.x, &renderer.viewportSize.y);
        renderer.draw(delta);
        
        glColor3f(0.3f, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f(d.min.x/10, d.min.y/10, -0.01f);
        glVertex3f(d.min.x/10, d.max.y/10, -0.01f);
        glVertex3f(d.max.x/10, d.max.y/10, -0.01f);
        glVertex3f(d.max.x/10, d.min.y/10, -0.01f);
        glEnd();
        
        glBegin(GL_LINES);
    	for(layer &l : d.layers){
            for(contur &c : l.conts){
                if(c.ctype == contur::toolpath || c.ctype == contur::input)
                    displaycontour(&c);
            }
        }
        glEnd();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}