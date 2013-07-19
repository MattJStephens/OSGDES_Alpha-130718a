//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#ifndef _OSG_DES_
#define _OSG_DES_

#define NOMINMAX //Physx

//#include <boost/array.hpp>
//#include <boost/asio.hpp>
#include "windows.h"
#include <string>

#include <time.h>
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <direct.h>
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
#include <sstream>


//
//#include "BigFloat.h"
//
//#include <boost/math/distributions/normal.hpp>
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/variate_generator.hpp>
//typedef boost::mt19937                     ENG;    // Mersenne Twister
//typedef boost::normal_distribution<double> DIST;   // Normal Distribution
//typedef boost::variate_generator<ENG,DIST> GEN;    // Variate generator

std::string get_default_script();
std::string get_node_script();

#endif