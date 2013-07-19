#ifndef _OSGSPHERE_H_
#define _OSGSPHERE_H_

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Group>
#include <osgDB/ReadFile>

#include "MotionCallback.h"
#include "NxPhysics.h"
#include "Util.h"

/* CPhysXSphere actor */

class CPhysXSphere : public osg::MatrixTransform
{
public:
	CPhysXSphere(NxScene* gScene, float radius, osg::Vec3f position) :
		_scene(gScene), _radius(radius), 
		_position(position), _applyShaderColor(true)
	{
		osg::Geode* _geode = init( _scene, _radius, _position);
		addChild(_geode);
		setMatrix(osg::Matrix::scale(radius,radius,radius)*osg::Matrix::translate(position));
		if (_applyShaderColor)
			setUpdateCallback(new MotionCallback(_actor, this->getOrCreateStateSet()));	
		  else
			setUpdateCallback(new MotionCallback(_actor));		

		// for debugging purposes only
		//setName("sphere");
		//_actor->setName(this->getName().c_str());		MJS
	}	
		NxActor* getActor() {return _actor;};
private:
	osg::Geode* init(NxScene*, float, osg::Vec3f);
	float _radius;
	osg::Vec3f _position;
	NxScene* _scene;
	NxActor* _actor;
	bool _applyShaderColor;
};


// demo scene
osg::Group* distributeSpheres(NxScene* gScene, float terrainSize, int n=100);

#endif