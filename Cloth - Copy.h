#ifndef _CLOTH_H_
#define _CLOTH_H_
#include "PxPhysics.h"
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
	Cloth(PxScene *scene, osg::Vec3f origin, PxReal w, PxReal h, PxReal d, 
		char *texFilename = NULL, bool tearLines = false)
		: mVertexRenderBuffer(NULL), mScene(NULL), mCloth(NULL), mClothMesh(NULL),
		mIndexRenderBuffer(NULL), mTempTexCoords(NULL), mNumTempTexCoords(0), 
		mTexId(0), mTeared(false)
	{
		bool gHardwareSimulation = false;
		mInitDone = false;
		
		float offset = 10.0f; // tmp

		clothDesc.globalPose.t = PxVec3(origin.x(), origin.z()+offset, origin.y());
		clothDesc.thickness = 0.2f;
		clothDesc.bendingStiffness = 0.5f;
		clothDesc.stretchingStiffness = 0.8f;
		clothDesc.density = 1.0f;
		clothDesc.friction = 0.5f;
		clothDesc.dampingCoefficient = 0.5f;
		clothDesc.solverIterations = 5;
		clothDesc.flags |= PX_CLF_BENDING;
		clothDesc.flags != PX_CLF_BENDING_ORTHO;
		clothDesc.flags |= PX_CLF_COLLISION_TWOWAY; // cloth-solid intraction
		clothDesc.flags |= PX_CLF_SELFCOLLISION;
		clothDesc.flags != PX_CLF_DAMPING ;

		if (gHardwareSimulation)
			clothDesc.flags |= PX_CLF_HARDWARE;

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
	Cloth(PxScene *scene, PxClothDesc &desc, PxReal w, PxReal h, PxReal d, 
		std::string imagefile);
	PxCloth *getPxCloth() { return mCloth; }
private:
	void init(PxScene *scene, PxClothDesc &desc, PxClothMeshDesc &meshDesc);
	void generateRegularMeshDesc(PxClothMeshDesc &desc, PxReal w, PxReal h, PxReal d, 
		bool texCoords);

	void releaseMeshDescBuffers(const PxClothMeshDesc& desc);

	bool cookMesh(PxClothMeshDesc& desc);

	void allocateReceiveBuffers(int numVertices, int numTriangles);
	void releaseReceiveBuffers();

	bool createTexture(std::string filename);
	void updateTextureCoordinates();

	bool mInitDone;

	PxClothMeshDesc meshDesc;

		PxClothDesc clothDesc;

	PxMeshData mReceiveBuffers;
	PxScene *mScene;
	PxCloth *mCloth;
	PxClothMesh *mClothMesh;

	int mTexId;

	// Structure for the rendering buffer
	struct RenderBufferVertexElement
	{
		PxVec3 position;
		PxVec3 normal;
		float texCoord[2];
	};

	// The rendering buffers
	RenderBufferVertexElement* mVertexRenderBuffer;
	PxU32* mIndexRenderBuffer;

	PxU32 mMaxVertices;
	PxU32 mMaxIndices;
	PxU32 mNumIndices;
	PxU32 mNumParentIndices;
	PxU32 mNumVertices;
	PxU32 mLastNumVertices;

	PxU32 mMeshDirtyFlags;
	bool mTeared;

	// Temporary storage for tex coords
	float *mTempTexCoords;
	PxU32   mNumTempTexCoords;

	osg::Geode* _geo_cloth;
};

// for demo scenes
Cloth* createFreeCloth(PxScene* scene, osg::Vec3f origin);
Cloth* createAttachedCloth(PxScene* scene, osg::Vec3f origin);

#endif