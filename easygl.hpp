//
//  easygl.h
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//
#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>
#include "knuthcam.hpp"
#include "contour.hpp"

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

class easygl
{
public:
    glm::vec3 movement;
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
    glm::vec3 direction;
    glm::vec3 right;
	glm::quat orientation;
    glm::mat4 modelview;
    glm::mat4 projection;

	glm::ivec2 viewportSize;
	double fieldOfView;
	double near, far;
	double aspectRatio;
    bool dragl;
    bool dragr;
    glm::mat4 dragmodelview;
    GLfloat zbuf = 1;
    drawing d;
    glm::ivec2 mouse, lastMouse;
    glm::vec2 glmouse, gllastmouse;

	easygl();
	~easygl();

	void init();
	void draw();
    void scroll(double);
    void movemouse(double, double);
    void viewz();
    void viewz2();
    void viewx();
    void viewy();
    void viewp();
    void zoomin();
    void zoomout();
private:
    const float speed = 1.0f; // movement speed

	void drawBox(GLdouble width, GLdouble height, GLdouble depth);
	void drawPath();
	void drawAxis();
	void drawGrid();
};
