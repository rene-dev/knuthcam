#ifndef knuthcam_h
#define knuthcam_h

#include <glm/glm.hpp>
#include <string>

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
    enum{
        ocont,//offene kontur
        ccont,//geschlossene kontuur
        toolpath
    }type;
}cont;

typedef struct{
    std::string name;
    cont segments;
    std::vector<cont> conts;
}layer_t;

typedef struct{
    std::vector<layer_t> layers;
    std::string name;
    vec2 min;
    vec2 max;
}drawing_t;

#endif
