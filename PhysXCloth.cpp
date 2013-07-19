/* Cloth class implementation, osg part by Janusz Goldasz */
#include "PhysXCloth.h"
#include "Cooking.h"
#include "Stream.h"
#define TEAR_MEMORY_FACTOR 3
#include <osg/Point>
#include <osg/LightModel>
#include <osg/StateSet>
#include <osg/Material>
#include "BmpLoader.h"
#include "OSG_Tools.h"

CPhysXCloth::CPhysXCloth(NxScene *scene, osg::Vec3f origin, NxReal w, NxReal h, NxReal d, NxReal thickness, NxReal bendingStiffness, 
	NxReal stretchingStiffness, NxReal density, NxReal friction, NxReal dampingCoefficient, NxU32 solverIterations, std::string filename, bool tearLines,
	ePrimitiveSetType primitive_set_type, bool attach) 
{
	m_primitive_set_type = primitive_set_type;
	mVertexRenderBuffer = NULL;	
	mCloth = NULL;
	mClothMesh = NULL;
	mIndexRenderBuffer = NULL;
	mTempTexCoords = NULL;
	mNumTempTexCoords = 0;
	mTexId = 0;
	mTeared = false;
	bool gHardwareSimulation = false;
	mInitDone = false;		
	float offset = 10.0f; // tmp
	m_w= w;
	m_h= h;
	m_d= d;
	m_attach = attach;
	mScene = scene;	

	//clothDesc.globalPose.t = NxVec3(origin.x(), origin.z()+offset, origin.y());
	m_clothDesc.globalPose.t = NxVec3(origin.x(), origin.z(), origin.y());
	m_clothDesc.thickness = thickness;
	m_clothDesc.bendingStiffness = bendingStiffness;
	m_clothDesc.stretchingStiffness = stretchingStiffness;
	m_clothDesc.density = density;
	m_clothDesc.friction = friction;
	m_clothDesc.dampingCoefficient = dampingCoefficient;
	m_clothDesc.solverIterations = solverIterations;
	m_clothDesc.flags |= NX_CLF_BENDING;
	m_clothDesc.flags != NX_CLF_BENDING_ORTHO;
	m_clothDesc.flags |= NX_CLF_COLLISION_TWOWAY; 
	m_clothDesc.flags |= NX_CLF_SELFCOLLISION;
	m_clothDesc.flags != NX_CLF_DAMPING ;

	if (gHardwareSimulation) {	m_clothDesc.flags |= NX_CLF_HARDWARE; }	
	m_create_texture_coords=false;
	if(filename.size()> 0) { 
		m_create_texture_coords = true;
	}
	generateRegularMeshDesc();
	init();
	if (m_create_texture_coords)  { createTexture(filename); }
	setMatrix(osg::Matrix::identity());
	addChild( p_geode_cloth );
	p_geode_cloth->getDrawable(0)->setUpdateCallback(new ClothUpdateCallback(mCloth));	
}

void CPhysXCloth::init()
{	
	if (m_clothDesc.flags & NX_CLF_TEARABLE)
	{
		m_meshDesc.flags |= NX_CLOTH_MESH_TEARABLE;	
	}
	m_clothDesc.flags |= NX_CLF_VISUALIZATION;
	cookMesh(m_meshDesc);
	releaseMeshDescBuffers();
	allocateReceiveBuffers(m_meshDesc.numVertices, m_meshDesc.numTriangles);
	m_clothDesc.clothMesh = mClothMesh;
	m_clothDesc.meshData = mReceiveBuffers;
	mCloth = mScene->createCloth(m_clothDesc);
	if(m_attach)
	{		
		int numX = (int)(m_w/m_d)+1;
		int numY = (int)(m_h/m_d)+1;
		int i1 = 0;
		NxVec3 p1 = mCloth->getPosition(i1);
		int i2 = numX;
		NxVec3 p2 = mCloth->getPosition(i2);
		int i3 = numX*(numY+1);
		NxVec3 p3 = mCloth->getPosition(i3);
		int i4 = (numX+1)*(numY+1)-1;
		NxVec3 p4 = mCloth->getPosition(i4);

		mCloth->attachVertexToGlobalPosition(i1,p1);
		mCloth->attachVertexToGlobalPosition(i2,p2);
		//mCloth->attachVertexToGlobalPosition(i3,p3);
		//mCloth->attachVertexToGlobalPosition(i4,p4);
	}
	mInitDone = true;
}

