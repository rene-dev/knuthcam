#pragma once

#include "knuthcam.hpp"
//#include "seg.hpp"
//#include <string>
//#include <iostream>

class importexport{
	public:
		int open(std::string file, drawing &d);
		int save(std::string file, drawing &d);
};
