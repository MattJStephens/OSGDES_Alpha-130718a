#ifndef _OSGCUBE_H_
#define _OSGCUBE_H_

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Group>
#include <osgDB/ReadFile>

#include "MotionCallback.h"
#include "NxPhysics.h"
#include "Util.h"

/* cube actor */
class Cube : public osg::MatrixTransform
{
public:
	Cube(NxScene* gScene, osg::Vec3f size, osg::Vec3f position) :
	  _scene(gScene), _size(size), _position(position), _applyShaderColor(true)
	  {
		  osg::Geode* _geode = this->init( _scene, _size, _position);
		  addChild(_geode);
		  setMatrix(osg::Matrix::scale(_size)*osg::Matrix::translate(position));
		  if (_applyShaderColor)
			setUpdateCallback(new MotionCallback(_actor, this->getOrCreateStateSet()));	
		  else
			setUpdateCallback(new MotionCallback(_actor));	

		  // for debugging only
		  setName("cube");
		  _actor->setName(this->getName().c_str());

	  }	
	  NxActor* getActor() {return _actor;};
public:
	~Cube(void){};
	osg::Geode* init(NxScene*, osg::Vec3f, osg::Vec3f);
private:
	
	osg::Vec3f _size;
	osg::Vec3f _position;

	NxScene* _scene;
	NxActor* _actor;
	bool _applyShaderColor;
};

// demo functions
osg::Group* createTower(NxScene* scene, osg::Vec3f origin, int n=20);
osg::Group* createTowers(NxScene* scene, osg::Vec3f origin, float dx, float dy, int nx, int ny);
osg::Group* createWall(NxScene* scene, osg::Vec3f origin, float size, int nx, int ny);
osg::Group* createTriangle(NxScene* scene, osg::Vec3f origin, float size, int n);

#endif