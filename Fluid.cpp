#include "Fluid.h"
#include "PhysXPlane.h"
#include "PXTools.h"

Fluid::Fluid(NxScene* scene, osg::Vec3f origin, osg::Vec3f gravity=osg::Vec3f(0,0,0)) : 
_scene(scene), _origin(origin)
{
	bool gShadows = false;
	bool gHardwareSimulation = false;

	//Create a set of initial particles
	ParticleSDK*	initParticles = new ParticleSDK[MAX_PARTICLES];
	unsigned initParticlesNum = 0;

	//remove gravity.
	_scene->setGravity(NxVec3(gravity.x(),gravity.z(),gravity.y()));

	gRotationAxis.normalize();

	while (initParticlesNum < MAX_PARTICLES)
	{
		NxVec3 pos;

		pos = RandNormalVec(NxVec3(0,0,0), gGenerationRadius/24);

		if (pos.magnitudeSquared() < gPlanetRadius*gPlanetRadius)
			continue;

		if (pos.magnitudeSquared() > gGenerationRadius*gGenerationRadius)
			continue;

		ParticleSDK& newParticle = initParticles[initParticlesNum++];

		gCenter = NxVec3(origin.x(), origin.z(),origin.y());
		newParticle.position = gCenter + pos;
		positions.push_back( newParticle.position );
		newParticle.velocity = RandNormalVec(NxVec3(0,0,0), 0.2f);
	}

	//Setup structure to pass initial particles.
	NxParticleData initParticleData;

	initParticleData.numParticlesPtr		= &initParticlesNum;

	initParticleData.bufferPos				= &initParticles[0].position.x;
	initParticleData.bufferPosByteStride	= sizeof(ParticleSDK);

	initParticleData.bufferVel				= &initParticles[0].velocity.x;
	initParticleData.bufferVelByteStride	= sizeof(ParticleSDK);

	//Setup fluid descriptor
	NxFluidDesc fluidDesc;
	fluidDesc.maxParticles                  = MAX_PARTICLES;
	fluidDesc.kernelRadiusMultiplier		= 2.0f;
	fluidDesc.restParticlesPerMeter			= 3.6f;
	fluidDesc.motionLimitMultiplier			= 2.0f;
	fluidDesc.packetSizeMultiplier			= 8;
	fluidDesc.stiffness						= 100.0f;
	fluidDesc.viscosity						= gViscosity;
	fluidDesc.restDensity					= 1000.0f;
	fluidDesc.damping						= 0.0001f;
	fluidDesc.simulationMethod				= NX_F_SPH;
	if (!gHardwareSimulation)
		fluidDesc.flags &= ~NX_FF_HARDWARE;

	fluidDesc.initialParticleData			= initParticleData;

	//Create user fluid.
	//- create NxFluid in NxScene
	//- setup the buffers to read from data from the SDK
	//- set NxFluid::userData field to MyFluid instance
	NxVec3 color(0,0,0);
	bool trackUserData = false;
	bool provideCollisionNormals = false;
	initialize(fluidDesc, trackUserData, provideCollisionNormals, color, 0.04f);	

	delete[] initParticles;

	setMatrix(osg::Matrix::identity());

	_geode = createSphere(origin);
	_geode->getDrawable(0)->setUpdateCallback( new FluidUpdateCallback( _scene, this ));

	addChild( _geode );
}

Fluid::~Fluid(void)
{
	delete[] mParticleBuffer;
	mParticleBuffer = NULL;
	_scene->releaseFluid(*mFluid);

	if (mTrackUserData)
	{
		delete[] mMyParticleBuffer;
		mMyParticleBuffer = NULL;
		delete[] mCreatedParticleIds;
		mCreatedParticleIds = NULL;
		delete[] mDeletedParticleIds;
		mDeletedParticleIds = NULL;
	}

	//if (mRenderBuffer)
	//{
	//	delete[] mRenderBuffer;
	//	mRenderBuffer = 0;
	//}
}


osg::Geode* Fluid::createSphere(osg::Vec3f origin)
{
	osg::Geode* geode = new osg::Geode;
	osg::Geometry* pointsGeom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array;		


		int i,j;
	// create particles - POINTS
	{
		

		for (int i=0;i<positions.size(); i++)
		{
			osg::Vec3f p = osg::Vec3f(positions[i].x, positions[i].z, positions[i].y);
			vertices->push_back(p);
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
		pointsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,positions.size()));
		

		osg::Point* point = new osg::Point;
        point->setDistanceAttenuation(osg::Vec3(0.0,0.0000,0.05f));
		point->setSize(20.0f);
        stateSet->setAttribute(point);

		osg::PointSprite *sprite = new osg::PointSprite();
        stateSet->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);

		

		// add the points geometry to the geode.
		geode->addDrawable(pointsGeom);
	}

	return geode;
}


void Fluid::initialize(NxFluidDesc &desc, bool trackUserData, bool provideCollisionNormals, const NxVec3 &particleColor, NxReal particleSize)
{

	mMaxParticles = desc.maxParticles;
	mParticleBuffer = new ParticleSDK[mMaxParticles];
	desc.userData = this;

	//Setup particle write data.
	NxParticleData particleData;
	particleData.numParticlesPtr = &mParticleBufferNum;
	particleData.bufferPos = &mParticleBuffer[0].position.x;
	particleData.bufferPosByteStride = sizeof(ParticleSDK);
	particleData.bufferVel = &mParticleBuffer[0].velocity.x;
	particleData.bufferVelByteStride = sizeof(ParticleSDK);
	particleData.bufferDensity = &mParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(ParticleSDK);
	particleData.bufferLife = &mParticleBuffer[0].lifetime;
	particleData.bufferLifeByteStride = sizeof(ParticleSDK);
	particleData.bufferId = &mParticleBuffer[0].id;
	particleData.bufferIdByteStride = sizeof(ParticleSDK);

	if (provideCollisionNormals)
	{
		particleData.bufferCollisionNormal = &mParticleBuffer[0].collisionNormal.x;
		particleData.bufferCollisionNormalByteStride = sizeof(ParticleSDK);
	}

	desc.particlesWriteData = particleData;

	//User data buffers
	if (mTrackUserData)
	{
		mMyParticleBuffer = new MyParticle[mMaxParticles];
		mCreatedParticleIds = new NxU32[mMaxParticles];
		mDeletedParticleIds = new NxU32[mMaxParticles];

		//Setup id write data.
		NxParticleIdData idData;

		//Creation
		idData.numIdsPtr = &mCreatedParticleIdsNum;
		idData.bufferId = mCreatedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		desc.particleCreationIdWriteData = idData;

		//Deletion
		idData.numIdsPtr = &mDeletedParticleIdsNum;
		idData.bufferId = mDeletedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		desc.particleDeletionIdWriteData = idData;
	}

	mFluid = _scene->createFluid(desc);
	assert(mFluid);

}

/*demo scene*/
osg::Group* createDemoFluid(NxScene* scene, osg::Vec3f origin)
{
	osg::Group* group = new osg::Group;

	Fluid* fluid = new Fluid(scene, origin);

	group->addChild(fluid);
	return fluid;
}

osg::Group* createDemoFluid2(NxScene* scene, osg::Vec3f origin, float size)
{
	osg::Group* group = new osg::Group;

	CPhysXPlane* plane = new CPhysXPlane(scene, origin, size);

	group->addChild(plane);
	Fluid* fluid = new Fluid(scene, osg::Vec3f(5,5,5), osg::Vec3f(0,0,-19.81));
	group->addChild(fluid);

	return group;
}

