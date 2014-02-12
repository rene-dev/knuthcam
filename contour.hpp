#ifndef cont_tour_h
#define cont_tour_h

#include <iostream>
#include <string>
#include <vector>
#include <dxflib/dl_dxf.h>
#include <dxflib/dl_creationadapter.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

#include "knuthcam.hpp"

void findcont_tours(drawing_t &d);
void showopen(drawing_t &d);
void showclosed(drawing_t &d);
void showseg_tments(drawing_t &d);
void displaycont_tour(cont_t c);
void offset(layer_t &c,float);
void join(seg_t &s1, seg_t &s2,cont_t &c,glm::vec2,float);
float angle(glm::vec2,glm::vec2);
float angle(glm::vec2 a, glm::vec2 b);
float angle(glm::vec2 a);
bool turn(cont_t);

#endif
