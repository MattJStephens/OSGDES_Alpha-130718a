/*
	This class is an NxFluid class example. Uses osg & PhysX.
	Contains some original code from the PhysX demo - samples project
*/
#ifndef _FLUID_H_
#define _FLUID_H_

#include "NxPhysics.h"

#include <osg/MatrixTransform>
#include <osg/StateSet>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Program>
#include <osg/PointSprite>
#include <osg/PolygonStipple>
#include <osg/Point>
#include <osg/StateSet>


#define MAX_PARTICLES 4000

static NxReal gGenerationRadius = 20;

static NxVec3 gCenter(0,0,0);
static NxReal gPlanetRadius = 3;
static NxReal gAtmosphereRadius = gPlanetRadius + 0.1f;
static NxVec3 gRotationAxis(0.2,1,0.3);

static NxReal gRotationSpeed  = 1.5f;
static NxReal gGravityPlanet = 8.0f;
static NxReal gFriction = 0.005f;
static NxReal gViscosity = 60.0f;

struct ParticleSDK
{
	NxVec3	position;
	NxVec3  velocity;
	NxReal	density;
	NxReal  lifetime;
	NxU32	id;
	NxVec3	collisionNormal;
};


struct ParticleUpdateSDK
{
	NxVec3	force;
	NxU32   flags;
};

static ParticleUpdateSDK* gUpdates = NULL;
struct MyParticle
{
	float rgba[4];
};


class FluidCallback;

class CPhysXFluid : public osg::MatrixTransform
{
public:
	CPhysXFluid(NxScene* scene, osg::Vec3f origin, osg::Vec3f gravity);
	NxFluid*				getNxFluid() { return mFluid; }	
	const ParticleSDK*		getParticles() { return mParticleBuffer; }
	unsigned				getParticlesNum() { return mParticleBufferNum; }
	MyParticle*				getMyParticles() { return mMyParticleBuffer; }

	const unsigned*			getCreatedIds() { return mCreatedParticleIds; }
	unsigned				getCreatedIdsNum() { return mCreatedParticleIdsNum; }

	const unsigned*			getDeletedIds() { return mDeletedParticleIds; }
	unsigned				getDeletedIdsNum() { return mDeletedParticleIdsNum; }

	void					setParticleSize(NxReal size) { mParticleSize = size; }
	NxReal					getParticleSize() { return mParticleSize; }
	
public:
	~CPhysXFluid(void);
private:
	osg::Geode* createSphere(osg::Vec3f origin);
	void initialize(NxFluidDesc &desc, bool trackUserData, bool provideCollisionNormals, const NxVec3& particleColor, NxReal particleSize);
	NxScene* _scene;
	osg::Vec3f  _origin;
	osg::Geode* _geode;
	std::vector<NxVec3> positions;

	unsigned			mParticleBufferNum;
	ParticleSDK*		mParticleBuffer;
	NxFluid*			mFluid;

	NxVec3				mParticleColor;
	NxReal				mParticleSize;
	NxU32				mMaxParticles;

	/**
	These fields are only relevant for tracking user partilce data (MyParticle)
	*/
	bool				mTrackUserData;
	MyParticle*			mMyParticleBuffer;
	NxU32				mCreatedParticleIdsNum;
	NxU32*				mCreatedParticleIds;
	NxU32				mDeletedParticleIdsNum;
	NxU32*				mDeletedParticleIds;

	//rendering
	float*				mRenderBuffer;
	float*				mRenderBufferUserData;

};

osg::Group* createDemoFluid(NxScene* scene, osg::Vec3f origin);
osg::Group* createDemoFluid2(NxScene* scene, osg::Vec3f origin, float size=10);
NxVec3 RandNormalVec(const NxVec3& mu, NxReal sig);

// particle updater
static float vmax = 5.0f;

