/* Cloth class implementation, osg part by Janusz Goldasz */
#include "Cloth.h"
#include "Cooking.h"
#include "Stream.h"
#define TEAR_MEMORY_FACTOR 3
#include <osg/Point>

// -----------------------------------------------------------------------
void Cloth::init(NxScene *scene, NxClothDesc &desc, NxClothMeshDesc &meshDesc)
{
	mScene = scene;

	// if we want tearing we must tell the cooker
	// this way it will generate some space for particles that will be generated during tearing
	if (desc.flags & NX_CLF_TEARABLE)
		meshDesc.flags |= NX_CLOTH_MESH_TEARABLE;

	// Enable Debug rendering for this cloth
	desc.flags |= NX_CLF_VISUALIZATION;

	cookMesh(meshDesc);
	releaseMeshDescBuffers(meshDesc);

	allocateReceiveBuffers(meshDesc.numVertices, meshDesc.numTriangles);

	desc.clothMesh = mClothMesh;
	desc.meshData = mReceiveBuffers;
	mCloth = scene->createCloth(desc);
	mInitDone = true;
}

void Cloth::generateRegularMeshDesc(NxClothMeshDesc &desc, NxReal w, NxReal h, NxReal d, bool 
									texCoords)
{
	int numX = (int)(w / d) + 1;
	int numY = (int)(h / d) + 1;

	desc.numVertices				= (numX+1) * (numY+1);
	desc.numTriangles				= numX*numY*2;
	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.triangleStrideBytes		= 3*sizeof(NxU32);
	desc.vertexMassStrideBytes		= sizeof(NxReal);
	desc.vertexFlagStrideBytes		= sizeof(NxU32);
	desc.points						= (NxVec3*)malloc(sizeof(NxVec3)*desc.numVertices);
	desc.triangles					= (NxU32*)malloc(sizeof(NxU32)*desc.numTriangles*3);
	desc.vertexMasses				= 0;
	desc.vertexFlags				= 0;
	desc.flags						= 0;

	mMaxVertices = TEAR_MEMORY_FACTOR * desc.numVertices;
	mMaxIndices  = 3 * desc.numTriangles;


	_geo_cloth = new osg::Geode;
	int i,j;
	// create particles - POINTS
	{
		osg::Geometry* pointsGeom = new osg::Geometry();
		osg::Vec3Array* vertices = new osg::Vec3Array;		

		NxVec3 *p = (NxVec3*)desc.points;
		for (i = 0; i <= numY; i++) {
			for (j = 0; j <= numX; j++) {
				p->set(d*j, 0.0f, d*i); 
				vertices->push_back(osg::Vec3f(d*j, d*i, 0.0f));
				p++;
			}
		}

		pointsGeom->setVertexArray(vertices);

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,0.8f));

		pointsGeom->setColorArray(colors);
		pointsGeom->setColorBinding(osg::Geometry::BIND_OVERALL);


		// set the normal in the same way color.
		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
		pointsGeom->setNormalArray(normals);
		pointsGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);
		
		osg::StateSet* stateSet = new osg::StateSet();
        
		pointsGeom->setStateSet(stateSet);
		pointsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,vertices->size()));
		

		osg::Point* point = new osg::Point;
        point->setDistanceAttenuation(osg::Vec3(0.0,0.0000,0.05f));
		point->setSize(20.0f);
        stateSet->setAttribute(point);

		osg::PointSprite *sprite = new osg::PointSprite();
        stateSet->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);

		

		// add the points geometry to the geode.
		_geo_cloth->addDrawable(pointsGeom);
	}

	
	if (texCoords) {
		mTempTexCoords = (float *)malloc(sizeof(float)*2*TEAR_MEMORY_FACTOR*desc.numVertices);
		float *f = mTempTexCoords;
		float dx = 1.0f; if (numX > 0) dx /= numX;
		float dy = 1.0f; if (numY > 0) dy /= numY;
		for (i = 0; i <= numY; i++) {
			for (j = 0; j <= numX; j++) {
				*f++ = j*dx;
				*f++ = i*dy;
			}
		}
		
	
		mNumTempTexCoords = desc.numVertices;
	}
	else
	{
		mNumTempTexCoords = 0;
		mTempTexCoords = NULL;
	}

	NxU32 *id = (NxU32*)desc.triangles;
	for (i = 0; i < numY; i++) {
		for (j = 0; j < numX; j++) {
			NxU32 i0 = i * (numX+1) + j;
			NxU32 i1 = i0 + 1;
			NxU32 i2 = i0 + (numX+1);
			NxU32 i3 = i2 + 1;
			if ((j+i)%2) {
				*id++ = i0; *id++ = i2; *id++ = i1;
				*id++ = i1; *id++ = i2; *id++ = i3;
			}
			else {
				*id++ = i0; *id++ = i2; *id++ = i3;
				*id++ = i0; *id++ = i3; *id++ = i1;
			}
		}
	}

}


