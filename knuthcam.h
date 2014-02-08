#ifndef knuthcam_h
#define knuthcam_h

#include <glm/glm.hpp>

using glm::vec2;

typedef struct{
    vec2 start;
    vec2 end;
    vec2 mid;
    double r;
    bool used;
    enum{
        cw,
        ccw,
        line
    }type;
}seg;

typedef struct{
    std::vector<seg> segments;
}cont;

typedef struct{
    string name;
    cont segments;
    std::vector<cont> conts;
    std::vector<cont> openconts;
}layer_t;

typedef struct{
    std::vector<layer_t> layers;
    string name;
}drawing_t;

#endif
