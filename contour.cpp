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

float angle(glm::vec2 v1, glm::vec2 v2){
    float a = atan2(v1.y, v1.x)/M_PI*180.0f;
    float b = atan2(v2.y, v2.x)/M_PI*180.0f;
    float c = fmodf(360.0f-(b-a), 360.0f);
    if(c<0)
        c+=360;
    return c;
}

float angle(glm::vec2 v){
    float a = atan2(v.y, v.x)/M_PI*180.0f;
    float foo = fmodf(a, 360.0f);
    if(foo<0)
        foo+=360;
    return foo;
}

bool turn(cont c){
    float sum = 0;
    seg tmp;
    tmp.start = c.segments.front().start;
    tmp.end = c.segments.front().end;
    for(seg &s : c.segments){
        sum += angle(s.end-s.start,tmp.end-tmp.start)-180;
        tmp = s;
    }
    return sum<0?true:false;
}

void displaycontour(cont c){
    for(seg &s : c.segments){
        //cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
        if(s.type == seg::line){
            glVertex3f(s.start.x/10, s.start.y/10, 0);
            glVertex3f(s.end.x/10, s.end.y/10, 0);
        }else if(s.type == seg::cw || s.type == seg::ccw){
            glVertex3f(s.start.x/10, s.start.y/10, 0);
            float a = angle(s.end-s.mid,s.start-s.mid);
            float step = 1.0f;
            vec2 arc = s.start;
            if(s.type == seg::cw){
                step*=-1;
                a = 360-a;
            }
            for(float i = fabs(step); i<a;i+=fabs(step)){
                arc = glm::rotate(arc-s.mid,step);
                arc+=s.mid;
                glVertex3f(arc.x/10, arc.y/10, 0);
                glVertex3f(arc.x/10, arc.y/10, 0);
            }
            glVertex3f(s.end.x/10, s.end.y/10, 0);
        }
    }
}

glm::vec2 tangent(seg s){
    vec2 v;
    if(s.type == seg::cw)
        v = glm::rotate(s.mid-s.start,90.0f);
    if(s.type == seg::ccw)
        v = glm::rotate(s.mid-s.start,-90.0f);
    if(s.type == seg::line)
        v = s.end-s.start;
    return glm::normalize(v);
}

void join(seg &s1, seg &s2,cont &c,vec2 v){
    float a = angle(tangent(s2),tangent(s1));
    if(s1.end == s2.start){
        cout << "passt!" << a << endl;
    }else if(a >= 180){
        cout << "arc einfügen:" << a << endl;
        seg newseg;
        newseg.type = seg::cw;
        newseg.start = s1.end;
        newseg.end = s2.start;
        newseg.mid = v;
        c.segments.push_back(newseg);
    }else{
        //s1.end = s1.start;
        //s2.end = s2.start;
        cout << "clippen: " << a << endl;
    }
}

void offset(layer_t &l,float r){
    std::vector<cont> newconts;
    for(cont &c : l.conts){
        if(c.type != cont::ccont)
            continue;
        cont newcont;
        newcont.type = cont::toolpath;
        for(seg &s : c.segments){
            seg newseg;
            seg newarc;
            newseg.type = s.type;
            if(s.type == seg::line){
                newseg.start = s.start+r*normalize(rotate(s.end-s.start, 90.0f));
                newseg.end = s.end+r*normalize(rotate(s.end-s.start, 90.0f));
            }else if(s.type == seg::cw || s.type == seg::ccw){
                newseg.start = s.start+normalize(s.start-s.mid)*(s.type == seg::cw?1.0f:-1.0f)*r;
                newseg.end = s.end+normalize(s.end-s.mid)*(s.type == seg::cw?1.0f:-1.0f)*r;
                newseg.mid = s.mid;
            }
            if(!newcont.segments.empty()){
                seg last = newcont.segments.back();
                join(last, newseg, newcont, s.start);
            }

            newcont.segments.push_back(newseg);
        }
        if(!newcont.segments.empty()){
            seg last = newcont.segments.back();
            seg first = newcont.segments.front();
            join(last, first, newcont, c.segments.back().start);
        }
        
        //TODO: check closed
        newconts.push_back(newcont);
    }
    for(cont &c : newconts){
        l.conts.push_back(c);
    }
}