void Cloth::releaseMeshDescBuffers(const NxClothMeshDesc& desc)
{
	NxVec3* p = (NxVec3*)desc.points;
	NxU32* t = (NxU32*)desc.triangles;
	NxReal* m = (NxReal*)desc.vertexMasses;
	NxU32* f = (NxU32*)desc.vertexFlags;
	free(p);
	free(t);
	free(m);
	free(f);
}

bool Cloth::cookMesh(NxClothMeshDesc& desc)
{
	// Store correct number to detect tearing mesh in time
	mLastNumVertices = desc.numVertices;

	// we cook the mesh on the fly through a memory stream
	// we could also use a file stream and pre-cook the mesh
	MemoryWriteBuffer wb;
	assert(desc.isValid());
	bool success = CookClothMesh(desc, wb);

	if (!success) 
		return false;

	MemoryReadBuffer rb(wb.data);
	mClothMesh = mScene->getPhysicsSDK().createClothMesh(rb);
	return true;
}


void Cloth::allocateReceiveBuffers(int numVertices, int numTriangles)
{
	// here we setup the buffers through which the SDK returns the dynamic cloth data
	// we reserve more memory for vertices than the initial mesh takes
	// because tearing creates new vertices
	// the SDK only tears cloth as long as there is room in these buffers

	//NxU32 maxVertices = TEAR_MEMORY_FACTOR * numVertices;
	//NxU32 maxIndices = 3*numTriangles;

	if (mVertexRenderBuffer == NULL)
	{
		// Allocate Render Buffer for Vertices if it hasn't been done before
		mVertexRenderBuffer = (RenderBufferVertexElement*)malloc(sizeof(RenderBufferVertexElement) * mMaxVertices);
		memset(mVertexRenderBuffer, 0, sizeof(RenderBufferVertexElement) * mMaxVertices);
	}

	if (mIndexRenderBuffer == NULL)
	{
		// Allocate Render Buffer for Indices if it hasn't been done before
		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
	}

	mReceiveBuffers.verticesPosBegin         = &(mVertexRenderBuffer[0].position.x);
	mReceiveBuffers.verticesNormalBegin      = &(mVertexRenderBuffer[0].normal.x);
	mReceiveBuffers.verticesPosByteStride    = sizeof(RenderBufferVertexElement);
	mReceiveBuffers.verticesNormalByteStride = sizeof(RenderBufferVertexElement);
	mReceiveBuffers.maxVertices              = mMaxVertices;
	mReceiveBuffers.numVerticesPtr           = &mNumVertices;

	// the number of triangles is constant, even if the cloth is torn
	NxU32 maxIndices = 3*numTriangles;
	mReceiveBuffers.indicesBegin             = mIndexRenderBuffer;
	mReceiveBuffers.indicesByteStride        = sizeof(NxU32);
	mReceiveBuffers.maxIndices               = maxIndices;
	mReceiveBuffers.numIndicesPtr            = &mNumIndices;

	if (mNumTempTexCoords > 0)
	{
		// Copy Tex Coords from temp buffers to graphics buffer
		assert(mNumTempTexCoords == numVertices);

		for (NxU32 i = 0; i < mNumTempTexCoords; i++)
		{
			mVertexRenderBuffer[i].texCoord[0] = mTempTexCoords[2*i+0];
			mVertexRenderBuffer[i].texCoord[1] = mTempTexCoords[2*i+1];
		}

		// Get rid of temp buffer
		mNumTempTexCoords = 0;
		free (mTempTexCoords);
		mTempTexCoords = NULL;
	}

	// the parent index information would be needed if we used textured cloth
	mReceiveBuffers.parentIndicesBegin       = (NxU32*)malloc(sizeof(NxU32)*mMaxVertices);
	mReceiveBuffers.parentIndicesByteStride  = sizeof(NxU32);
	mReceiveBuffers.maxParentIndices         = mMaxVertices;
	mReceiveBuffers.numParentIndicesPtr      = &mNumParentIndices;

	mReceiveBuffers.dirtyBufferFlagsPtr = &mMeshDirtyFlags;

	// init the buffers in case we want to draw the mesh 
	// before the SDK as filled in the correct values
	mMeshDirtyFlags = 0;
	mNumVertices = 0;
	mNumIndices = 0;
	mNumParentIndices = 0;
}

