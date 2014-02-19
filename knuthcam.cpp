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

#include "importexport.hpp"
#include "DxfParser.hpp"
#include "svg.hpp"
#include "knuthcam.hpp"
#include "contour.hpp"
#include "easygl.hpp"

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

float angle2(glm::vec2 v1, glm::vec2 v2){
    float a = atan2(v1.y, v1.x)/M_PI*180.0f;
    float b = atan2(v2.y, v2.x)/M_PI*180.0f;
    float c = b - a;
    if(c > 180.0f){
        c -= 360.0f;
    }
    else if(c < -180.0f){
        c += 360.0f;
    }
    return(c);
}
float angle1(glm::vec2 v){
    float a = atan2(v.y, v.x)/M_PI*180.0f;
    if(a > 180.0f){
        a -= 360.0f;
    }
    else if(a < -180.0f){
        a += 360.0f;
    }
    return(a);
}

bool near(glm::vec2 v1, glm::vec2 v2){
    if(fabsf(length(v1 - v2)) < tolerance){
        return(true);
    }
    return(false);
}

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
		glm::ivec2 diff = lastMouse - mouse;
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
    //renderer.movement += glm::vec3(mouse,0);
    //renderer.movement.x += (float)(mouse-renderer.viewportSize/2).x/renderer.viewportSize.x;
    //renderer.movement.y += (float)(mouse-renderer.viewportSize/2).y/renderer.viewportSize.y;
    //cout << glm::to_string((float)(mouse-renderer.viewportSize/2).x/renderer.viewportSize.x) << endl;
    //renderer.fieldOfView += yoffset;
    //cout << renderer.fieldOfView << endl;
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
    

//    glm::vec2 A = glm::vec2(0,0);
//    glm::vec2 B = glm::vec2(0,100);
//    glm::vec2 C = glm::vec2(100,0);
//    glm::vec2 D = glm::vec2(100,100);
//    glm::vec2 E = glm::vec2(50,50);
//    glm::vec2 F = glm::vec2(30,50);
//    glm::vec2 G = glm::vec2(70,50);
//    glm::vec2 H = glm::vec2(50,30);
//    glm::vec2 I = glm::vec2(50,70);
//
//    layer l1;
//    l1  //<< new seg_line(A, B)
//        //<< new seg_line(D, B)
//        //<< new seg_line(D, C)
//        << new seg_arc(0, F, E, G)
//        << new seg_arc(0, G, E, F);
//    
//        //<< new seg_line(C, A);
//
//    d << l1;
//    d.min = glm::vec2(-10, -10);
//    d.max = glm::vec2(110, 110);
    for(layer &l : d.layers){
        l.findcontours();
        l.offset(1);
        l.show();
    }

    
    //findcontours(d);
    //showclosed(d);
    //showopen(d);
    //svg_backend.save("test.svg", d);
    //cout << "minmax" << to_string(d.min) << " " << to_string(d.max) << endl;
    
    //for(layer_t &l : d.layers){
    //    offset(l,1.0f);
        //offset(l,-1.0f);
    //}
    
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