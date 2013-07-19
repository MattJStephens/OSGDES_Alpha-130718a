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

/* CPhysXPlane actor - static actor */
//class CPhysXPlane : public osg::MatrixTransform
class CPhysXPlane 
{
public:
	osg::Vec3f m_origin;	
	osg::ref_ptr<osg::Geometry> mp_geometry;
	osg::ref_ptr<osg::Geode> mp_geode;
	osg::ref_ptr<osg::Texture2D> mp_texture;
	osg::ref_ptr<osg::Image> mp_image;
	osg::ref_ptr<osg::MatrixTransform> mp_matrix_transform;
	osg::ref_ptr<osg::PositionAttitudeTransform> mp_PAT;

	NxScene * m_scene;
	NxPlaneShapeDesc m_PlaneShapeDesc;	
    NxActorDesc m_ActorDesc;  
    NxActor* m_Actor;
	
	float m_size;	
	std::string m_filename;
	bool m_flip;

	CPhysXPlane();	
	void init(NxScene* scene, osg::Vec3f origin, std::string filename, float size);	
};

#endif