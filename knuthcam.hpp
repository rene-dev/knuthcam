#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/norm.hpp>
//#include <string>
//#include <iostream>
#include <vector>
#include "seg.hpp"

typedef struct{
    float r;
    enum{
        inside,
        outside
    }operation;
    float rpm;
    float feed;
    float plunge;
    float zstart;
    float zend;
    float zstep;
}operation_t;

class contur{ // ein geschlossener polygonzug, cw = innenkontur
private:
    seg_t* seg;
    unsigned int s;
    bool next_seg;
    
public:
    enum{
        toolpath,
        input
    } ctype;
    contur(){
        seg = 0;
        s = 0;
        next_seg = 1;
    }
    
    seg_t* step(){
        if(seg){
            seg = seg->link(next_seg);
        }
        return(seg);
    }
    
    seg_t* step_back(){
        if(seg){
            seg = seg->link(!next_seg);
        }
        return(seg);
    }
    
    seg_t* curr(){
        return(seg);
    }
    
    seg_t* next(){
        if(seg){
            return(seg->link(next_seg));
        }
        return(seg);
    }
    
    seg_t* prev(){
        if(seg){
            return(seg->link(!next_seg));
        }
        return(seg);
    }
    
    void reverse(){
        next_seg = !next_seg;
        seg_t* begin = curr();
        do{
            curr()->reverse();
            
        }while(step() != begin);
    }
    
    void insert(seg_t* s){ // insert segment after seg
        if(s){
            int i = 0;
            seg_t* tmp = s;
            do{
                i++;
                tmp = tmp->next();
            }while(s != tmp);
            
            if(seg){
                seg_t* st = s;
                seg_t* en = s->link(!next_seg);
                st->link(!next_seg, seg);
                en->link(next_seg, seg->link(next_seg));
                seg->link(next_seg)->link(!next_seg, en);
                seg->link(next_seg, st);
            }
            seg = s;
            this->s += i;
        }
    }
    
    void del(seg_t* s){
        if(s){
            if(curr() == s){
                step_back();
            }
            
            s->link(next_seg)->link(!next_seg, s->link(!next_seg));
            s->link(!next_seg)->link(next_seg, s->link(next_seg));
            
            s->link(next_seg, s);
            s->link(next_seg, s);
            this->s--;
            
            if(s == 0){
                seg = 0;
            }
            s->destroy();
        }
    }
    
    seg_t* cut(seg_t* s){ // s must be in seg
        if(s){
            if(curr() == s){
                step_back();
            }
            
            s->link(next_seg)->link(!next_seg, s->link(!next_seg));
            s->link(!next_seg)->link(next_seg, s->link(next_seg));
            
            s->link(next_seg, s);
            s->link(next_seg, s);
            this->s--;
            
            if(s == 0){
                seg = 0;
            }
        }
        return(s);
    }
    
    contur* cut(seg_t* start, seg_t* end){ // start -> end must be in seg
        seg_t* tmp = start;
        contur* result = new(contur);
        do{
            result->insert(cut(tmp));
            tmp = tmp->link(next_seg);
            
        }while(tmp != end);
        return(result);
    }
    
    contur& operator<<(seg_t* s){
        insert(s);
        return(*this);
    }

    contur& operator>>(seg_t* s){
        del(s);
        return(*this);
    }
    
    seg_t::seg_type type(){
        if(seg){
            return(curr()->type());
        }
        return(seg_t::none);
    }

    glm::vec2 start(){
        if(seg){
            //return(prev()->end());
            return(curr()->start());
        }
        return(glm::vec2(NAN, NAN));
    }
    
    glm::vec2 end(){
        if(seg){
            return(curr()->end());
        }
        return(glm::vec2(NAN, NAN));
    }
    
    float length(){
        return(curr()->length());
    }
    
    bool complex(){
        return(curr()->complex());
    }
    
    glm::vec2 points(float t){
        return(curr()->points(t));
    }
    
    glm::vec2 start_tan(){
        return(curr()->start_tan());
    }
    
    glm::vec2 end_tan(){
        return(curr()->end_tan());
    }
    
    seg_t* offset(float r){
        return(curr()->offset(r));
    };
    
    glm::vec2 mid(){
        if(seg){
            if(seg->type() == seg_t::ccw || seg->type() == seg_t::cw)
            return(((seg_arc*)curr())->mid());
        }
        return(glm::vec2(NAN, NAN));
    }
    
    unsigned int size(){
        return(s);
    }
    
    float angle(){
        return seg_t::angle(curr()->end_tan(), next()->start_tan());
    }
    
    bool concave(){
        if(size()){
            float a = angle();
            if(a > 0 && a < 180.0f){
                return(true);
            }
        }
        return(false);
    }
    
    bool cw(){
        if(size() == 1){
            return(curr()->angle() <= 0);
        }
        float a = 0;
        seg_t* begin = curr();
        do{
            a += angle(); // seg -> seg
            a += seg_t::angle(curr()->start_tan(), curr()->end_tan()); // seg
        }while(step() != begin);
        if(a <= 0){
            return(true);
        }
        return(false);
    }
    
