// ===============================================================================
//						  AGEIA PHYSX SDK SAMPLE PROGRAMS
//					        TEST SOFT BODY IMPLEMENTATION
//
//		            Written by Matthias Müller-Fischer 03-08-05
//					Revised by Roni Oeschger
// ===============================================================================

/* Visualisation using point sprites by Janusz Goldasz */

#ifndef MYSOFTBODY_H
#define MYSOFTBODY_H

#include "NxPhysics.h"
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Vec3f>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PolygonStipple>
#include <osg/PointSprite>
#include <osg/Geometry>
#include <osg/Point>
class ObjMesh;

enum SoftBodyType
{
	SOFTBODY_BOX = 0,
	SOFTBODY_SPHERE	= 1,
};

// soft body demo implementation from nvidia - osg stuff is mine, JG
class CPhysXSoftBody : public osg::MatrixTransform
{
public:
	CPhysXSoftBody() { }
	/* create mesh from tet file */
	CPhysXSoftBody(NxScene *scene, NxSoftBodyDesc &desc, char *tetFileName, ObjMesh *surfaceMesh);
	/* regular mesh */
	CPhysXSoftBody(NxScene *scene, NxSoftBodyDesc &desc, SoftBodyType type, NxReal wx, NxReal wy, NxReal wz, NxReal h, bool tearLines = false);

	~CPhysXSoftBody();

	virtual void simulateAndDraw(bool shadows, bool tetraMesh, bool tetraWireframe);
	NxSoftBody *getNxSoftBody() { return mSoftBody; }

	const NxMeshData* getMeshData() const { return &mReceiveBuffers; }


protected:
	virtual void init(NxScene *scene, NxSoftBodyDesc &desc, NxSoftBodyMeshDesc &meshDesc, ObjMesh *surfaceMesh);
	bool generateTetMeshDesc(NxSoftBodyMeshDesc &desc, char *filename);

	void releaseMeshDescBuffers(const NxSoftBodyMeshDesc& desc);
	bool cookMesh(NxSoftBodyMeshDesc& desc);

	virtual void allocateReceiveBuffers(int numVertices, int numTetrahedra);
	virtual void releaseReceiveBuffers();

	void generateBoxMeshDesc(NxSoftBodyMeshDesc &desc, NxReal wx, NxReal wy, NxReal wz, NxReal h, bool tearLines = false);

	int childNr(int i0, int i1, NxVec3 &pos);
	void sphereSubdivide(const NxVec3 &p0, const NxVec3 &p1, const NxVec3 &p2, int i0, int i1, int i2, int level);
	void generateSphereMeshDesc(NxSoftBodyMeshDesc &desc, NxReal wx, NxReal wy, NxReal wz, NxReal h, bool tearLines = false);
	static const int maxSphereLevels = 4;
	NxArray<NxU32> mSphereIndices;
	NxArray<NxVec3> mSphereVertices;
	NxArray<int> mSphereDivisions;

	bool mInitDone;

	NxMeshData mReceiveBuffers;
	NxScene *mScene;
	NxSoftBody *mSoftBody;
	NxSoftBodyMesh *mSoftBodyMesh;

	ObjMesh *mSurfaceMesh;

	NxArray<NxVec3> mTetraRenderVerts;
	NxArray<NxVec3> mTetraRenderNormals;

	NxReal  mScale;
	NxVec3  mOffset;

	osg::Geode* _geode;
};


// not used - crashes
class SoftBodyCallback : public osg::NodeCallback
{
public:
	SoftBodyCallback(NxScene* scene, CPhysXSoftBody* sb) : _scene(scene), _sb(sb)
	{

	};

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		if (_sb)
		{
			_sb->simulateAndDraw(false, false, false);
		}
		traverse(node, nv);
	}
private:
	NxScene* _scene;
	CPhysXSoftBody* _sb;
};

osg::Group* createDemoSoftBody(NxScene* scene, osg::Vec3f origin, NxArray<CPhysXSoftBody*> &sb);

#endif
