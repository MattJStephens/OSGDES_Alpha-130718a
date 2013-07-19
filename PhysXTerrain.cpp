/* Modified CPhysXTerrain class - PhysX example, osg part by Janusz Goldasz */
#include "PhysXTerrain.h"
#include "PhysXSphere.h"

TerrainData::TerrainData() :
size	(0),
nbVerts	(0),
nbFaces	(0),
offset	(0.0f),
width	(0.0f),
chaos	(0.0f),
verts	(NULL),
colors	(NULL),
normals	(NULL),
faces	(NULL)
{

}


TerrainData::~TerrainData()
{
	release();
}

void TerrainData::release()
{
	NX_DELETE_ARRAY(normals);
	NX_DELETE_ARRAY(faces);
	NX_DELETE_ARRAY(colors);
	NX_DELETE_ARRAY(verts);
}

void TerrainData::init(NxU32 s, NxF32 o, NxF32 w, NxF32 c, bool flat, const NxVec3* pos)
{
	release();

	size	= s;
	offset	= o;
	width	= w;
	chaos	= c;
	nbVerts = size*size;
	nbFaces	= (size-1)*(size-1)*2;

	////////

	// Initialize terrain vertices
	verts = new NxVec3[nbVerts];
	for(NxU32 y=0;y<size;y++)
	{
		for(NxU32 x=0;x<size;x++)
		{
			verts[x+y*size] = NxVec3(NxF32(x)-(NxF32(size-1)*0.5f), 0.0f, NxF32(y)-(NxF32(size-1)*0.5f)) * width;
		}
	}

	// Initialize terrain colors
	{
		colors = new NxVec3[nbVerts];
		for(NxU32 y=0;y<size;y++)
		{
			for(NxU32 x=0;x<size;x++)
			{
				colors[x+y*size] = NxVec3(0.5f, 0.4f, 0.2f);
			}
		}
	}

	// Initialize terrain faces
	faces = new NxU32[nbFaces*3];
	NxU32 k = 0;
	for(NxU32 j=0;j<size-1;j++)
	{
		for(NxU32 i=0;i<size-1;i++)
		{
			// Create first triangle
			faces[k++] = i   + j*size;
			faces[k++] = i   + (j+1)*size;
			faces[k++] = i+1 + (j+1)*size;

			// Create second triangle
			faces[k++] = i   + j*size;
			faces[k++] = i+1 + (j+1)*size;
			faces[k++] = i+1 + j*size;
		}
	}

	struct Local
	{
		static void _Compute(bool* done, NxVec3* field, NxU32 x0, NxU32 y0, NxU32 currentSize, NxF32 value, NxU32 initSize)
		{
			// Compute new size
			currentSize>>=1;
			if(!currentSize) return;

			// Compute new heights
			NxF32 v0 = (value * NxF32(rand()-RAND_MAX_OVER_TWO) * ONE_OVER_RAND_MAX);
			NxF32 v1 = (value * NxF32(rand()-RAND_MAX_OVER_TWO) * ONE_OVER_RAND_MAX);
			NxF32 v2 = (value * NxF32(rand()-RAND_MAX_OVER_TWO) * ONE_OVER_RAND_MAX);
			NxF32 v3 = (value * NxF32(rand()-RAND_MAX_OVER_TWO) * ONE_OVER_RAND_MAX);
			NxF32 v4 = (value * NxF32(rand()-RAND_MAX_OVER_TWO) * ONE_OVER_RAND_MAX);

			NxU32 x1 = (x0+currentSize)				% initSize;
			NxU32 x2 = (x0+currentSize+currentSize)	% initSize;
			NxU32 y1 = (y0+currentSize)				% initSize;
			NxU32 y2 = (y0+currentSize+currentSize)	% initSize;

			if(!done[x1 + y0*initSize])	field[x1 + y0*initSize].y = v0 + 0.5f * (field[x0 + y0*initSize].y + field[x2 + y0*initSize].y);
			if(!done[x0 + y1*initSize])	field[x0 + y1*initSize].y = v1 + 0.5f * (field[x0 + y0*initSize].y + field[x0 + y2*initSize].y);
			if(!done[x2 + y1*initSize])	field[x2 + y1*initSize].y = v2 + 0.5f * (field[x2 + y0*initSize].y + field[x2 + y2*initSize].y);
			if(!done[x1 + y2*initSize])	field[x1 + y2*initSize].y = v3 + 0.5f * (field[x0 + y2*initSize].y + field[x2 + y2*initSize].y);
			if(!done[x1 + y1*initSize])	field[x1 + y1*initSize].y = v4 + 0.5f * (field[x0 + y1*initSize].y + field[x2 + y1*initSize].y);

			done[x1 + y0*initSize] = true;
			done[x0 + y1*initSize] = true;
			done[x2 + y1*initSize] = true;
			done[x1 + y2*initSize] = true;
			done[x1 + y1*initSize] = true;

			// Recurse through 4 corners
			value *= 0.5f;
			_Compute(done, field, x0,	y0,	currentSize, value, initSize);
			_Compute(done, field, x0,	y1,	currentSize, value, initSize);
			_Compute(done, field, x1,	y0,	currentSize, value, initSize);
			_Compute(done, field, x1,	y1,	currentSize, value, initSize);
		}
	};

	// Fractalize
	bool* done = new bool[nbVerts];
	memset(done,0,nbVerts*sizeof(bool));
	verts[0].y = 10.0f;
	verts[size-1].y = 10.0f;
	verts[size*(size-1)].y = 10.0f;
	verts[nbVerts-1].y = 10.0f;
	Local::_Compute(done, verts, 0, 0, size, chaos, size);
	for(NxU32 i=0;i<nbVerts;i++)	verts[i].y += offset;
	delete[] done;
	done=NULL;

	// Build vertex normals
	normals = new NxVec3[nbVerts];
	NxBuildSmoothNormals(nbFaces, nbVerts, verts, faces, NULL, normals, true);
}