// -----------------------------------------------------------------------
bool Cloth::createTexture(std::string filename)
{
	/*BmpLoader bl;
	if (!bl.loadBmp(filename)) return false;

	glGenTextures(1, &mTexId);
	if (!mTexId) return false;
	glBindTexture(GL_TEXTURE_2D, mTexId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	bl.mWidth, bl.mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bl.mRGB);*/

	return true;
}


void Cloth::releaseReceiveBuffers()
{
	free (mReceiveBuffers.parentIndicesBegin);

	mReceiveBuffers.setToDefault();
}

void Cloth::updateTextureCoordinates()
{
	NxU32 numVertices = *mReceiveBuffers.numVerticesPtr;
	NxU32 *parent = (NxU32 *)mReceiveBuffers.parentIndicesBegin + mLastNumVertices;
	for (NxU32 i = mLastNumVertices; i < numVertices; i++, parent++) {
		mVertexRenderBuffer[i].texCoord[0] = mVertexRenderBuffer[*parent].texCoord[0];
		mVertexRenderBuffer[i].texCoord[1] = mVertexRenderBuffer[*parent].texCoord[1];
	}

	mTeared |= mLastNumVertices != numVertices;
	mLastNumVertices = numVertices;
}




Cloth* createFreeCloth(NxScene* scene, osg::Vec3f origin)
{
	Cloth *regularCloth = new Cloth(scene, origin, 8.0f, 8.0f, 0.2f, 0);
	if (!regularCloth->getNxCloth())
	{
		printf("Error: Unable to create the cloth for the current scene.\n");
		delete regularCloth;
	} else
	{		
		//regularCloth->getNxCloth()->attachToShape(*capsule->getShapes(), 0);
	}
	return regularCloth;
}


Cloth* createAttachedCloth(NxScene* scene, osg::Vec3f origin)
{
	float w = 8.0f;
	float h = 8.0f;
	float d = 0.2f;
	Cloth *regularCloth = new Cloth(scene, origin, w, h, d, 0);
	if (!regularCloth->getNxCloth())
	{
		printf("Error: Unable to create the cloth for the current scene.\n");
		delete regularCloth;
	} else
	{	
		int numX = (int)(w/d)+1;
		int numY = (int)(h/d)+1;
		int i1 = 0;
		NxVec3 p1 = regularCloth->getNxCloth()->getPosition(i1);
		int i2 = numX;
		NxVec3 p2 = regularCloth->getNxCloth()->getPosition(i2);
		int i3 = numX*(numY+1);
		NxVec3 p3 = regularCloth->getNxCloth()->getPosition(i3);
		int i4 = (numX+1)*(numY+1)-1;
		NxVec3 p4 = regularCloth->getNxCloth()->getPosition(i4);

		regularCloth->getNxCloth()->attachVertexToGlobalPosition(i1,p1);
		regularCloth->getNxCloth()->attachVertexToGlobalPosition(i2,p2);
		regularCloth->getNxCloth()->attachVertexToGlobalPosition(i3,p3);
		regularCloth->getNxCloth()->attachVertexToGlobalPosition(i4,p4);
	}
	return regularCloth;
}