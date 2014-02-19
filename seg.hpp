#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/norm.hpp>
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
    virtual void show() = 0;
    virtual void reverse() = 0;
    virtual float length() = 0;
    virtual float angle() = 0;
    virtual bool complex() = 0;
    virtual glm::vec2 points(float t) = 0;
    virtual glm::vec2 start_tan() = 0;
    virtual glm::vec2 end_tan() = 0;
    virtual void destroy() = 0;
    virtual seg_t* offset(float r) = 0;
    virtual glm::vec2 offset_start(float r) = 0;
    virtual glm::vec2 offset_end(float r) = 0;
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
        t = fmodf(fabsf(t), 1);
        return(s + t * (e - s));
    }
               
    glm::vec2 start_tan(){
        return(glm::normalize(e - s));
    }
               
    glm::vec2 end_tan(){
        return(glm::normalize(e - s));
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
        
        t = fmodf(fabsf(t), 1);
        return(m + glm::rotate(s - m, angle() * t));
    }
               
    glm::vec2 start_tan(){
        return(glm::normalize(glm::rotate(s - m, (t == ccw)?(90.0f):(-90.0f))));
    }
               
    glm::vec2 end_tan(){
        return(glm::normalize(glm::rotate(e - m, (t == ccw)?(90.0f):(-90.0f))));
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
        else if(fabsf(glm::length(e-m)) < (t == cw ? -r : r)){
            seg_t* t1 = new seg_line(start, this->mid());
            seg_t* t2 = new seg_line(this->mid(), end);
            t1->link(1, t2);
            t1->link(0, t2);
            t2->link(1, t1);
            t2->link(0, t1);
            newseg = t1;//new seg_line(start, end);
        }
        return(newseg);
    }
    

};