// since we have build the physics component, time to create the visual part
osg::Geode* TerrainData::createVisualTerrain(int numRows, int numColumns)
{
	osg::Geode* geode = new osg::Geode;
	osg::Geometry* geometry = new osg::Geometry;

	osg::Vec3Array& v = *(new osg::Vec3Array(numColumns*numRows));
	osg::Vec2Array& t = *(new osg::Vec2Array(numColumns*numRows));
	osg::Vec4ubArray& color = *(new osg::Vec4ubArray(1));

	color[0].set(255,255,255,255);

	float rowCoordDelta = width/(float)(numRows-1);
	float columnCoordDelta = width/(float)(numColumns-1);

	float rowTexDelta = 1.0f/(float)(numRows-1);
	float columnTexDelta = 1.0f/(float)(numColumns-1);

	osg::Vec3f origin = osg::Vec3f(0,0,0);// unused
	osg::Vec3 pos = osg::Vec3f(0.0f,0.0f,0.0f); // unused
	osg::Vec2 tex(1.0f,1.0f);
	int vi=0;
	
	float scale_z = 1.0f; //tmp	

	for(int r=0;r<numRows;++r)
	{		
		tex.x() = 0.0f;
		for(int c=0;c<numColumns;++c)
		{
			// y, z are swapped due to PhysX using a different coordinate system
			v[vi].set(verts[r+c*numRows].x,verts[r+c*numRows].z,(verts[r+c*numRows].y));
			t[vi].set(tex.x(),tex.y());			
			tex.x()-=columnTexDelta;
			++vi;
		}
		tex.y() -= rowTexDelta;
	}

	geometry->setVertexArray(&v);
	geometry->setColorArray(&color);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	geometry->setTexCoordArray(0,&t);

	for(int r=0;r<numRows-1;++r)
	{
		osg::DrawElementsUShort& drawElements = *(new osg::DrawElementsUShort(GL_QUAD_STRIP,2*numColumns));
		geometry->addPrimitiveSet(&drawElements);
		int ei=0;
		for(int c=0;c<numColumns;++c)
		{
			drawElements[ei++] = (r+1)*numColumns+c;
			drawElements[ei++] = (r)*numColumns+c;
		}
	}

	// osg part
	geode->addDrawable(geometry);

	osg::LightModel* lightModel = new osg::LightModel();
	lightModel->setTwoSided ( true );    

	osg::Material* material = new osg::Material;
    material->setColorMode ( osg::Material::OFF );
    material->setEmission ( osg::Material::BACK, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );

	   
	geode->getOrCreateStateSet()->setAttributeAndModes( lightModel, osg::StateAttribute::ON );
	geode->getOrCreateStateSet()->setAttributeAndModes( material, osg::StateAttribute::ON );
	osgUtil::SmoothingVisitor sv;
	sv.smooth(*geometry);
	return geode;
}


osg::Geode* CPhysXTerrain::createPhysicsTerrain(NxPhysicsSDK* gPhysicsSDK, NxScene* scene, osg::Vec3f origin)
{
	NxTriangleMeshShapeDesc terrainShapeDesc;
	NxTriangleMesh* terrainMesh = NULL;
	NxTriangleMeshDesc terrainDesc;	
	NxActorDesc ActorDesc;
	MemoryWriteBuffer buf;
	static ErrorStream gMyErrorStream;
	static UserAllocator* gMyAllocator = NULL;

	_gTerrainData = new TerrainData;
	_gTerrainData->init(TERRAIN_SIZE, TERRAIN_OFFSET, TERRAIN_WIDTH, TERRAIN_CHAOS);
	terrainDesc.numVertices	= _gTerrainData->nbVerts;
	terrainDesc.numTriangles = _gTerrainData->nbFaces;
	terrainDesc.pointStrideBytes = sizeof(NxVec3);
	terrainDesc.triangleStrideBytes	= 3*sizeof(NxU32);
	terrainDesc.points = _gTerrainData->verts;
	terrainDesc.triangles = _gTerrainData->faces;
	terrainDesc.flags = 0;
	terrainDesc.heightFieldVerticalAxis	= NX_Y;
	terrainDesc.heightFieldVerticalExtent= -1000.0f;
	bool status = InitCooking(gMyAllocator, &gMyErrorStream);
	if (!status) {
		printf("Unable to initialize the cooking library. Please make sure that you have correctly installed the latest version of the AGEIA PhysX SDK.");
		exit(1);
	}	
	status = CookTriangleMesh(terrainDesc, buf);
	if (!status) {
		printf("Unable to cook a triangle mesh.");
		exit(1);
	}
	MemoryReadBuffer readBuffer(buf.data);
	terrainMesh = gPhysicsSDK->createTriangleMesh(readBuffer);
	terrainShapeDesc.meshData = terrainMesh;
	terrainShapeDesc.shapeFlags	= NX_SF_FEATURE_INDICES;
	ActorDesc.shapes.pushBack(&terrainShapeDesc);	
	_gTerrain = scene->createActor(ActorDesc);
	_gTerrain->userData = (void*)0;

	CloseCooking();

	return _gTerrainData->createVisualTerrain(TERRAIN_SIZE, TERRAIN_SIZE);
}


/* demo scene */
osg::Group* createHeightfieldScene(NxPhysicsSDK* sdk, NxScene* scene, osg::Vec3f origin)
{
	osg::Group* group = new osg::Group;

	group->addChild(new CPhysXTerrain(sdk, scene, origin));
	group->addChild(distributeSpheres(scene, TERRAIN_SIZE));

	return group;
}