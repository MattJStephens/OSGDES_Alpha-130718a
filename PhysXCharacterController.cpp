/*Primitive character controller class implementation, Janusz Goldasz */
#include "PhysXCharacterController.h"



CPhysXCharacterController::CPhysXCharacterController(NxControllerManager* gManager, NxScene* gScene, 
								   osg::Vec3f origin) : 
_gManager(gManager), 
_gScene(gScene), 
_origin(origin)
{
	setMatrix(osg::Matrix::identity());	

	_geode = (osg::Geode*) osgDB::readNodeFile("../models/capsule.osg");
	if (_geode)
	{
		_scale = 1.0f;
		setMatrix(osg::Matrix::translate(_origin));
		_position = _origin;

		addChild( _geode );

		_desc.position.x		= _origin.x();
		_desc.position.y		= _origin.z();
		_desc.position.z		= _origin.y();
		_desc.radius			= gInitialRadius * _scale;
		_desc.height			= gInitialHeight * _scale;
		_desc.upDirection	    = NX_Y;
		_desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
		_desc.stepOffset		= gInitialRadius * 0.5 * _scale;
		_desc.callback		    = &gControllerHitReport;
		_gManager->createController(_gScene, _desc);
	}
}


NxController* CPhysXCharacterController::getController()
{
	return _gManager->getController(0);
}

bool CPhysXCharacterController::calcDisplacement()
{
	// tmp
	return false;
}

NxExtendedVec3 CPhysXCharacterController::getCharacterPos(NxU32 characterIndex)
{
	return _gManager->getController(characterIndex)->getFilteredPosition();
}

osg::Vec3f CPhysXCharacterController::getCurrentPos(NxU32 characterIndex)
{
	// this should be changed to allow for retrieving the controller;s position. tmp.
	return _position;
}

NxActor* CPhysXCharacterController::getCharacterActor(NxU32 characterIndex)
{
	return _gManager->getController(characterIndex)->getActor();
}

bool CPhysXCharacterController::resetCharacterPos(NxU32 index, const NxVec3& pos)
{
	setMatrix( osg::Matrix::translate(osg::Vec3f(pos.x, pos.z, pos.y)));
	return _gManager->getController(index)->setPosition(
		NxExtendedVec3(pos.x, pos.y, pos.z));
}

NxU32 CPhysXCharacterController::moveCharacter(NxU32 characterIndex, NxScene* scene, 
									  const NxVec3& dispVector, NxF32 elapsedTime, 
									  NxU32 collisionGroups, NxF32 heightDelta)
{
	NxF32 sharpness = 1.0f;

	NxU32 collisionFlags;
	NxVec3 d = dispVector*elapsedTime;	

	if(heightDelta!=0.0f) d.y+=heightDelta;
	_gManager->getController(characterIndex)->move(
		d, collisionGroups, 0.000001f, collisionFlags, sharpness);

	_position = _position + osg::Vec3f(d.x, d.z, d.y + heightDelta );
	setMatrix( osg::Matrix::translate( _position ) );
	
	return collisionFlags;
}


void CPhysXCharacterController::updateControllers()
{
	_gManager->updateControllers();
}
