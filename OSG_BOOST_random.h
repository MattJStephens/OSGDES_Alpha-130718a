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

#ifndef _INCL_OSG_OSG_BOOST_random
#define _INCL_OSG_OSG_BOOST_random

#include "OSG_time.h"

#include <boost/math/distributions/normal.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/geometric_distribution.hpp>
#include <boost/random/triangle_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/lognormal_distribution.hpp>

#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

typedef boost::mt19937 Mersenne_Twister;    // Mersenne Twister

typedef boost::bernoulli_distribution<double> Bernoulli_Distribution;
typedef boost::geometric_distribution<int> Geometric_Distribution;
typedef boost::triangle_distribution<double> Triangle_Distribution;
typedef boost::exponential_distribution<double> Exponential_Distribution;
typedef boost::normal_distribution<double> Normal_Distribution;  
typedef boost::lognormal_distribution<double> Lognormal_distribution;  

typedef boost::variate_generator<Mersenne_Twister, Bernoulli_Distribution> Bernoulli_variate_generator;   
typedef boost::variate_generator<Mersenne_Twister, Geometric_Distribution> Geometric_variate_generator;   
typedef boost::variate_generator<Mersenne_Twister, Triangle_Distribution> Triangle_variate_generator;   
typedef boost::variate_generator<Mersenne_Twister, Exponential_Distribution> Exponential_variate_generator;   
typedef boost::variate_generator<Mersenne_Twister, Normal_Distribution> Normal_variate_generator;   
typedef boost::variate_generator<Mersenne_Twister, Lognormal_distribution> Lognormal_variate_generator;   

#endif