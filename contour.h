#ifndef contour_h
#define contour_h

#include <iostream>
#include <string>
#include <vector>
#include <dxflib/dl_dxf.h>
#include <dxflib/dl_creationadapter.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "knuthcam.h"

void findcontours(drawing_t &d);
void showopen(drawing_t &d);
void showclosed(drawing_t &d);
void showsegments(drawing_t &d);

#endif
