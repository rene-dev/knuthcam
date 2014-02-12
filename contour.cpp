#include "contour.hpp"

using std::cout;
using std::endl;
using glm::vec2;
using glm::length;
using glm::to_string;
using glm::normalize;
using glm::rotate;

void findcont_tours(drawing_t &d){
	vec2 pos;
	bool sucess = false;
	double tolerance = 0.01; // konturfehler
	for(layer_t &l : d.layers){
		//cout << "suche layer " << l.name << endl;
		for(seg_t &s1 : l.seg_tments.segments){
			if(!s1.used){
				cont_t c;
				s1.used = true;
				c.segments.push_back(s1);
				//cout << "neu mit:" << to_string(s1.start) << to_string(s1.end) << endl;
				pos = s1.end;
				do{
					sucess = false;
					for(seg_t &s2 : l.seg_tments.segments){
						//cout << "checking" << to_string(s2.start) << to_string(s2.end) << endl;
						if(!s2.used){
							if(abs(length(pos - s2.start)) <= tolerance){
								s2.used = true;
								c.segments.push_back(s2);
								//cout << "passt!" << to_string(s2.start) << to_string(s2.end) << endl;
								sucess = true;
								pos = s2.end;
							}else if(abs(length(pos - s2.end)) <= tolerance){
								if(s2.type == seg_t::ccw){
									s2.type = seg_t::cw;
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
                    c.type = cont_t::ccont_t;
					l.conts.push_back(c);
				}else{
                    c.type = cont_t::ocont_t;
					l.conts.push_back(c);
				}
			}
			//cout << to_string(s.start) << to_string(s.end) << endl;
		}
	}
}

void showseg_tments(drawing_t &d){
	for(layer_t &l : d.layers){
		cout << "layer " << l.name << endl;
		for(seg_t &s1 : l.seg_tments.segments){
			cout << to_string(s1.start) << to_string(s1.end) << endl;
		}
	}
}

void showclosed(drawing_t &d){
	cout << "closed" << endl;
	for(layer_t &l : d.layers){
		cout << "layer " << l.name << endl;
		for(cont_t &c : l.conts){
			cout << " " << "cont_t" << endl;
			for(seg_t &s1 : c.segments){
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

bool turn(cont_t c){
    float sum = 0;
    seg_t tmp;
    tmp.start = c.segments.back().start;
    tmp.end = c.segments.back().end;
    for(seg_t &s : c.segments){
        sum += angle(s.end-s.start,tmp.start-tmp.end)-180;
        tmp = s;
    }
    return sum<0?false:true;
}

void displaycont_tour(cont_t c){
    for(seg_t &s : c.segments){
        //cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
        if(s.type == seg_t::line){
            glVertex3f(s.start.x/10, s.start.y/10, 0);
            glVertex3f(s.end.x/10, s.end.y/10, 0);
        }else if(s.type == seg_t::cw || s.type == seg_t::ccw){
            glVertex3f(s.start.x/10, s.start.y/10, 0);
            float a = angle(s.end-s.mid,s.start-s.mid);
            float step = 1.0f;
            vec2 arc = s.start;
            if(s.type == seg_t::cw){
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

void join(seg_t &s1, seg_t &s2,cont_t &c,vec2 v,float r){
    float a;
    if(s1.end == s2.start){
        cout << "passt!" << endl;
    }else{
        a = angle(s1.end-v,s2.start-v);
        if(r>0?a < 180:a > 180){
            cout << "arc einfügen:" << a << endl;
            seg_t newseg_t;
            newseg_t.type = r>0?seg_t::cw:seg_t::ccw;
            newseg_t.start = s1.end;
            newseg_t.end = s2.start;
            newseg_t.mid = v;
            c.segments.push_back(newseg_t);
        }else{
            //s1.end = s1.start;
            //s2.end = s2.start;
            cout << "clippen: " << a << endl;
        }
    }
}

void offset(layer_t &l,float r){
    std::vector<cont_t> newcont_ts;
    for(cont_t &c : l.conts){
        if(c.type != cont_t::ccont_t)
            continue;
        cont_t newcont_t;
        newcont_t.type = cont_t::toolpath;
        for(seg_t &s : c.segments){
            seg_t newseg_t;
            newseg_t.type = s.type;
            if(s.type == seg_t::line){
                newseg_t.start = s.start+r*normalize(rotate(s.end-s.start, 90.0f));
                newseg_t.end = s.end+r*normalize(rotate(s.end-s.start, 90.0f));
            }else if(s.type == seg_t::cw || s.type == seg_t::ccw){
                newseg_t.start = s.start+normalize(s.start-s.mid)*(s.type == seg_t::cw?1.0f:-1.0f)*r;
                newseg_t.end = s.end+normalize(s.end-s.mid)*(s.type == seg_t::cw?1.0f:-1.0f)*r;
                newseg_t.mid = s.mid;
            }
            if(!newcont_t.segments.empty()){
                seg_t last = newcont_t.segments.back();
                join(last, newseg_t, newcont_t, s.start,r);
            }

            newcont_t.segments.push_back(newseg_t);
        }
        if(!newcont_t.segments.empty()){
            seg_t last = newcont_t.segments.back();
            seg_t first = newcont_t.segments.front();
            join(last, first, newcont_t, c.segments.back().end,r);
        }
        
        //TODO: check closed
        newcont_ts.push_back(newcont_t);
    }
    for(cont_t &c : newcont_ts){
        l.conts.push_back(c);
    }
}
