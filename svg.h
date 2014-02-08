#ifndef svg_h
#define svg_h

#include <iostream>
#include <string>
#include <vector>

#include "knuthcam.h"
#include "importexport.h"

using std::cout;
using std::endl;
using std::string;

class svg : public importexport{
	public:
		int open(string s, drawing_t &d);
		int save(string s, drawing_t &d);
};

#endif