void CPhysXCloth::generateRegularMeshDesc()
{
	p_geode_cloth= new osg::Geode();	
	osg::ref_ptr<osg::Geometry> p_geometry= new osg::Geometry();
	osg::ref_ptr<osg::StateSet> stateSet= new osg::StateSet();	
	osg::ref_ptr<osg::Vec3Array> p_Vec3Array_verticies= new osg::Vec3Array();	
	osg::ref_ptr<osg::Vec3Array> p_Vec3Array_normals= new osg::Vec3Array();	
	osg::ref_ptr<osg::Vec4Array> p_Vec4Array_colors= new osg::Vec4Array();	
	osg::ref_ptr<osg::Vec2Array> p_Vec2Array_Tex;
	osg::ref_ptr<osg::DrawElementsUInt> faceArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0); 	
		
	int i,j;
	int numX = (int)(m_w / m_d) + 1;
	int numY = (int)(m_h / m_d) + 1;
	int NumVertices =  (numX+1) * (numY+1);
	int NumFaces =  numX * numY * 2;

	m_meshDesc.numVertices = NumVertices;
	m_meshDesc.numTriangles	= NumFaces;
	m_meshDesc.pointStrideBytes	= sizeof(NxVec3);
	m_meshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
	m_meshDesc.vertexMassStrideBytes = sizeof(NxReal);
	m_meshDesc.vertexFlagStrideBytes = sizeof(NxU32);
	m_meshDesc.points = (NxVec3*)malloc(sizeof(NxVec3) * m_meshDesc.numVertices);
	m_meshDesc.triangles = (NxU32*)malloc(sizeof(NxU32) * m_meshDesc.numTriangles*3);
	m_meshDesc.vertexMasses	= 0;
	m_meshDesc.vertexFlags = 0;
	m_meshDesc.flags = 0;

	p_Vec3Array_verticies->reserve(NumVertices);
	p_Vec3Array_normals->reserve(NumVertices);
	p_Vec4Array_colors->reserve(NumVertices);


	mMaxVertices = TEAR_MEMORY_FACTOR * m_meshDesc.numVertices;
	mMaxIndices  = 3 * m_meshDesc.numTriangles;	
	
	NxVec3 *p = (NxVec3*)m_meshDesc.points;
	NxU32 *id = (NxU32*)m_meshDesc.triangles;	

	//create vertex array
	for (i = 0; i <= numY; i++) {
		for (j = 0; j <= numX; j++) {
			p->set(m_d* j, 0.0f, m_d*i); 
			p_Vec3Array_verticies->push_back(osg::Vec3f(m_d*j, m_d*i, 0.0f));
			p++;
			//vertex colors
			p_Vec4Array_colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
		}
	}	
	
	//create face index array
	for (i = 0; i < numY; i++) {
		for (j = 0; j < numX; j++) {
			NxU32 i0 = i * (numX+1) + j;
			NxU32 i1 = i0 + 1;
			NxU32 i2 = i0 + (numX+1);
			NxU32 i3 = i2 + 1;
			if ((j+i)%2) {
				*id++ = i0; 
				*id++ = i2; 
				*id++ = i1;
				*id++ = i1; 
				*id++ = i2; 
				*id++ = i3;
				faceArray->push_back(i0);
				faceArray->push_back(i2);
				faceArray->push_back(i1);
				faceArray->push_back(i1);
				faceArray->push_back(i2);
				faceArray->push_back(i3);
			}
			else {
				*id++ = i0; 
				*id++ = i2; 
				*id++ = i3;
				*id++ = i0; 
				*id++ = i3; 
				*id++ = i1;
				faceArray->push_back(i0);
				faceArray->push_back(i2);
				faceArray->push_back(i3);
				faceArray->push_back(i0);
				faceArray->push_back(i3);
				faceArray->push_back(i1);
			}
		}
	}	

	for (i = 0; i < NumVertices; i++) {
		p_Vec3Array_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));	
	}

	p_geometry->setUseDisplayList(true);    
	p_geometry->setUseVertexBufferObjects(true);    
	p_geometry->setVertexArray(p_Vec3Array_verticies);
	p_geometry->setNormalArray(p_Vec3Array_normals);
	p_geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	p_geometry->setColorArray(p_Vec4Array_colors);
	p_geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	switch(m_primitive_set_type)
	{
		case PrimitiveSetType_Points:
		{		
			p_geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, p_Vec3Array_verticies->size()));		
			osg::Point* point = new osg::Point;
			point->setDistanceAttenuation(osg::Vec3(0.0, 0.0000, 0.05f));
			point->setSize(20.0f);
			stateSet->setAttribute(point);
			osg::PointSprite *sprite = new osg::PointSprite();
			stateSet->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);		
			
			break;
		}
		case PrimitiveSetType_Triangles:
		{
			p_geometry->addPrimitiveSet(faceArray);	
			//p_geometry->setUseVertexBufferObjects(true);			
		}	
	}
	p_geode_cloth->addDrawable(p_geometry);

	  

	//osg::Material* material = new osg::Material;
 //  // material->setColorMode ( osg::Material::OFF );
 //  // material->setEmission ( osg::Material::BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	//material->setAmbient ( osg::Material::BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	//material->setDiffuse ( osg::Material::BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	//material->setSpecular ( osg::Material::BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	//p_geode_cloth->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );	
	
	osg::ref_ptr<osg::Material> material= new osg::Material();
	///material->setColorMode ( osg::Material::ON );
	material->setDiffuse ( osg::Material::FRONT, osg::Vec4(random_float(1),random_float(1), random_float(1), 1.0f) );	
	material->setDiffuse ( osg::Material::BACK, osg::Vec4(random_float(1),random_float(1), random_float(1), 1.0f) );	
	//material->setDiffuse ( osg::Material::FRONT, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	//material->setDiffuse ( osg::Material::BACK, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	p_geode_cloth->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );

	//osg::LightModel* lightModel = new osg::LightModel();
	//lightModel->setTwoSided ( true );  
	//p_geode_cloth->getOrCreateStateSet()->setAttributeAndModes( lightModel, osg::StateAttribute::ON );


	//mNumTempTexCoords = 0;
	//mTempTexCoords = NULL;

	/*if (m_create_texture_coords) {
		mTempTexCoords = (float *)malloc(sizeof(float)*2*TEAR_MEMORY_FACTOR*m_meshDesc.numVertices);
		float *f = mTempTexCoords;
		float dx = 1.0f; if (numX > 0) dx /= numX;
		float dy = 1.0f; if (numY > 0) dy /= numY;
		for (i = 0; i <= numY; i++) {
			for (j = 0; j <= numX; j++) {
				*f++ = j*dx;
				*f++ = i*dy;
			}
		}	
		mNumTempTexCoords = m_meshDesc.numVertices;
	}*/
	
}
void CPhysXCloth::releaseMeshDescBuffers()
{
	NxVec3* p = (NxVec3*)m_meshDesc.points;
	NxU32* t = (NxU32*)m_meshDesc.triangles;
	NxReal* m = (NxReal*)m_meshDesc.vertexMasses;
	NxU32* f = (NxU32*)m_meshDesc.vertexFlags;
	free(p);
	free(t);
	free(m);
	free(f);
}
bool CPhysXCloth::cookMesh(NxClothMeshDesc& desc)
{
	mLastNumVertices = desc.numVertices;
	MemoryWriteBuffer wb;
	assert(desc.isValid());
	bool success = CookClothMesh(desc, wb);
	if (!success) { return false; }
	MemoryReadBuffer rb(wb.data);
	mClothMesh = mScene->getPhysicsSDK().createClothMesh(rb);
	return true;
}
void CPhysXCloth::allocateReceiveBuffers(int numVertices, int numTriangles)
{
	if (mVertexRenderBuffer == NULL)
	{		
		mVertexRenderBuffer = (RenderBufferVertexElement*)malloc(sizeof(RenderBufferVertexElement) * mMaxVertices);
		memset(mVertexRenderBuffer, 0, sizeof(RenderBufferVertexElement) * mMaxVertices);
	}
	if (mIndexRenderBuffer == NULL)
	{	
		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
	}
	mReceiveBuffers.verticesPosBegin         = &(mVertexRenderBuffer[0].position.x);
	mReceiveBuffers.verticesNormalBegin      = &(mVertexRenderBuffer[0].normal.x);
	mReceiveBuffers.verticesPosByteStride    = sizeof(RenderBufferVertexElement);
	mReceiveBuffers.verticesNormalByteStride = sizeof(RenderBufferVertexElement);
	mReceiveBuffers.maxVertices              = mMaxVertices;
	mReceiveBuffers.numVerticesPtr           = &mNumVertices;
	NxU32 maxIndices = 3*numTriangles;
	mReceiveBuffers.indicesBegin             = mIndexRenderBuffer;
	mReceiveBuffers.indicesByteStride        = sizeof(NxU32);
	mReceiveBuffers.maxIndices               = maxIndices;
	mReceiveBuffers.numIndicesPtr            = &mNumIndices;
	if (mNumTempTexCoords > 0)
	{	
		assert(mNumTempTexCoords == numVertices);
		for (NxU32 i = 0; i < mNumTempTexCoords; i++)
		{
			mVertexRenderBuffer[i].texCoord[0] = mTempTexCoords[2*i+0];
			mVertexRenderBuffer[i].texCoord[1] = mTempTexCoords[2*i+1];
		}	
		mNumTempTexCoords = 0;
		free (mTempTexCoords);
		mTempTexCoords = NULL;
	}
	mReceiveBuffers.parentIndicesBegin       = (NxU32*)malloc(sizeof(NxU32)*mMaxVertices);
	mReceiveBuffers.parentIndicesByteStride  = sizeof(NxU32);
	mReceiveBuffers.maxParentIndices         = mMaxVertices;
	mReceiveBuffers.numParentIndicesPtr      = &mNumParentIndices;
	mReceiveBuffers.dirtyBufferFlagsPtr = &mMeshDirtyFlags;	
	mMeshDirtyFlags = 0;
	mNumVertices = 0;
	mNumIndices = 0;
	mNumParentIndices = 0;
}
bool CPhysXCloth::createTexture(std::string filename)
{
	BmpLoader bl;
	if (!bl.loadBmp(filename)) return false;

	GLuint gl_int;
	glGenTextures(1, &gl_int);
	mTexId = (int)gl_int;
	if (!mTexId) return false;
	glBindTexture(GL_TEXTURE_2D, mTexId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	bl.mWidth, bl.mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bl.mRGB);

	return true;
}
void CPhysXCloth::releaseReceiveBuffers()
{
	free (mReceiveBuffers.parentIndicesBegin);

	mReceiveBuffers.setToDefault();
}
void CPhysXCloth::updateTextureCoordinates()
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
CPhysXCloth* createFreeCloth(NxScene* scene, osg::Vec3f origin)
{
	//CPhysXCloth *regularCloth = new CPhysXCloth(scene, origin, 8.0f, 8.0f, 0.2f, NULL, false, PrimitiveSetType_Points);
	//if (!regularCloth->getNxCloth())
	//{
	//	printf("Error: Unable to create the cloth for the current scene.\n");
	//	delete regularCloth;
	//} 
	//else
	//{		
	//	//regularCloth->getNxCloth()->attachToShape(*capsule->getShapes(), 0);
	//}
	//return regularCloth;
	return NULL;
}
CPhysXCloth* createAttachedCloth(NxScene* scene, osg::Vec3f origin)
{
	/*float w = 8.0f;
	float h = 8.0f;
	float d = 0.2f;
	CPhysXCloth *regularCloth = new CPhysXCloth(scene, origin, w, h, d, NULL, false, PrimitiveSetType_Points);
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
	}*/
	//return regularCloth;

	return NULL;
}