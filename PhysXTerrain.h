#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "NxPhysics.h"
#include <stdio.h>

#include <windows.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Drawable>
#include <osg/MatrixTransform>
#include <osgTerrain/Terrain>
#include <osg/LightModel>
#include <osg/StateSet>
#include <osg/Material>
#include <osgUtil/SmoothingVisitor>
#include "Cooking.h"
#include "Stream.h"
#include "UserAllocator.h"
#include "ErrorStream.h"

#define ONE_OVER_RAND_MAX	(1.0f / NxF32(RAND_MAX))
#define RAND_MAX_OVER_TWO   (RAND_MAX / 2 + 1)

#define TERRAIN_SIZE	33
#define TERRAIN_CHAOS	300.0f

#define TERRAIN_OFFSET	0.0f
#define TERRAIN_WIDTH	20.0f


// class to generate a terrain dataset for use with collisions

class TerrainData// : public NxAllocateable
	{
	public:
				TerrainData();
				~TerrainData();

	void		init(NxU32 size, NxF32 offset, NxF32 width, NxF32 chaos, bool flat=false, const NxVec3* pos=NULL);
	void		release();
	osg::Geode* createVisualTerrain(int, int);

	NxU32		size;
	NxU32		nbVerts;
	NxU32		nbFaces;
	NxF32		offset;
	NxF32		width;
	NxF32		chaos;
	NxVec3*		verts;
	NxVec3*		colors;
	NxVec3*		normals;
	NxU32*		faces;
	};



class CPhysXTerrain : public osg::MatrixTransform
{
public:
	CPhysXTerrain(NxPhysicsSDK* sdk, NxScene* scene, osg::Vec3f origin):
	  _sdk(sdk), _scene(scene), _origin(origin)
	{
		setMatrix(osg::Matrix::identity());

		addChild(createPhysicsTerrain(_sdk, _scene, _origin));
	}
private:
	osg::Geode* createPhysicsTerrain(NxPhysicsSDK* gPhysicsSDK,NxScene* scene, 
								 osg::Vec3f origin);
	NxPhysicsSDK* _sdk;
	NxActor* _gTerrain;
	NxScene* _scene;
	TerrainData* _gTerrainData;
	osg::Vec3f _origin;
};

/* demo scene */
osg::Group* createHeightfieldScene(NxPhysicsSDK* sdk, NxScene* scene, osg::Vec3f origin);

#endif