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

#include "ClothCallback.h"


/* cloth actor */
class Cloth : public osg::MatrixTransform {
public:
	Cloth(NxScene *scene, osg::Vec3f origin, NxReal w, NxReal h, NxReal d, 
		char *texFilename = NULL, bool tearLines = false)
		: mVertexRenderBuffer(NULL), mScene(NULL), mCloth(NULL), mClothMesh(NULL),
		mIndexRenderBuffer(NULL), mTempTexCoords(NULL), mNumTempTexCoords(0), 
		mTexId(0), mTeared(false)
	{
		bool gHardwareSimulation = false;
		mInitDone = false;
		
		float offset = 10.0f; // tmp

		clothDesc.globalPose.t = NxVec3(origin.x(), origin.z()+offset, origin.y());
		clothDesc.thickness = 0.2f;
		clothDesc.bendingStiffness = 0.5f;
		clothDesc.stretchingStiffness = 0.8f;
		clothDesc.density = 1.0f;
		clothDesc.friction = 0.5f;
		clothDesc.dampingCoefficient = 0.5f;
		clothDesc.solverIterations = 5;
		clothDesc.flags |= NX_CLF_BENDING;
		clothDesc.flags != NX_CLF_BENDING_ORTHO;
		clothDesc.flags |= NX_CLF_COLLISION_TWOWAY; // cloth-solid intraction
		clothDesc.flags |= NX_CLF_SELFCOLLISION;
		clothDesc.flags != NX_CLF_DAMPING ;

		if (gHardwareSimulation)
			clothDesc.flags |= NX_CLF_HARDWARE;

		generateRegularMeshDesc(meshDesc, w, h, d, texFilename != NULL);
		init(scene, clothDesc, meshDesc);
		if (texFilename) 
			createTexture(texFilename);

		setMatrix(osg::Matrix::identity());
		addChild( _geo_cloth );

		_geo_cloth->getDrawable(0)->setUpdateCallback(new ClothUpdateCallback(mCloth));		
	}
	~Cloth()
	{
		if (mInitDone) {
			if (mCloth) mScene->releaseCloth(*mCloth);
			if (mClothMesh) mScene->getPhysicsSDK().releaseClothMesh(*mClothMesh);
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
	Cloth(NxScene *scene, NxClothDesc &desc, NxReal w, NxReal h, NxReal d, 
		std::string imagefile);
	NxCloth *getNxCloth() { return mCloth; }
private:
	void init(NxScene *scene, NxClothDesc &desc, NxClothMeshDesc &meshDesc);
	void generateRegularMeshDesc(NxClothMeshDesc &desc, NxReal w, NxReal h, NxReal d, 
		bool texCoords);

	void releaseMeshDescBuffers(const NxClothMeshDesc& desc);

	bool cookMesh(NxClothMeshDesc& desc);

	void allocateReceiveBuffers(int numVertices, int numTriangles);
	void releaseReceiveBuffers();

	bool createTexture(std::string filename);
	void updateTextureCoordinates();

	bool mInitDone;

	NxClothMeshDesc meshDesc;

		NxClothDesc clothDesc;

	NxMeshData mReceiveBuffers;
	NxScene *mScene;
	NxCloth *mCloth;
	NxClothMesh *mClothMesh;

	int mTexId;

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

	// Temporary storage for tex coords
	float *mTempTexCoords;
	NxU32   mNumTempTexCoords;

	osg::Geode* _geo_cloth;
};

// for demo scenes
Cloth* createFreeCloth(NxScene* scene, osg::Vec3f origin);
Cloth* createAttachedCloth(NxScene* scene, osg::Vec3f origin);

#endif