#include "PhysXFluid.h"
#include "PhysXPlane.h"
#include "PXTools.h"

CPhysXFluid::CPhysXFluid(NxScene* scene) 
{		
	m_scene = scene;
	
	m_origin =osg::Vec3f(0,0,0);
	m_gravity =osg::Vec3f(0,0,0);
	m_color= osg::Vec4(1.0f,1.0f,0.0f,0.8f);
	m_distance_attenuation =osg::Vec3(0.0,0.0000,0.05f);
	mCenter = NxVec3(0,0,0);
	mRotationAxis= NxVec3(0.2,1,0.3);
	
	mGenerationRadius = 20;	
	mPlanetRadius = 3;
	mAtmosphereRadius = mPlanetRadius + 0.1f;
	mRotationSpeed  = 1.5f;
	mGravityPlanet = 8.0f;
	mFriction = 0.005f;
	mViscosity = 60.0f;
	mKernelRadiusMultiplier= 2.0f;
	mRestParticlesPerMeter = 3.6f;
	mMotionLimitMultiplier = 2.0f;
	mPacketSizeMultiplier = 8;
	mStiffness	= 100.0f;
	mRestDensity = 1000.0f;
	mDamping = 0.0001f;
	mParticleSize = 20;
	m_point_size = 20.0f;
}

CPhysXFluid::~CPhysXFluid(void)
{
	delete[] mParticleBuffer;
	mParticleBuffer = NULL;

	if(mFluid!= NULL)
	{
		//m_scene->releaseFluid(*mFluid);
	}
	if (mTrackUserData)
	{
		delete[] mMyParticleBuffer;
		mMyParticleBuffer = NULL;
		delete[] mCreatedParticleIds;
		mCreatedParticleIds = NULL;
		delete[] mDeletedParticleIds;
		mDeletedParticleIds = NULL;
	}	
}

void CPhysXFluid::createSphere(osg::Vec3f origin)
{	
	m_geode = new osg::Geode;
	m_pointsGeom = new osg::Geometry();
	m_vertices = new osg::Vec3Array;		
	m_normals = new osg::Vec3Array;		
	m_colors = new osg::Vec4Array;	
	m_stateSet = new osg::StateSet();    	
	m_point = new osg::Point;	
	m_sprite = new osg::PointSprite();
	m_draw_arrays = new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, positions.size());

	for (std::size_t i= 0; i< positions.size(); i++)
	{
		osg::Vec3f p = osg::Vec3f(positions[i].x, positions[i].z, positions[i].y);
		m_vertices->push_back(p);
	}
	m_pointsGeom->setVertexArray(m_vertices);	
	m_colors->push_back(m_color);
	m_pointsGeom->setColorArray(m_colors);
	m_pointsGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	m_pointsGeom->setNormalArray(m_normals);
	m_pointsGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);			
	m_pointsGeom->setStateSet(m_stateSet);
	m_pointsGeom->addPrimitiveSet(m_draw_arrays);	
    m_point->setDistanceAttenuation(m_distance_attenuation);
	m_point->setSize(mParticleSize);
    m_stateSet->setAttribute(m_point);		
    m_stateSet->setTextureAttributeAndModes(0, m_sprite, osg::StateAttribute::ON);	
	m_geode->addDrawable(m_pointsGeom);	
}


void CPhysXFluid::initialize()
{
	bool gShadows = false;
	bool gHardwareSimulation = false;
	bool trackUserData = false;
	bool provideCollisionNormals = false;
	unsigned initParticlesNum = 0;

	ParticleSDK* initParticles = new ParticleSDK[MAX_PARTICLES];	
	m_scene->setGravity(NxVec3(m_gravity.x(),m_gravity.z(),m_gravity.y()));
	mRotationAxis.normalize();
	while (initParticlesNum < MAX_PARTICLES)
	{
		NxVec3 pos;
		pos = RandNormalVec(NxVec3(0,0,0), mGenerationRadius);
		if (pos.magnitudeSquared() < mPlanetRadius*mPlanetRadius)
			continue;
		if (pos.magnitudeSquared() > mGenerationRadius*mGenerationRadius)
			continue;
		ParticleSDK& newParticle = initParticles[initParticlesNum++];
		mCenter = NxVec3(m_origin.x(), m_origin.z(), m_origin.y());
		newParticle.position = mCenter + pos;
		positions.push_back( newParticle.position );
		newParticle.velocity = RandNormalVec(NxVec3(0,0,0), 0.2f);
	}
	
	NxParticleData initParticleData;
	initParticleData.numParticlesPtr = &initParticlesNum;
	initParticleData.bufferPos = &initParticles[0].position.x;
	initParticleData.bufferPosByteStride = sizeof(ParticleSDK);
	initParticleData.bufferVel = &initParticles[0].velocity.x;
	initParticleData.bufferVelByteStride = sizeof(ParticleSDK);

	//Setup fluid descriptor
	NxFluidDesc fluidDesc;
	fluidDesc.maxParticles = MAX_PARTICLES;
	fluidDesc.kernelRadiusMultiplier= mKernelRadiusMultiplier;
	fluidDesc.restParticlesPerMeter = mRestParticlesPerMeter;
	fluidDesc.motionLimitMultiplier = mMotionLimitMultiplier;
	fluidDesc.packetSizeMultiplier = mPacketSizeMultiplier;
	fluidDesc.stiffness	= mStiffness;
	fluidDesc.viscosity	= mViscosity;
	fluidDesc.restDensity = mRestDensity;
	fluidDesc.damping = mDamping;
	fluidDesc.simulationMethod = NX_F_SPH;
	if (!gHardwareSimulation)
	{
		fluidDesc.flags &= ~NX_FF_HARDWARE;
	}
	fluidDesc.initialParticleData= initParticleData;		
	fluidDesc.userData = this;	
	
	mMaxParticles = fluidDesc.maxParticles;
	mParticleBuffer = new ParticleSDK[mMaxParticles];	

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

	fluidDesc.particlesWriteData = particleData;

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
		fluidDesc.particleCreationIdWriteData = idData;

		//Deletion
		idData.numIdsPtr = &mDeletedParticleIdsNum;
		idData.bufferId = mDeletedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		fluidDesc.particleDeletionIdWriteData = idData;
	}

	mFluid = m_scene->createFluid(fluidDesc);
	assert(mFluid);

	delete[] initParticles;
	setMatrix(osg::Matrix::identity());
	createSphere(m_origin);
	m_geode->getDrawable(0)->setUpdateCallback( new FluidUpdateCallback( m_scene, this ));
	addChild( m_geode );

}


