#ifndef importexport_h
#define importexport_h

#include "knuthcam.hpp"
#include <string>
#include <iostream>

class importexport{
	public:
		int open(std::string file, drawing &d);
		int save(std::string file, drawing &d);
};

#endif