#include <iostream>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

#include "DxfParser.hpp"
#include "easygl.hpp"

using std::cout;
using std::endl;
using std::reverse;
using glm::length;
using glm::to_string;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::mat4;
using glm::unProject;

const float sensitivity = 0.01f; // mouse sensitivity

easygl renderer;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
        renderer.drag = true;
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
        renderer.drag = false;
}

static void mousepos_callback(GLFWwindow* window, double xpos, double ypos)
{
    renderer.movemouse(xpos, ypos);
}

static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    renderer.scroll(yoffset);
}

int main(int argc, char *argv[]){
    DxfParser parser;

    if(argc < 2){
        cout << "no file" << endl;
        return 0;
    }
    
    if(!parser.open(argv[1], renderer.d)){
        cout << "cannot open file" << endl;
    }

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

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &renderer.viewportSize.x, &renderer.viewportSize.y);
        renderer.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}