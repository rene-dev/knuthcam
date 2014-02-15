#include "contour.hpp"

using std::cout;
using std::endl;
using glm::vec2;
using glm::length;
using glm::to_string;
using glm::normalize;
using glm::rotate;

//void findcontours(drawing &d){
//	vec2 pos;
//	bool sucess = false;
////	double tolerance = 0.01; // konturfehler
//	for(layer_t &l : d.layers){
//		//cout << "suche layer " << l.name << endl;
//		for(seg_t &s1 : l.segments.segments){
//			if(!s1.used){
//				cont_t c;
//				s1.used = true;
//				c.segments.push_back(s1);
//				//cout << "neu mit:" << to_string(s1.start) << to_string(s1.end) << endl;
//				pos = s1.end;
//				do{
//					sucess = false;
//					for(seg_t &s2 : l.segments.segments){
//						//cout << "checking" << to_string(s2.start) << to_string(s2.end) << endl;
//						if(!s2.used){
//							if(abs(length(pos - s2.start)) <= tolerance){
//								s2.used = true;
//								c.segments.push_back(s2);
//								//cout << "passt!" << to_string(s2.start) << to_string(s2.end) << endl;
//								sucess = true;
//								pos = s2.end;
//							}else if(abs(length(pos - s2.end)) <= tolerance){
//								if(s2.type == seg_t::ccw){
//									s2.type = seg_t::cw;
//								}
//								s2.used = true;
//								vec2 t = s2.start;
//								s2.start = s2.end;
//								s2.end = t;
//								c.segments.push_back(s2);
//								//cout << "hallo2!" << endl;
//								pos = s2.end;
//								sucess = true;
//							}
//						}
//					}
//				}while(sucess);
//				if(abs(length(c.segments.front().start - c.segments.back().end)) <= tolerance){
//                    c.type = cont_t::ccont_t;
//					l.conts.push_back(c);
//				}else{
//                    c.type = cont_t::ocont_t;
//					l.conts.push_back(c);
//				}
//			}
//			//cout << to_string(s.start) << to_string(s.end) << endl;
//		}
//        for(cont_t &c : l.conts){
//            if(turn(c)){
//                for(seg_t &s : c.segments){
//                    vec2 t = s.start;
//                    s.start = s.end;
//                    s.end = t;
//                    if(s.type == seg_t::cw || s.type == seg_t::ccw)
//                        s.type = s.type == seg_t::cw?seg_t::ccw:seg_t::cw;
//                }
//                reverse(c.segments.begin(), c.segments.end());
//            }
//        }
//	}
//}
//
//void showsegments(drawing_t &d){
//	for(layer_t &l : d.layers){
//		cout << "layer " << l.name << endl;
//		for(seg_t &s1 : l.segments.segments){
//			cout << to_string(s1.start) << to_string(s1.end) << endl;
//		}
//	}
//}
//
//void showclosed(drawing_t &d){
//	cout << "closed" << endl;
//	for(layer_t &l : d.layers){
//		cout << "layer " << l.name << endl;
//		for(cont_t &c : l.conts){
//            if(c.type == cont_t::ccont_t){
//                cout << " " << "cont_t" << endl;
//                for(seg_t &s1 : c.segments){
//                    if(s1.type == seg_t::line){
//                        cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
//                    }
//                    else{
//                        cout << "  " << to_string(s1.start) << to_string(((seg_arc*)&s1)->mid) << to_string(s1.end) << endl;
//                    }
//                }
//            }
//		}
//	}
//}
//
//void showopen(drawing_t &d){
//	cout << "open" << endl;
//	for(layer_t &l : d.layers){
//		cout << "layer " << l.name << endl;
//		for(cont_t &c : l.conts){
//            if(c.type == cont_t::ocont_t){
//                cout << " " << "cont_t" << endl;
//                for(seg_t &s1 : c.segments){
//                    if(s1.type == seg_t::line){
//                        cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
//                    }
//                    else{
//                        cout << "  " << to_string(s1.start) << to_string(((seg_arc*)&s1)->mid) << to_string(s1.end) << endl;
//                    }
//                }
//            }
//		}
//	}
//}
//
//bool intersec(seg_t s1, seg_t s2, float &p, float &t){
//	if(s1.type == seg_t::line && s2.type == seg_t::line){// line, line
//		float xy, ab;
//		bool partial = false;
//		double denom = (s2.end.x - s2.end.y) * (s1.start.x - s1.start.y) - (s1.end.x - s1.end.y) * (s2.start.x - s2.start.y);
//		if (denom == 0) {
//			xy = -1;
//			ab = -1;
//		} else {
//			xy = (s2.start.x * (s1.end.y - s2.end.y) + s2.start.y * (s2.end.x - s1.end.y) + s1.start.y * (s2.end.y - s2.end.x)) / denom;
//			partial = xy >= 0 && xy <= 1;
//			if (partial) {
//				// no point calculating this unless xy is between 0 & 1
//				ab = (s1.end.y * (s1.start.x - s2.start.y) + s2.end.y * (s1.start.y - s1.start.x) + s1.end.x * (s2.start.y - s1.start.y)) / denom; 
//			}
//		}
//		if ( partial && (ab >= 0 && ab) <= 1) {
//			ab = 1-ab;
//			xy = 1-xy;
//			p = xy;
//			t = ab;
//			return true;
//		} 
//		return false; 	
//	}
//	else if(s1.type != seg_t::line && s2.type != seg_t::line){// arc, arc
//	
//	}
//	else if(s1.type != seg_t::line){// arc, line 
//		
//	}
//	else{// arc, line
//	
//	}
//	return(false);
//}
//
//float angle(glm::vec2 v1, glm::vec2 v2){
//    float a = atan2(v1.y, v1.x)/M_PI*180.0f;
//    float b = atan2(v2.y, v2.x)/M_PI*180.0f;
//    float c = fmodf(360.0f-(b-a), 360.0f);
//    if(c<0)
//        c+=360;
//    return c;
//}
//
//float angle(glm::vec2 v){
//    float a = atan2(v.y, v.x)/M_PI*180.0f;
//    float foo = fmodf(a, 360.0f);
//    if(foo<0)
//        foo+=360;
//    return foo;
//}
//
//bool turn(cont_t c){
//    float sum = 0;
//    seg_t tmp;
//    tmp.start = c.segments.back().start;
//    tmp.end = c.segments.back().end;
//    for(seg_t &s : c.segments){
//        sum += angle(s.end-s.start,tmp.start-tmp.end)-180;
//        tmp = s;
//    }
//    return sum<0?false:true;
//}
//
void displaycontour(contur* c){
    seg_t* begin = c->curr();
    vec2 p;
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

    }while(c->step() != begin);
}
//
//void join(seg_t &s1, seg_t &s2,cont_t &c,vec2 v,float r){
//    float a;
//    if(s1.end == s2.start){
//        //cout << "passt!" << endl;
//    }else{
//        a = angle(s1.end-v,s2.start-v);
//        if(r>0?a <= 180:a >= 180){
//            //cout << "arc einfügen:" << a << endl;
//            seg_arc newseg;
//            newseg.type = r>0?seg_t::cw:seg_t::ccw;
//            newseg.start = s1.end;
//            newseg.end = s2.start;
//            newseg.mid = v;
//            c.segments.push_back(newseg);
//        }else{
//            seg_t newseg1,newseg2;
//            newseg1.type = seg_t::line;
//            newseg1.start = s1.end;
//            newseg1.end = v;
//            newseg2.type = seg_t::line;
//            newseg2.start = v;
//            newseg2.end = s2.start;
//            c.segments.push_back(newseg1);
//            c.segments.push_back(newseg2);
//            //s1.end = s1.start;
//            //s2.end = s2.start;
//            //cout << "clippen: " << a << endl;
//        }
//    }
//}
//
//void trim(cont_t &c1){
//	float p;
//	float t;
//	for(seg_t &s1 : c1.segments){
//		for(seg_t &s2 : c1.segments){
//			if(!(s1.start == s2.start && s1.end == s2.end)){
//				if(intersec(s1, s2, p, t)){
//					cout << "trim p:" << p << " t:" << t << endl;
//					s1.end = s1.start + p * (s1.end - s1.start);
//					s2.start = s2.start + t * (s2.end - s2.start);
//				}
//			}
//		}
//	}
//}
//
//bool concave(seg_t s1, seg_t s2){
//	vec2 v1, v2;
//	
//	switch(s1.type){
//        case seg_t::misc:
//		case seg_t::line:
//			v1 =  s1.end-s1.start;
//		break;
//		
//		case seg_t::cw:
//			v1 =  rotate(s1.end-((seg_arc*)&s1)->mid, -90.0f);
//		break;
//		
//		case seg_t::ccw:
//			v1 =  rotate(s1.end-((seg_arc*)&s1)->mid, 90.0f);
//		break;
//	}
//	
//	switch(s2.type){
//        case seg_t::misc:
//		case seg_t::line:
//			v2 =  s2.end-s2.start;
//		break;
//		
//		case seg_t::cw:
//			v2 =  rotate(s2.start-((seg_arc*)&s2)->mid, -90.0f);
//		break;
//		
//		case seg_t::ccw:
//			v2 =  rotate(s2.start-((seg_arc*)&s2)->mid, 90.0f);
//		break;
//	}
//	
//	float a = angle(v1, v2);
//	if(a > 0 && a < 180){
//		return(true);
//	}
//	return(false);
//}
//
//void offset(layer_t &l,float r){
//    
//    
//    /*std::vector<cont_t> newconts;
//    for(cont_t &c : l.conts){
//        if(c.type != cont_t::ccont_t)
//            continue;
//        cont_t newcont;
//        newcont.type = cont_t::toolpath;
//        seg_t newseg_t;
//        for(seg_t &s : c.segments){
//            if(s.type == seg_t::line){
//                newseg_t = *new(seg_line);
//                newseg_t.type = s.type;
//                newseg_t.start = s.start+r*normalize(rotate(s.end-s.start, 90.0f));
//                newseg_t.end = s.end+r*normalize(rotate(s.end-s.start, 90.0f));
//            }else if(s.type == seg_t::cw || s.type == seg_t::ccw){
//                newseg_t = *new(seg_arc);
//                newseg_t.type = s.type;
//                newseg_t.start = s.start+normalize(s.start-((seg_arc*)&s)->mid)*(s.type == seg_t::cw?1.0f:-1.0f)*r;
//                newseg_t.end = s.end+normalize(s.end-((seg_arc*)&s)->mid)*(s.type == seg_t::cw?1.0f:-1.0f)*r;
//                ((seg_arc*)&newseg_t)->mid = ((seg_arc*)&s)->mid;
//            }
//            if(!newcont.segments.empty()){
//                seg_t last = newcont.segments.back();
//                join(last, newseg_t, newcont, s.start,r);
//            }
//
//            newcont.segments.push_back(newseg_t);
//        }
//        if(!newcont.segments.empty()){
//            seg_t last = newcont.segments.back();
//            seg_t first = newcont.segments.front();
//            join(last, first, newcont, c.segments.back().end,r);
//            cont_t newcont2;
//            newcont2.type = cont_t::toolpath;
//            //trim(newcont);
//	        //TODO: check closed
//            newconts.push_back(newcont);
//        }
//        
//    }
//    for(cont_t &c : newconts){
//        l.conts.push_back(c);
//    }*/
//    
//    
//}
