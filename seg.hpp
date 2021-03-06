#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>
//#include <string>
#include <iostream>
//#include "knuthcam.hpp"

#define tolerance 0.005f // konturfehler

// segment class
class seg_t{
public:
    enum seg_type{
        cw,
        ccw,
        line,
        misc,
        none
    };
    static float angle(glm::vec2, glm::vec2);
    static float angle(glm::vec2);
    static bool near(glm::vec2, glm::vec2);
protected:
    seg_t* l[2];
    glm::vec2 s;
    glm::vec2 e;
    seg_type t;
public:
    bool used;
    seg_t(){
        t = none;
        s = glm::vec2(NAN, NAN);
        e = glm::vec2(NAN, NAN);
        used = false;
        l[0] = this;
        l[1] = this;
    }
    seg_t(glm::vec2 end) : seg_t(){
        this->e = end;
    }
    seg_t(glm::vec2 start, glm::vec2 end) : seg_t(end){
        this->s = start;
    }
    virtual ~seg_t(){}
    glm::vec2 start(){
        return(s);
    }
    glm::vec2 end(){
        return(e);
    }
    seg_type type(){
        return(t);
    }
    void start(glm::vec2 s){
        this->s = s;
    }
    void end(glm::vec2 e){
        this->e = e;
    }
    void type(seg_type t){
        this->t = t;
    }
    seg_t* link(bool n){
        return(l[n]);
    }
    void link(bool n, seg_t* s){
        l[n] = s;
    }
    seg_t* next(){
        return(l[1]);
    }
    void next(seg_t* s){
        l[1] = s;
    }
    seg_t* prev(){
        return(l[0]);
    }
    void prev(seg_t* s){
        l[0] = s;
    }
    
    std::vector<glm::vec2> intersect(seg_t* a){
        std::vector<glm::vec2> result;
        switch(t){
            case line:
                switch(a->type()){
                    case line:
                        glm::vec2 res;
                        float a1 = e.y - s.y;
                        float b1 = s.x - e.x;
                        float c1 = a1 * s.x + b1 * s.y;
                        
                        float a2 = a->end().y - a->start().y;
                        float b2 = a->start().x - a->end().x;
                        float c2 = a2 * a->start().x + b2 * a->start().y;
                        
                        float det = a1 * b2 - a2 * b1;
                        if(fabs(det) < 1.0e-6){
                        }
                        else{ // TODO: on line?
                            res.x = (b2 * c1 - b1 * c2) / det;
                            res.y = (a1 * c2 - a2 * c1) / det;
                            result.push_back(res);
                        }
                        
                        
                        
                        
                }
        }
        return(result);
    }
    
    virtual void show() = 0;
    virtual void reverse() = 0;
    virtual float length() = 0;
    virtual float angle() = 0;
    virtual bool complex() = 0;
    virtual glm::vec2 points(float t) = 0;
    virtual glm::vec2 tans(float t) = 0;
    virtual glm::vec2 start_tan() = 0;
    virtual glm::vec2 end_tan() = 0;
    virtual void destroy() = 0;
    virtual seg_t* offset(float r) = 0;
    virtual glm::vec2 offset_start(float r) = 0;
    virtual glm::vec2 offset_end(float r) = 0;
    virtual glm::vec2 min() = 0;
    virtual glm::vec2 max() = 0;
};

class seg_line: public seg_t{
	public:
		seg_line(glm::vec2 e) : seg_t(e){
			this->t = line;
		}
		seg_line(glm::vec2 s, glm::vec2 e) : seg_t(s, e){
			this->t = line;
		}
    void show(){
        std::cout << "(" << start().x << "/" << start().y << ") -> (" << end().x << "/" << end().y << ") " << std::endl;
    }
    
    void reverse(){
        glm::vec2 tmp;
        tmp = s;
        s = e;
        e = tmp;
    }
    
    float length(){
        return(fabsf(glm::length(e - s)));
    }
    
    bool complex(){
        return(false);
    }
    
    glm::vec2 points(float t){
        if(t < 0.0f){
            t = 0.0f;
        }
        if(t > 1.0f){
            t = 1.0f;
        }
        return(s + t * (e - s));
    }
    
    glm::vec2 tans(float t){
        return(glm::normalize(e - s));
    }
    
    glm::vec2 start_tan(){
        return(tans(0.0f));
    }
    
    glm::vec2 end_tan(){
        return(tans(1.0f));
    }
    
    void destroy(){
        delete(this);
    }
    
    glm::vec2 offset_start(float r){
        return(s+r*glm::normalize(glm::rotate(e-s, 90.0f)));
    }
    