    seg_t* join(float r){
        seg_t* s = 0;
        if(!seg_t::near(curr()->offset_end(r), next()->offset_start(r))){
            if((angle() > 0.0f && r > 0) || (angle() < -0.0f && r < 0)){
                seg_line* l1 = new seg_line(curr()->offset_end(r), curr()->end());
                seg_line* l2 = new seg_line(curr()->end(), next()->offset_start(r));
                l1->link(!next_seg, l2);
                l1->link(next_seg, l2);
                l2->link(!next_seg, l1);
                l2->link(next_seg, l1);
                s = l1;
            }
            else{
                seg_arc* a = new seg_arc(r >= 0, curr()->offset_end(r), curr()->end(), next()->offset_start(r));
                s = a;
            }
        }
        return(s);
    };
    
    bool closed(){
        seg_t* begin = curr();
        if(size()){
            do{
                if(!seg_t::near(curr()->start(), prev()->end())){
                    return(false);
                }
            }while(step() != begin);
        }
        return(true);
    }
    
    void close(){
        seg_t* begin = curr();
        if(size()){
            if(size() == 1){
                if(curr()->type() == seg_t::cw || curr()->type() == seg_t::ccw){
                    if(seg_t::near(curr()->start(), curr()->end())){
                        curr()->end(curr()->start());
                    }
                    else{
                        std::cout << "contour:close: arc not closeable" << std::endl;
                    }
                }
                else{
                    std::cout << "contour:close: single line not closeable" << std::endl;
                }
            }
            do{
                if(seg_t::near(curr()->start(), prev()->end())){
                    curr()->start(prev()->end());
                }
                else{
                    curr()->reverse();
                    if(seg_t::near(curr()->start(), prev()->end())){
                        curr()->start(prev()->end());
                    }
                    else{
                        std::cout << "contour:close: segments not closeable" << std::endl;
                    }
                }
            }while(step() != begin);
        }
    }
    
    void show(){
        seg_t* begin = curr();
        std::cout << std::endl;
        std::cout << "size: " << size() << std::endl;
        std::cout << "closed: " << closed() << std::endl;
        std::cout << "cw: " << cw() << std::endl;
        do{
            curr()->show();
        }while(step() != begin);
        std::cout << std::endl;
    }
};

class layer{
public:
    std::string name;
    std::vector<seg_t *> segments;
    std::vector<contur> conts;
    layer&  operator<<(const contur& c){
        conts.push_back(c);
        return(*this);
    }
    layer& operator<<(seg_t* s){
        segments.push_back(s);
        return(*this);
    }
    
    void offset(float r){
        std::vector<contur> newconts;
        for(contur c : conts){
            contur newcont;
            newcont.ctype = contur::toolpath;
            seg_t* begin = c.curr();
            do{
                newcont << c.offset(r);
                newcont << c.join(r);
                
            }while(c.step() != begin);
            newconts.push_back(newcont);
        }
        for(contur c : newconts){
            conts.push_back(c);
        }
    }
    
    void findcontours(){
        glm::vec2 pos;
        bool sucess = false;
        //cout << "suche layer " << l.name << endl;
        for(seg_t *s1 : segments){
            if(!s1->used){
                contur c;
                c.ctype = contur::input;
                s1->used = true;
                c << s1;
                //cout << "neu mit:" << to_string(s1.start) << to_string(s1.end) << endl;
                pos = s1->end();
                do{
                    sucess = false;
                    for(seg_t *s2 : segments){
                        //cout << "checking" << to_string(s2.start) << to_string(s2.end) << endl;
                        if(!s2->used){
                            if(seg_t::near(pos, s2->start())){
                                s2->used = true;
                                c << s2;
                                //cout << "passt!" << to_string(s2.start) << to_string(s2.end) << endl;
                                sucess = true;
                                pos = s2->end();
                            }else if(seg_t::near(pos, s2->end())){
                                s2->reverse();
                                s2->used = true;
                                c << s2;
                                //cout << "hallo2!" << endl;
                                pos = s2->end();
                                //pos = s2->start();
                                sucess = true;
                            }
                        }
                    }
                }while(sucess);
                //c.closed();
                //c.close();
                //c.reverse();
                if(!c.cw()){
                    c.reverse();
                }
                *this << c;
            }
            //cout << to_string(s.start) << to_string(s.end) << endl;
        }
    }
    
    void show(){
        for(contur c : conts){
            c.show();
        }
    }
};

class drawing{
public:
    std::vector<layer> layers;
    std::string name;
    std::string currentlayer;
    glm::vec2 min;
    glm::vec2 max;
    
    drawing(){
        currentlayer = "0";
    }
    
    void insert(seg_t* seg, std::string layername){
    	for(layer &l : layers){
            if(l.name == layername){
                l << seg;
                return;
            }
        }
    }

    drawing& operator<<(seg_t* s){
        insert(s, currentlayer);
        return(*this);
    }
    
    drawing& operator<<(const std::string& layername){
        currentlayer = layername;
        return(*this);
    }
    
    drawing& operator<<(layer& l){
        layers.push_back(l);
        return(*this);
    }
};