struct FluidUpdateCallback : public osg::Drawable::UpdateCallback
{
public:
	FluidUpdateCallback( NxScene* scene, CPhysXFluid* fluid) : 
	  _scene( scene ),
		  _fluid( fluid )
	  {

	  }
	  /* need to get control over individual vertices */
	  virtual void update(osg::NodeVisitor*, osg::Drawable* drawable)
	  {		  

		  osg::Geometry* geo = dynamic_cast<osg::Geometry*>(drawable);
		  if (!geo) return;		  

		  if (!gUpdates)
		  {
			  gUpdates = new ParticleUpdateSDK[MAX_PARTICLES];
		  }

		  //Get particles from MyFluid and compute forces.
		  const ParticleSDK* particles = _fluid->getParticles();
		  unsigned particlesNum = _fluid->getParticlesNum();

		  NxReal planetRadius4 = gPlanetRadius*gPlanetRadius*gPlanetRadius*gPlanetRadius;								

		  osg::Vec3Array* vertices = new osg::Vec3Array;	
		  osg::Vec4Array* colors = new osg::Vec4Array;	

		  for (unsigned i=0; i<particlesNum; i++)
		  {
			  const ParticleSDK& particle = particles[i];
			  ParticleUpdateSDK& update = gUpdates[i];
			  NxVec3& force = update.force;
			  force.set(0,0,0);
			  NxU32& flags = update.flags;
			  flags = 0;
			  //Dist vector
			  NxVec3 distVec = particle.position - gCenter;

			  vertices->push_back(osg::Vec3f(particle.position.x, particle.position.z, 
				  particle.position.y));
			  float v = particle.velocity.magnitude()/vmax;
			  if (v)
			  {
					float r = __max(1.0f - .25f*v,0.0);
					float g = __max(1.0f - .5f*v,0.0);
					float b = __max(1.0f -  v,0.0);
					osg::Vec4f c(r, g, b, 0.8f );
					colors->push_back(c);
			  }

			  NxReal dist = distVec.normalize();

			  if (dist > gGenerationRadius*2)
			  {
				  flags |= NX_FP_DELETE;
				  continue;
			  }

			  NxReal dist2 = dist*dist;
			  NxReal dist4 = dist2*dist2;

			  //Tangental vectors:
			  NxVec3 tan0Vec = distVec.cross(gRotationAxis);
			  tan0Vec.normalize();
			  NxVec3 tan1Vec = tan0Vec.cross(distVec);
			  tan1Vec.normalize();

			  //Collision and gravity
			  if (dist < gPlanetRadius)
			  {
				  NxReal dot = particle.velocity.dot(distVec);
				  if (dot < 0.0f)
					  force -= distVec*(dot);
			  }
			  else
			  {
				  force -= distVec*(gGravityPlanet/dist2);
			  }

			  if (dist > gAtmosphereRadius)
				  continue;

			  //Friction with planet surface
			  //Compute local planet speed.
			  NxReal temp = distVec.dot(gRotationAxis);
			  NxReal planetVelAbs = (1 - temp*temp) * gRotationSpeed;
			  NxVec3 planetVel = tan0Vec * planetVelAbs;

			  //project particle velocity onto planet plane.
			  NxVec3 projVel = particle.velocity - distVec * particle.velocity.dot(distVec);

			  NxVec3 velDiff = planetVel - projVel;
			  NxReal velDiffAbs = velDiff.magnitude();

			  NxVec3 drag = velDiff * NxMath::min(1.0f, gFriction*velDiffAbs*velDiffAbs);
			  force += drag;
		  }

		  geo->setVertexArray(vertices);
		  geo->setColorArray(colors);

		  NxParticleUpdateData updateData;
		  updateData.forceMode = NX_VELOCITY_CHANGE;
		  updateData.bufferForce = &gUpdates[0].force.x;
		  updateData.bufferForceByteStride = sizeof(ParticleUpdateSDK);
		  updateData.bufferFlag = &gUpdates[0].flags;
		  updateData.bufferFlagByteStride = sizeof(ParticleUpdateSDK);

		  _fluid->getNxFluid()->updateParticles(updateData);


	  }
private:
	CPhysXFluid* _fluid;
	NxScene* _scene;
};


#endif