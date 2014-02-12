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
}seg_t;

typedef struct{
    std::vector<seg_t> segments;
    enum{
        ocont_t,//offene kontur
        ccont_t,//geschlossene kontuur
        toolpath
    }type;
}cont_t;

typedef struct{
    std::string name;
    cont_t seg_tments;
    std::vector<cont_t> conts;
}layer_t;

typedef struct{
    std::vector<layer_t> layers;
    std::string name;
    vec2 min;
    vec2 max;
}drawing_t;

#endif
