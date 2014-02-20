//
//  easygl.c
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#include "easygl.hpp"

using std::cout;
using std::endl;
using glm::to_string;
using glm::cross;
using glm::quat;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::lookAt;
using glm::perspective;
using glm::value_ptr;

easygl::easygl()
{
}

easygl::~easygl()
{
}

void easygl::init()
{
    target = vec3(0, 1, 0);
    up = vec3(0, 1, 0);
    orientation = quat();

    direction = vec3(0, 0, -1);
    right = cross(up, direction);
    up = cross(direction, right);
    position = vec3(5.0f, 5.0f, 5.0f);
    
	fieldOfView = 60.0f;
	near = 0.1f, far = 1000.0f;
	aspectRatio = 1.0;

	glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LINE_SMOOTH);
    //glLineWidth(2.0f);

    //sphere.load("sphere.stl", vec4(1.0f, 0.0f, 1.0f, 1.0f));
    for(layer &l : d->layers){
        l.findcontours();
        l.offset(1);
        l.show();
    }
}

void easygl::draw()
{
    // update camera
    //vec3 direction = rotate(orientation, vec3(0, 0, -1));
    //vec3 direction = rotate(quat(0,0,0,0), vec3(0, 0, -1));
    position = vec3(5.0f, 5.0f, 5.0f) + (movement.z * direction + movement.x * right - movement.y * up);
    target = position + direction;
    modelview = lookAt(position, target, up);
    projection = perspective(fieldOfView, aspectRatio, near, far);
    
	glViewport(0,0, viewportSize.x, viewportSize.y);
	if(viewportSize.y == 0)
		viewportSize.y = 1;
	aspectRatio = (float)viewportSize.x / (float)viewportSize.y;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(value_ptr(modelview));

    // light
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(vec4(0.6f, 0.7f, 0.7f, 0.0f)));
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(vec4(0.1f, 0.1f, 0.1f, 1.0f)));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(vec4(1.0f, 0.9f, 0.8f, 1.0f)));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	// scene
    //drawGrid();
    //drawRobot();
    drawAxis();
    //drawPath();

    
    glColor3f(0.3f, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(d->min.x/10, d->min.y/10, -0.01f);
    glVertex3f(d->min.x/10, d->max.y/10, -0.01f);
    glVertex3f(d->max.x/10, d->max.y/10, -0.01f);
    glVertex3f(d->max.x/10, d->min.y/10, -0.01f);
    glEnd();
    
    glBegin(GL_LINES);
    for(layer &l : d->layers){
        for(contur &c : l.conts){
            if(c.ctype == contur::toolpath || c.ctype == contur::input)
                displaycontour(&c);
        }
    }
    glEnd();
    
    glFlush();
}

void easygl::scroll(double offset){
    fieldOfView = CLAMP(fieldOfView -= offset,1,150);
    //movement.z += offset;
    /*
    double deltaAperture = offset * -fieldOfView / 200.0;
    fieldOfView += deltaAperture;
    if (fieldOfView < 50) // do not let aperture <= 0.1
        fieldOfView = 50;
    if (fieldOfView > 150) // do not let aperture >= 180
        fieldOfView = 150;
    */
}

void easygl::drawBox(GLdouble width, GLdouble height, GLdouble depth)
{
	const GLfloat vertices[8][3] = {
		{1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
		{1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };
	const GLfloat normals[6][3] = {
		{0.0, 0.0, 1.0}, {-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
		{0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}};
	const short faces[6][4] = {
		{3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, value_ptr(vec4(0.5f, 0.5f, 1.0f, 1.0f)));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, value_ptr(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2.0f);

    glPushMatrix();
    glTranslatef(0, height/2, 0);
    glScalef(width/2, height/2, depth/2);
    glBegin(GL_QUADS);
    for(int f = 0; f < 6; f++)
	{
		glNormal3fv(normals[f]);
        for(int i = 0; i < 4; i++)
            glVertex3fv(vertices[faces[f][i]]);
	}
    glEnd ();
    glPopMatrix();

    glDisable(GL_LIGHTING);
}

void easygl::drawAxis()
{
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();
}

void easygl::drawGrid()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (GLfloat i = -2; i <= 2; i += 1)
    {
        glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
        glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();
}
