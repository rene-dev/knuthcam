#ifndef importexport_h
#define importexport_h

#import "knuthcam.h"
#include <string>
#include <iostream>

class importexport{
	public:
		int open(std::string file, drawing_t &d);
		int save(std::string file, drawing_t &d);
};

#endif