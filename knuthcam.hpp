#ifndef knuthcam_h
#define knuthcam_h

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/norm.hpp>
#include <string>
#include <iostream>

#define tolerance 0.005 // konturfehler
float angle2(glm::vec2 v1, glm::vec2 v2);
float angle1(glm::vec2 v);
bool near(glm::vec2 v1, glm::vec2 v2);

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
        return(fabs(glm::length(e - s)));
    }
    
    bool complex(){
        return(false);
    }
    
    glm::vec2 points(float t){
        t = fmodf(fabs(t), 1);
        return(s + t * (e - s));
    }
               
    glm::vec2 start_tan(){
        return(glm::normalize(e - s)+s);
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
        float a = angle2(s - m, e - m);
        if(a < 0){
            a += 360;
        }
        if(t == cw){
            return(a-360);
        }
        return(a);
        //return(0.0f);
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
        return(fabs(glm::length(e - s)));
    }
    
    bool complex(){
        return(true);
    }
    
    glm::vec2 points(float t){
        
        t = fmodf(fabs(t), 1);
        return(m + glm::rotate(s - m, angle() * t));
    }
               
    glm::vec2 start_tan(){
        return(glm::normalize(glm::rotate(s - m, (t == ccw)?(90.0f):(-90.0f)))+s);
    }
               
    glm::vec2 end_tan(){
        return(glm::normalize(glm::rotate(s - m, (t == cw)?(90.0f):(-90.0f))));
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
        if(abs(glm::length(e-m)) > (t == cw ? r : -r)){
            newseg = new seg_arc(t == seg_t::cw, start, m, end);
        }
        else if(abs(glm::length(e-m)) < (t == cw ? r : -r)){
            newseg = new seg_line(start, end);
        }
        return(newseg);
    }
    

};

class contur{ // ein geschlossener polygonzug, cw = innenkontur
private:
    seg_t* seg;
    unsigned int s;
    bool next_seg;
    
public:
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
        glm::vec2 v1, v2;
        //std::cout << "ang: ("<< start().x << "/" << start().y << ")"<< "/(" << end().x << "/" << end().y << ") -> ("<< next()->start().x << "/" << next()->start().y << ")"<< "/(" << next()->end().x << "/" << next()->end().y << ")";
        if(size() > 1){
            switch(type()){
                case seg_t::misc:
                case seg_t::line:
                    v1 =  end() - start();
                break;
                    
                case seg_t::cw:
                    v1 =  glm::rotate(end() - mid(), -90.0f);
                break;
                    
                case seg_t::ccw:
                    v1 =  glm::rotate(end() - mid(), 90.0f);
                break;
                    
                case seg_t::none:
                    return(NAN);
                break;
            }
            
            switch(next()->type()){
                case seg_t::misc:
                case seg_t::line:
                    v2 =  next()->end() - next()->start();
                break;
                    
                case seg_t::cw:
                    v2 =  glm::rotate(((seg_arc*)next())->start() - ((seg_arc*)next())->mid(), -90.0f);
                break;
                    
                case seg_t::ccw:
                    v2 =  glm::rotate(((seg_arc*)next())->start() - ((seg_arc*)next())->mid(), 90.0f);
                break;
                
                case seg_t::none:
                    return(NAN);
                break;
            }
            
            float a;
            a = angle2(v1, v2);
            
            return(a);
        }
        return(NAN);
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
        float a = 0;
        seg_t* begin = curr();
        do{
            a += angle();
        }while(step() != begin);
        if(a <= 0){
            return(true);
        }
        return(false);
    }
    
    seg_t* join(float r){
        seg_t* s = 0;
        if((angle() > 0.0f && r > 0) || (angle() < 0.0f && r <= 0)){
            seg_line* l1 = new seg_line(curr()->offset_end(r), curr()->end());
            seg_line* l2 = new seg_line(curr()->end(), next()->offset_start(r));
            l1->link(!next_seg, l2);
            l1->link(next_seg, l2);
            l2->link(!next_seg, l1);
            l2->link(next_seg, l1);
            s = l1;
        }
        else if(fabs(angle()) > 0.01f){
            seg_arc* a = new seg_arc(1, curr()->offset_end(r), curr()->end(), next()->offset_start(r));
            s = a;
        }
        return(s);
    };
    
    bool closed(){
        seg_t* begin = curr();
        if(size()){
            do{
                if(!near(curr()->start(), prev()->end())){
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
                    if(near(curr()->start(), curr()->end())){
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
                if(near(curr()->start(), prev()->end())){
                    curr()->start(prev()->end());
                }
                else{
                    curr()->reverse();
                    if(near(curr()->start(), prev()->end())){
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
                s1->used = true;
                c << s1;
                //cout << "neu mit:" << to_string(s1.start) << to_string(s1.end) << endl;
                pos = s1->end();
                do{
                    sucess = false;
                    for(seg_t *s2 : segments){
                        //cout << "checking" << to_string(s2.start) << to_string(s2.end) << endl;
                        if(!s2->used){
                            if(near(pos, s2->start())){
                                s2->used = true;
                                c << s2;
                                //cout << "passt!" << to_string(s2.start) << to_string(s2.end) << endl;
                                sucess = true;
                                pos = s2->end();
                            }else if(near(pos, s2->end())){
                                s2->reverse();
                                s2->used = true;
                                c << s2;
                                //cout << "hallo2!" << endl;
                                pos = s2->end();
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

#endif
