#ifndef _CPlane_H_
#define _CPlane_H_

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/StateSet>
#include <osgDB/ReadFile>
#include "NxPhysics.h"

/* CPlane actor - static actor */
class CPlane : public osg::MatrixTransform
{
public:
	CPlane(NxScene* scene, osg::Vec3f origin, float size):
	  _scene(scene), _origin(origin), _size(size)
	  {
		  init();		  
		  addChild(createQuadGeometry(_origin, _size, true));
		  setName("CPlane");
	  }
public:
	~CPlane(void){};
private:
	// methods
	void init();
	osg::Geode* createQuadGeometry(osg::Vec3f origin, float size, bool flip=false);
	// variables
	float _size;
	osg::Vec3f _origin;
	NxScene* _scene;
	osg::Geode* _geode;
};

#endif