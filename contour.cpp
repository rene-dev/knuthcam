#include "contour.h"

using std::cout;
using std::endl;
using glm::vec2;
using glm::length;
using glm::to_string;

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
					l.conts.push_back(c);
				}else{
					l.openconts.push_back(c);
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

void showopen(drawing_t &d){
	cout << "open" << endl;
	for(layer_t &l : d.layers){
		cout << "layer " << l.name << endl;
		for(cont &c : l.openconts){
			cout << " " << "cont" << endl;
			for(seg &s1 : c.segments){
				cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
			}
		}
	}
}
