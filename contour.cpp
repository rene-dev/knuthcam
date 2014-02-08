#include "contour.h"

using std::cout;
using std::endl;
using glm::vec2;
using glm::length;
using glm::to_string;
using glm::normalize;
using glm::rotate;

void findcontours(drawing_t &d){
	vec2 pos;
	bool sucess = false;
	double tolerance = 0.01; // konturfehler
	for(layer_t &l : d.layers){
		//cout << "suche layer " << l.name << endl;
		for(seg &s1 : l.segments.segments){
			if(!s1.used){
				cont c;
				s1.used = true;
				c.segments.push_back(s1);
				//cout << "neu mit:" << to_string(s1.start) << to_string(s1.end) << endl;
				pos = s1.end;
				do{
					sucess = false;
					for(seg &s2 : l.segments.segments){
						//cout << "checking" << to_string(s2.start) << to_string(s2.end) << endl;
						if(!s2.used){
							if(abs(length(pos - s2.start)) <= tolerance){
								s2.used = true;
								c.segments.push_back(s2);
								//cout << "passt!" << to_string(s2.start) << to_string(s2.end) << endl;
								sucess = true;
								pos = s2.end;
							}else if(abs(length(pos - s2.end)) <= tolerance){
								if(s2.type == seg::ccw){
									s2.type = seg::cw;
								}
								s2.used = true;
								vec2 t = s2.start;
								s2.start = s2.end;
								s2.end = t;
								c.segments.push_back(s2);
								//cout << "hallo2!" << endl;
								pos = s2.end;
								sucess = true;
							}
						}
					}
				}while(sucess);
				if(abs(length(c.segments.front().start - c.segments.back().end)) <= tolerance){
                    c.type = cont::ccont;
					l.conts.push_back(c);
				}else{
                    c.type = cont::ocont;
					l.conts.push_back(c);
				}
			}
			//cout << to_string(s.start) << to_string(s.end) << endl;
		}
	}
}

void showsegments(drawing_t &d){
	for(layer_t &l : d.layers){
		cout << "layer " << l.name << endl;
		for(seg &s1 : l.segments.segments){
			cout << to_string(s1.start) << to_string(s1.end) << endl;
		}
	}
}

void showclosed(drawing_t &d){
	cout << "closed" << endl;
	for(layer_t &l : d.layers){
		cout << "layer " << l.name << endl;
		for(cont &c : l.conts){
			cout << " " << "cont" << endl;
			for(seg &s1 : c.segments){
				cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
			}
		}
	}
}

void displaycontour(cont c){
    for(seg &s : c.segments){
        //cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
        if(s.type == seg::line){
            glVertex3f(s.start.x/10, s.start.y/10, 0);
            glVertex3f(s.end.x/10, s.end.y/10, 0);
        }else if(s.type == seg::cw || s.type == seg::ccw){
            float angle = glm::angle(normalize(s.end-s.mid),normalize(s.start-s.mid));
            float step = 2.0f;
            vec2 arc = s.start;
            if(s.type == seg::cw)
                step*=-1;
                glVertex3f(s.start.x/10, s.start.y/10, 0);
                for(float i = fabs(step); i<angle;i+=fabs(step)){
                    arc = glm::rotate(arc-s.mid,step);
                    arc+=s.mid;
                    glVertex3f(arc.x/10, arc.y/10, 0);
                    glVertex3f(arc.x/10, arc.y/10, 0);
                }
            glVertex3f(s.end.x/10, s.end.y/10, 0);
        }
    }
}

void offset(layer_t &l){
    bool right = true;
    std::vector<cont> newconts;
    for(cont &c : l.conts){
        cont newcont;
        newcont.type = cont::toolpath;
        for(seg &s : c.segments){
            seg newseg;
            newseg.type = s.type;
            newseg.start = s.start+normalize(rotate(s.end-s.start, right?-90.0f:90.0f));
            newseg.end = s.end+normalize(rotate(s.end-s.start, right?-90.0f:90.0f));
            newseg.mid = s.mid+normalize(rotate(s.end-s.start, right?-90.0f:90.0f));
            newcont.segments.push_back(newseg);
        }
        newconts.push_back(newcont);
    }
    for(cont &c : newconts){
        l.conts.push_back(c);
    }
}