    glm::vec2 offset_end(float r){
        return(e+r*glm::normalize(glm::rotate(e-s, 90.0f)));
    }
    
    seg_t* offset(float r){
        glm::vec2 start = offset_start(r);
        glm::vec2 end = offset_end(r);
        seg_t* newseg = new seg_line(start,end);
        return newseg;
    }
    
    float angle(){
        return(0.0f);
    }
    
    glm::vec2 min(){
        return(glm::vec2(fminf(s.x, e.x), fminf(s.y, e.y)));
    }
    
    glm::vec2 max(){
        return(glm::vec2(fmaxf(s.x, e.x), fmaxf(s.y, e.y)));
    }
};

class seg_arc: public seg_t{
protected:
    glm::vec2 m;
public:
    seg_arc(bool cw_arc, glm::vec2 m, glm::vec2 e) : seg_t(e){
        if(cw_arc){
            this->t = cw;
        }
        else{
            this->t = ccw;
        }
        this->m = m;
    }
    seg_arc(bool cw_arc, glm::vec2 s, glm::vec2 m, glm::vec2 e) : seg_t(s, e){
        if(cw_arc){
            this->t = cw;
        }
        else{
            this->t = ccw;
        }
        this->m = m;
    }
    glm::vec2 mid(){
        return(m);
    }
    void mid(glm::vec2 m){
        this->m = m;
    }
    float angle(){
        float a = seg_t::angle(s - m, e - m);
        if(a < 0.0f){
            a += 360.0f;
        }
        if(t == cw){
            return(a-360.0f);
        }
        if(a < 0.01f){
            return(360.0f);
        }
        return(a);
    }
    void show(){
        if(t == ccw){
            std::cout << "c";
        }
        std::cout << "cw " << angle() << "° ";
        std::cout << "(" << start().x << "/" << start().y << ") -> (" << mid().x << "/" << mid().y << ") -> (" << end().x << "/" << end().y << ") " << std::endl;
    }
    void reverse(){
        glm::vec2 tmp;
        tmp = s;
        s = e;
        e = tmp;
        if(t == cw){
            t = ccw;
        }
        else{
            t = cw;
        }
    }
    
    float length(){
        return(fabsf(glm::length(e - s)));
    }
    
    bool complex(){
        return(true);
    }
    
    glm::vec2 points(float t){
        if(t < 0.0f){
            t = 0.0f;
        }
        if(t > 1.0f){
            t = 1.0f;
        }
        return(m + glm::rotate(s - m, angle() * t));
    }
    
    glm::vec2 tans(float t){
        glm::vec2 p = points(t);
        return(glm::normalize(glm::rotate(p - m, (this->t == ccw)?(90.0f):(-90.0f))));
    }
    
    glm::vec2 start_tan(){
        return(tans(0.0f));
    }
               
    glm::vec2 end_tan(){
        return(tans(1.0f));
    }

    void destroy(){
        delete(this);
    }
    
    glm::vec2 offset_start(float r){
        return(s + glm::normalize(s-m) * (t == cw ? r : -r));
    }
    
    glm::vec2 offset_end(float r){
        return(e + glm::normalize(e-m) * (t == cw ? r : -r));
    }
    
    seg_t* offset(float r){
        glm::vec2 start = offset_start(r);
        glm::vec2 end = offset_end(r);
        seg_t* newseg = 0;
        if(fabsf(glm::length(e-m)) > (t == cw ? -r : r)){
            newseg = new seg_arc(t == seg_t::cw, start, m, end);
        }
//        else {
//            this->next()->link(0, this->prev());
//            this->prev()->link(1, this->next());
//        }
        
        
        
        
//        if(fabsf(glm::length(e-m)) < (t == cw ? -r : r)){
//            seg_t* t1 = new seg_line(start, this->mid());
//            seg_t* t2 = new seg_line(this->mid(), end);
//            t1->link(1, t2);
//            t1->link(0, t2);
//            t2->link(1, t1);
//            t2->link(0, t1);
//            
//            //newseg = t1;//new seg_line(start, end);
//        }
        return(newseg);
    }
    
    glm::vec2 min(){
        glm::vec2 mi = m;
        mi = glm::min(s, mi);
        mi = glm::min(e, mi);
        
        return(glm::vec2(fminf(s.x, e.x), fminf(s.y, e.y)));
    }
    
    glm::vec2 max(){
        return(glm::vec2(fmaxf(s.x, e.x), fmaxf(s.y, e.y)));
    }
};
