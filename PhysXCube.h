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
class CPhysXCube 
{
public:
	CPhysXCube();
	NxActor* getActor() {return m_actor;};
public:
	~CPhysXCube(void){};
	void init(NxScene*, osg::Vec3f, osg::Vec3f, std::string geometry_type, std::string file_name);
	osg::ref_ptr<osg::Geode> mp_geode;
	osg::ref_ptr<osg::MatrixTransform> mp_matrix_transform;
private:
	
	osg::Vec3f m_size;
	osg::Vec3f m_position;

	std::string m_file_name;
	std::string m_geometry_type;
	
	NxBodyDesc m_BodyDesc;
	NxBoxShapeDesc m_BoxDesc;
	NxActorDesc m_ActorDesc;
	
	//NxActor* m_newActor;
	NxScene* m_scene;
	NxActor* m_actor;

	bool m_applyShaderColor;
};

// demo functions
osg::Group* createTower(NxScene* scene, osg::Vec3f origin, int n=20);
osg::Group* createTowers(NxScene* scene, osg::Vec3f origin, float dx, float dy, int nx, int ny);
osg::Group* createWall(NxScene* scene, osg::Vec3f origin, float size, int nx, int ny);
osg::Group* createTriangle(NxScene* scene, osg::Vec3f origin, float size, int n);

#endif