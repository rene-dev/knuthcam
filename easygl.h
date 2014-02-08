//
//  easygl.h
//  kinsim
//
//  Created by Rene, ands on 14/11/13.
//  Copyright (c) 2013 Rene, ands. All rights reserved.
//

#ifndef kinsim_easygl_h
#define kinsim_easygl_h

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

class easygl
{
public:
    glm::vec3 movement;
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::quat orientation;

	glm::ivec2 viewportSize;
	double fieldOfView;
	double near, far;
	double aspectRatio;

	easygl();
	~easygl();

	void init();
	void draw(float period);
    void scroll(double);

private:
    const float speed = 1.0f; // movement speed

	void drawBox(GLdouble width, GLdouble height, GLdouble depth);
	void drawPath();
	void drawAxis();
	void drawGrid();
};

#endif