void update_fluid(CPhysXFluid* p_fluid, osg::Drawable* p_drawable)
{
	osg::Geometry* geo = dynamic_cast<osg::Geometry*>(p_drawable);
	if (!geo) 
	{
		return;		  
	}
	if (!gUpdates)
	{
		gUpdates = new ParticleUpdateSDK[MAX_PARTICLES];
	}	
	const ParticleSDK* particles = p_fluid->getParticles();
	unsigned particlesNum = p_fluid->getParticlesNum();
	osg::ref_ptr<osg::Vec3Array> vertices= new osg::Vec3Array;	
	osg::ref_ptr<osg::Vec4Array> colors= new osg::Vec4Array;	
	for (unsigned i=0; i < particlesNum; i++)
	{
		const ParticleSDK& particle = particles[i];
		ParticleUpdateSDK& update = gUpdates[i];
		NxVec3& force = update.force;
		force.set(0,0,0);
		NxU32& flags = update.flags;
		flags = 0;		
		NxVec3 distVec = particle.position - p_fluid->mCenter;
		vertices->push_back(osg::Vec3f(particle.position.x, particle.position.z, particle.position.y));
		colors->push_back(p_fluid->m_color);	
	}
	geo->setVertexArray(vertices);
	geo->setColorArray(colors);
	NxParticleUpdateData updateData;
	updateData.forceMode = NX_VELOCITY_CHANGE;
	updateData.bufferForce = &gUpdates[0].force.x;
	updateData.bufferForceByteStride = sizeof(ParticleUpdateSDK);
	updateData.bufferFlag = &gUpdates[0].flags;
	updateData.bufferFlagByteStride = sizeof(ParticleUpdateSDK);
	p_fluid->getNxFluid()->updateParticles(updateData);
}


	/*
		float v = particle.velocity.magnitude()/vmax;
		if (v)
		{
			float r = __max(1.0f - .25f*v,0.0);
			float g = __max(1.0f - .5f*v,0.0);
			float b = __max(1.0f -  v,0.0);
			osg::Vec4f c(r, g, b, 0.8f );
			colors->push_back(c);
		}\*/

	/*
		NxReal dist = distVec.normalize();
		if (dist > p_fluid->mGenerationRadius*2)
		{
			flags |= NX_FP_DELETE;
			continue;
		}
		NxReal dist2 = dist*dist;
		NxReal dist4 = dist2*dist2;			
		NxVec3 tan0Vec = distVec.cross(p_fluid->mRotationAxis);
		tan0Vec.normalize();
		NxVec3 tan1Vec = tan0Vec.cross(distVec);
		tan1Vec.normalize();
		if (dist < p_fluid->mPlanetRadius)
		{
			NxReal dot = particle.velocity.dot(distVec);
			if (dot < 0.0f)
			{
				force -= distVec*(dot);
			}
		}
		else
		{
			force -= distVec*(p_fluid->mGravityPlanet/dist2);
		}
		if (dist > p_fluid->mAtmosphereRadius)
		{
			continue;
		}			
		NxReal temp = distVec.dot(p_fluid->mRotationAxis);
		NxReal planetVelAbs = (1 - temp*temp) * p_fluid->mRotationSpeed;
		NxVec3 planetVel = tan0Vec * planetVelAbs;
		NxVec3 projVel = particle.velocity - distVec * particle.velocity.dot(distVec);
		NxVec3 velDiff = planetVel - projVel;
		NxReal velDiffAbs = velDiff.magnitude();
		NxVec3 drag = velDiff * NxMath::min(1.0f, p_fluid->mFriction*velDiffAbs*velDiffAbs);
		force += drag;
		*/