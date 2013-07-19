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

#ifndef _INCL_OSG_Geometry_tools
#define _INCL_OSG_Geometry_tools

#include <string>
#include <osg/Geode>
#include <osg/AnimationPath>

using namespace osg;

osg::AnimationPath* createAnimationPath(const osg::Vec3& center,float radius, double looptime);
osg::Vec3 computeTerrainIntersection(osg::Node* subgraph,float x,float y);
osg::Geode* createUnitCube();
osg::Geode* createHeightField(std::string heightFile, std::string texFile);
osg::Geode *makeGalaxy(unsigned nvertices);
osg::StateSet* makeStateSet(float size);
osg::Node* createMovingModel(const osg::Vec3& center, float radius);

#endif

 