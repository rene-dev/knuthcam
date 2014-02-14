#include "svg.hpp"

//int svg::open(string s, drawing_t &d){
//	cout << "svg import not implemented!" << endl;
//	return(0);
//}

/*
int svg::save(string s, drawing_t &d){
	ofstream myfile;
	myfile.open (s);
	myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	myfile << " <svg xmlns=\"http://www.w3.org/2000/svg\"" << endl;
	myfile << "	 version=\"1.1\" baseProfile=\"full\"" << endl;
	//myfile << "  width=\"700px\" height=\"400px\" viewBox=\"0 0 350 200\">" << endl;
	myfile << ">" << endl;
	for(layer_t &l : d.layers){
		//cout << "layer " << l.name << endl;
		for(cont_t &c : l.conts){
			//cout << " " << "cont_t" << endl;
			myfile << " <path d = \"" << endl;
			for(seg_t &s1 : c.segments){
				//cout << "  " << to_string(s1.start) << to_string(s1.end) << endl;
				switch(s1.type){
					case seg_t::line:
						myfile << "  M " << s1.start.x << ", " << s1.start.y << " L " << s1.end.x << ", " << s1.end.y << endl;
					break;
					
					case seg_t::cw:
						myfile << "  M " << s1.start.x << " " << s1.start.y << " A " << glm::length(s1.start - ((seg_arc*)&s1)->mid) << ", " << glm::length(s1.start - ((seg_arc*)&s1)->mid) << " 0 0,0 " << s1.end.x << " " << s1.end.y << endl;
					break;
					
					case seg_t::ccw:
						myfile << "  M " << s1.start.x << " " << s1.start.y << " A " << glm::length(s1.start - ((seg_arc*)&s1)->mid) << ", " << glm::length(s1.start - ((seg_arc*)&s1)->mid) << " 0 0,1 " << s1.end.x << " " << s1.end.y << endl;
					break;
					
					default:
					break;
				}
			}
			myfile << "\" style=\"stroke:#000000; fill:none;\"/>" << endl;

		}
	}

	myfile << "</svg>";
	myfile.close();
	return(0);
}*/