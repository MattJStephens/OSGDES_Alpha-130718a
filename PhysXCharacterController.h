#ifndef __PHYSXMANIPULATOR_H__
#define __PHYSXMANIPULATOR_H__

#include <osg/Quat>
#include <osg/MatrixTransform>
#include <osg/Vec3f>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "ControllerHitReport.h"

static NxVec3	gInitialExtents(0.5f, 0.5f, 1.0f);
static NxF32	gInitialRadius = 0.5f;
static NxF32	gInitialHeight = 2.0f;

#define HEIGHT_ABOVE_GROUND 5

static float getHeightAboveGround()
{
	float height = HEIGHT_ABOVE_GROUND;	
	return height;
}

class CPhysXCharacterController : public osg::MatrixTransform
{
public:

	CPhysXCharacterController(NxControllerManager* gManager, NxScene* gScene, osg::Vec3f origin);

	virtual const char* className() const { return "PhysX Character Controller"; }


	double getHeight() { return (_height=getHeightAboveGround()); }

	NxController* getController();

	~CPhysXCharacterController()
	{
		_gManager->purgeControllers();
		NxReleaseControllerManager(_gManager);
	}

	bool calcDisplacement();

	NxExtendedVec3 getCharacterPos(NxU32 characterIndex);
	osg::Vec3f getCurrentPos(NxU32 characterIndex = 0);

	NxActor* getCharacterActor(NxU32 characterIndex);

	bool resetCharacterPos(NxU32 index, const NxVec3& pos);

	NxU32 moveCharacter(NxU32 characterIndex, NxScene* scene, 
		const NxVec3& dispVector, NxF32 elapsedTime, 
		NxU32 collisionGroups, NxF32 heightDelta);

	void updateControllers();


	double _scale;
	double _velocity;
	double _height;
	osg::Vec3d _position;
	NxExtendedVec3 _curpos;

	osg::Vec3d _origin;	
	double _distance;

	NxControllerManager* _gManager;
	NxScene* _gScene;
	NxCapsuleControllerDesc _desc;
	ControllerHitReport  gControllerHitReport;

	osg::Geode* _geode;
};


#endif