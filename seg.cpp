#include "seg.hpp"

float seg_t::angle(glm::vec2 v1, glm::vec2 v2){
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
float seg_t::angle(glm::vec2 v){
    float a = atan2(v.y, v.x)/M_PI*180.0f;
    if(a > 180.0f){
        a -= 360.0f;
    }
    else if(a < -180.0f){
        a += 360.0f;
    }
    return(a);
}

bool seg_t::near(glm::vec2 v1, glm::vec2 v2){
    if(fabsf(glm::length(v1 - v2)) < tolerance){
        return(true);
    }
    return(false);
}