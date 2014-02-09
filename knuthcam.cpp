#include <iostream>
#include <string>
#include <vector>
#include <dxflib/dl_dxf.h>
#include <dxflib/dl_creationadapter.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

#include "importexport.h"
#include "DxfParser.h"
#include "svg.h"
#include "knuthcam.h"
#include "contour.h"
#include "easygl.h"

using std::cout;
using std::endl;
using std::reverse;
using glm::length;
using glm::to_string;

const float sensitivity = 0.01f; // mouse sensitivity

bool w = false, s = false, a = false, d = false, q = false, e = false;
int drag;
glm::ivec2 mouse, lastMouse;
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
	if(drag)
	{
		glm::ivec2 diff = lastMouse - mouse;
		//renderer.orientation = renderer.orientation * glm::quat(glm::vec3(diff.y, diff.x, 0) * sensitivity);
    	renderer.movement -= glm::vec3(diff,0);
        
	}
	lastMouse = mouse;
}

static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    renderer.scroll(yoffset);
}

int main(int argc, char *argv[]){
    DxfParser parser;
    svg svg_backend;
    
    drawing_t d;
    
    if(argc < 2){
        cout << "no file" << endl;
        return 0;
    }
    
    if(!parser.open(argv[1], d)){
        cout << "cannot open file" << endl;
    }
    
    findcontours(d);
    showclosed(d);
    svg_backend.save("test.svg", d);
    cout << "minmax" << to_string(d.min) << " " << to_string(d.min) << endl;
    
    for(layer_t &l : d.layers){
        for(cont &c : l.conts){
            if(turn(c)){
                for(seg &s : c.segments){
                    vec2 t = s.start;
                    s.start = s.end;
                    s.end = t;
                    if(s.type == seg::cw || s.type == seg::ccw)
                        s.type = s.type == seg::cw?seg::ccw:seg::cw;
                }
                reverse(c.segments.begin(), c.segments.end());
            }
        }
        offset(l,1.0f);
        //offset(l,-1.0f);
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

    //renderer.currentPath = gcode("gcode.ngc");
    //interpol(renderer.currentPath);
    //renderer.robotState = &renderer.currentPath->pos;
    renderer.init();

	double time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
    	newTime = glfwGetTime();
    	delta = newTime - time;
    	time = newTime;
        

        glfwGetFramebufferSize(window, &renderer.viewportSize.x, &renderer.viewportSize.y);
        renderer.draw(delta);
        
        glBegin(GL_LINES);
    	for(layer_t &l : d.layers){
            for(cont &c : l.conts){
                switch (c.type) {
                    case cont::toolpath:
                        glColor3f(0, 1, 0);
                        break;
                    case cont::ocont:
                        glColor3f(1, 0, 0);
                        break;
                    case cont::ccont:
                        glColor3f(1, 1, 1);
                        break;
                    default:
                        break;
                }
                displaycontour(c);
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