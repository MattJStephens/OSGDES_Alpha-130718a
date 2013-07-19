#ifndef __JOINT_H__
#define __JOINT_H__

#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
/* Joint example */

#include <osg/Group>
#include <osgDB/ReadFile>

#include "MotionCallback.h"
#include "NxPhysics.h"
#include "PhysXCube.h"

enum JointType
{
	SPHERICAL = 0,
	REVOLUTE,
	PRISMATIC,
	CYLINDRICAL,
	FIXEDJOINT,
	DISTANCE,
	PULLEY
};


void setJointParameters(NxJoint& desc, NxActor* a, NxActor* b, osg::Vec3f anchorPos);
osg::Group* CPhysXJoint(NxScene* scene, osg::Vec3f anchorPos, float length);

#endif