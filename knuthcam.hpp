#ifndef knuthcam_h
#define knuthcam_h

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>
#include <iostream>

#define tolerance 0.001 // konturfehler
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
    
    virtual void show() = 0;
    
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
    virtual void reverse() = 0;
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
    void show(){
        if(t == ccw){
            std::cout << "c";
        }
        std::cout << "cw";
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
};

class contur{ // ein geschlossener polygonzug, cw = innenkontur
private:
    seg_t* seg;
    unsigned int size;
    bool next_seg;
    
public:
    contur(){
        seg = 0;
        size = 0;
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
            if(seg){
                s->link(!next_seg, seg);
                s->link(next_seg, seg->link(next_seg));
                seg->link(next_seg)->link(!next_seg, s);
                seg->link(next_seg, s);
            }
            seg = s;
            size++;
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
            size--;
            
            if(size == 0){
                seg = 0;
            }
            delete(s);
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
            size--;
            
            if(size == 0){
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
    
//    contur& operator<<(seg_t s){
//        seg_t* s1 = 0;
//        switch(s.type()){
//            case seg_t::line:
//                s1 = new seg_line(s.start(), s.end());
//            break;
//            case seg_t::cw:
//                s1 = new seg_cw_arc(s.start(), ((seg_arc*)&s)->mid(), s.end());
//            break;
//            case seg_t::ccw:
//                s1 = new seg_ccw_arc(s.start(), ((seg_arc*)&s)->mid(), s.end());
//            break;
//            case seg_t::misc:
//            break;
//            case seg_t::none:
//            break;
//        }
//        insert(s1);
//        return(*this);
//    }
    
    contur& operator>>(seg_t* s){
        cut(s);
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
            return(prev()->end());
        }
        return(glm::vec2(NAN, NAN));
    }
    
    glm::vec2 end(){
        if(seg){
            return(curr()->end());
        }
        return(glm::vec2(NAN, NAN));
    }
    
    glm::vec2 mid(){
        if(seg){
            if(seg->type() == seg_t::ccw || seg->type() == seg_t::cw)
            return(((seg_arc*)curr())->mid());
        }
        return(glm::vec2(NAN, NAN));
    }
    
    unsigned int length(){
        return(size);
    }
    
    float angle(){
        glm::vec2 v1, v2;
        //std::cout << "ang: ("<< start().x << "/" << start().y << ")"<< "/(" << end().x << "/" << end().y << ") -> ("<< next()->start().x << "/" << next()->start().y << ")"<< "/(" << next()->end().x << "/" << next()->end().y << ")";
        if(size > 1){
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
                    v2 =  next()->end() - end();
                break;
                    
                case seg_t::cw:
                    v2 =  glm::rotate(end() - ((seg_arc*)next())->mid(), -90.0f);
                break;
                    
                case seg_t::ccw:
                    v2 =  glm::rotate(end() - ((seg_arc*)next())->mid(), 90.0f);
                break;
                
                case seg_t::none:
                    return(NAN);
                break;
            }
            
            float a;
            a = angle2(v1, v2);

            //std::cout << " = " << a << std::endl;
            return(a);
        }
        return(NAN);
    }
    
    bool concave(){
        if(size){
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
    
    void show(){
        seg_t* begin = curr();
        std::cout << std::endl;
        std::cout << "size: " << length() << std::endl;
        do{
            curr()->show();
        }while(step() != begin);
        std::cout << std::endl;
    }
    
    void close(){
        seg_t* begin = curr();
        if(size){
            if(size == 1){
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
    glm::vec2 min;
    glm::vec2 max;
    drawing& operator<<(const layer& l){
        layers.push_back(l);
        return(*this);
    }
};

#endif
