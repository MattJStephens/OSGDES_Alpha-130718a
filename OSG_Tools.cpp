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

#include "OSG_Tools.h"


// v: A pointer to the array of vertices
// i: A pointer to the array of indices
// n: The number of indices (multiple of 3)
// This function uses Gauss's Theorem to calculate the volume of a body
// enclosed by a set of triangles. The triangle set must form a closed
// surface in R3 and be outward facing. Outward facing triangles index
// their vertices in a counterclockwise order where the x-axis points
// left, the y-axis point up and the z-axis points toward you (rhs).
//float CalculateVolume(const osg::Vec3d      *v,
//                      const unsigned int *i,
//                      const unsigned int  n)
//{
//   unsigned int j;
//   osg::Vec3d      v1;
//   osg::Vec3d      v2;
//   osg::Vec3d      v3;
//   float        volume = 0.0f;
//
//   for(j = 0; j < n; j+=3)
//   {
//      v1 = v[i[j ]];
//      v2 = v[i[j+1]];
//      v3 = v[i[j+2]];
//
//      volume += ((v2.y-v1.y)*(v3.z-v1.z)–
//                 (v2.z-v1.z)*(v3.y-v1.y) )*(v1.x+v2.x+v3.x);
//
//   }
//
//   return volume / 6.0f;
//}
//


double erf(double x)
{   
	// constants    
	double a1 =  0.254829592;   
	double a2 = -0.284496736;  
	double a3 =  1.421413741;  
	double a4 = -1.453152027;   
	double a5 =  1.061405429;    
	double p  =  0.3275911;   
	// Save the sign of x    
	int sign = 1;   
	if (x < 0)       
		sign = -1;   
	x = fabs(x);   
	
	// A&S formula 7.1.26  
	double t = 1.0/(1.0 + p*x); 
	double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);   
	return sign*y;
}

float lerp_f(float x0, float x1, float t) {
  return x0+ (x1- x0)* t;
}

double lerp_lf(double x0, double x1, double t) {
  return x0+ (x1- x0)* t;
}

osg::Vec3d lerp_Vec3d(osg::Vec3d v0, osg::Vec3d v1, double t) 
{ 
	osg::Vec3d vec;
	vec.x() = lerp_lf(v0.x(), v1.x(), t);
	vec.y() = lerp_lf(v0.y(), v1.y(), t);
	vec.z() = lerp_lf(v0.z(), v1.z(), t);
	return vec;
}


osg::Vec4d lerp_Vec4d(osg::Vec4d v0, osg::Vec4d v1, double t)
{
	osg::Vec4d vec;
	vec.x() = lerp_lf(v0.x(), v1.x(), t);
	vec.y() = lerp_lf(v0.y(), v1.y(), t);
	vec.z() = lerp_lf(v0.z(), v1.z(), t);
	vec.w() = lerp_lf(v0.w(), v1.w(), t);
	return vec;

}

osg::Quat lerp_Quat(osg::Quat v0, osg::Quat v1, double t)
{
	osg::Quat vec;

	vec.x() = lerp_lf(v0.x(), v1.x(), t);
	vec.y() = lerp_lf(v0.y(), v1.y(), t);
	vec.z() = lerp_lf(v0.z(), v1.z(), t);
	vec.w() = lerp_lf(v0.w(), v1.w(), t);

	return vec;
}