#ifndef _CLOTH_H_
#define _CLOTH_H_
#include "NxPhysics.h"
#include <iostream>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PolygonStipple>
#include <osg/PointSprite>

#include <osg/StateSet>

#include "PhysXClothCallback.h"

enum ePrimitiveSetType { PrimitiveSetType_Points, PrimitiveSetType_Triangles };

/* cloth actor */
class CPhysXCloth : public osg::MatrixTransform {
public:
	CPhysXCloth(NxScene *scene, osg::Vec3f origin, NxReal w, NxReal h, NxReal d, NxReal thickness, NxReal bendingStiffness, NxReal stretchingStiffness, NxReal density, NxReal friction, 
		NxReal dampingCoefficient, 	NxU32 solverIterations, std::string filename, bool tearLines, ePrimitiveSetType primitive_set_type, bool attach);
	~CPhysXCloth()
	{
		if (mInitDone) {
			//if (mCloth) mScene->releaseCloth(*mCloth); wtf?
			//if (mClothMesh) mScene->getPhysicsSDK().releaseClothMesh(*mClothMesh);
			releaseReceiveBuffers();

			// temp texcoords are only used temporary and should be deallocated way earlier
			assert(mTempTexCoords == NULL);

			if (mTexId) {
				//glDeleteTextures(1, &mTexId);
				mTexId = 0;
			}

			// delete the rendering buffers
			free (mVertexRenderBuffer);
			free (mIndexRenderBuffer);
			
		}
	};	
	NxCloth *getNxCloth() { return mCloth; }

	ePrimitiveSetType m_primitive_set_type;
private:
	void init();
	void generateRegularMeshDesc();
	void releaseMeshDescBuffers();
	bool cookMesh(NxClothMeshDesc& desc);
	void allocateReceiveBuffers(int numVertices, int numTriangles);
	void releaseReceiveBuffers();
	bool createTexture(std::string filename);
	void updateTextureCoordinates();
	bool mInitDone;
	bool m_create_texture_coords;
	NxClothMeshDesc m_meshDesc;
	NxClothDesc m_clothDesc;
	NxMeshData mReceiveBuffers;
	NxScene *mScene;
	NxCloth *mCloth;
	NxClothMesh *mClothMesh;
	int mTexId;
	bool m_attach;
	float m_w;
	float m_h;
	float m_d;
	// Structure for the rendering buffer
	struct RenderBufferVertexElement
	{
		NxVec3 position;
		NxVec3 normal;
		float texCoord[2];
	};

	// The rendering buffers
	RenderBufferVertexElement* mVertexRenderBuffer;
	NxU32* mIndexRenderBuffer;
	NxU32 mMaxVertices;
	NxU32 mMaxIndices;
	NxU32 mNumIndices;
	NxU32 mNumParentIndices;
	NxU32 mNumVertices;
	NxU32 mLastNumVertices;
	NxU32 mMeshDirtyFlags;
	bool mTeared;
	float *mTempTexCoords;
	NxU32 mNumTempTexCoords;
	osg::ref_ptr<osg::Geode> p_geode_cloth;	
};

// for demo scenes
CPhysXCloth* createFreeCloth(NxScene* scene, osg::Vec3f origin);
CPhysXCloth* createAttachedCloth(NxScene* scene, osg::Vec3f origin);

#endif