#ifndef _INCL_OSG_CUBE_GEODE
#define _INCL_OSG_CUBE_GEODE

#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

using namespace osg;

Geode *createUnitCube();
osg::Node* createHeightField(std::string heightFile, std::string texFile);

#endif

 