#include "contour.hpp"

using std::cout;
using std::endl;
using glm::vec2;
using glm::vec3;
using glm::length;
using glm::to_string;
using glm::normalize;
using glm::rotate;
using glm::value_ptr;

bool intersec(seg_t* s1, seg_t* s2, float &p, float &t){
	if(s1->type() == seg_t::line && s2->type() == seg_t::line){// line, line
		float xy, ab;
		bool partial = false;
		double denom = (s2->end().x - s2->end().y) * (s1->start().x - s1->start().y) - (s1->end().x - s1->end().y) * (s2->start().x - s2->start().y);
		if (denom == 0) {
			xy = -1;
			ab = -1;
		} else {
			xy = (s2->start().x * (s1->end().y - s2->end().y) + s2->start().y * (s2->end().x - s1->end().y) + s1->start().y * (s2->end().y - s2->end().x)) / denom;
			partial = xy >= 0 && xy <= 1;
			if (partial) {
				// no point calculating this unless xy is between 0 & 1
				ab = (s1->end().y * (s1->start().x - s2->start().y) + s2->end().y * (s1->start().y - s1->start().x) + s1->end().x * (s2->start().y - s1->start().y)) / denom;
			}
		}
		if ( partial && (ab >= 0 && ab) <= 1) {
			ab = 1-ab;
			xy = 1-xy;
			p = xy;
			t = ab;
			return true;
		} 
		return false; 	
	}
	else if(s1->type() != seg_t::line && s2->type() != seg_t::line){// arc, arc
	
	}
	else if(s1->type() != seg_t::line){// arc, line
		
	}
	else{// arc, line
	
	}
	return(false);
}

void displaycontour(contur* c){
    vec3 color[] = {vec3(0,1,0),vec3(1,1,1)};
    seg_t* begin = c->curr();
    vec2 p;
    glColor3fv(value_ptr(color[c->ctype] * 0.5f));
    do{
        
        p = c->start();
        glVertex3f(p.x/10, p.y/10, 0);
        if(c->complex()){
            for(float i = 0.0f; i < 360.0f; i++){
                p = c->points(i / 360.0);
                glVertex3f(p.x/10, p.y/10, 0);
                glVertex3f(p.x/10, p.y/10, 0);
            }
        }
        p = c->end();
        glVertex3f(p.x/10, p.y/10, 0);
        
        glColor3f(1, 0, 0);
        arrow(c->curr()->points(0.0f),c->curr()->tans(0.0f) + c->curr()->points(0.0f));
        arrow(c->curr()->points(0.5f),c->curr()->tans(0.5f) + c->curr()->points(0.5f));
        arrow(c->curr()->points(1.0f),c->curr()->tans(1.0f) + c->curr()->points(1.0f));   
        //glColor3f(0, 1, 0);
        //arrow(c->end(),c->end_tan() + c->end());
        glColor3fv(value_ptr(color[c->ctype]));

    }while(c->step() != begin);
}

void arrow(vec2 s,vec2 e){
    vec2 v1 = 0.5f*rotate(e-s, -45.0f+180.0f)+e;
    vec2 v2 = 0.5f*rotate(e-s, 45.0f+180.0f)+e;
    glVertex3f(s.x/10, s.y/10, 0.01);
    glVertex3f(e.x/10, e.y/10, 0.01);
    
    glVertex3f(v1.x/10, v1.y/10, 0.01);
    glVertex3f(e.x/10, e.y/10, 0.01);
    glVertex3f(v2.x/10, v2.y/10, 0.01);
    glVertex3f(e.x/10, e.y/10, 0.01);
}

