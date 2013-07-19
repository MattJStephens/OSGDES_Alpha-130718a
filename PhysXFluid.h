/*
	This class is an NxFluid class example. Uses osg & PhysX.
	Contains some original code from the PhysX demo - samples project
*/
#ifndef m_fluid_H_
#define m_fluid_H_

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
		NxVec3 mCenter;
		NxVec3 mRotationAxis;
		NxVec3 mParticleColor;		
		NxReal mPlanetRadius;
		NxReal mAtmosphereRadius;
		NxReal mGenerationRadius;		
		NxReal mRotationSpeed;
		NxReal mGravityPlanet;
		NxReal mFriction;
		NxReal mViscosity;	
		NxReal mKernelRadiusMultiplier;
		NxReal mRestParticlesPerMeter;
		NxReal mMotionLimitMultiplier;
		NxReal mPacketSizeMultiplier;
		NxReal mStiffness;
		NxReal mRestDensity;
		NxReal mDamping;
		NxReal mParticleSize;
		osg::Vec3f m_origin;
		osg::Vec3f m_gravity;
		osg::Vec4 m_color;
		osg::Vec3 m_distance_attenuation;
		float m_point_size;
	public:
		CPhysXFluid(NxScene* scene);
		void initialize();
		NxFluid* getNxFluid() { return mFluid; }	
		const ParticleSDK* getParticles() { return mParticleBuffer; }
		unsigned getParticlesNum() { return mParticleBufferNum; }
		MyParticle*	getMyParticles() { return mMyParticleBuffer; }
		const unsigned*	getCreatedIds() { return mCreatedParticleIds; }
		unsigned getCreatedIdsNum() { return mCreatedParticleIdsNum; }
		const unsigned*	getDeletedIds() { return mDeletedParticleIds; }
		unsigned getDeletedIdsNum() { return mDeletedParticleIdsNum; }
		void setParticleSize(NxReal size) { mParticleSize = size; }
		NxReal getParticleSize() { return mParticleSize; }
	
	public:
		~CPhysXFluid(void);
	private:
		void createSphere(osg::Vec3f origin);
		
		NxScene* m_scene;		
		std::vector<NxVec3> positions;
		unsigned mParticleBufferNum;
		ParticleSDK* mParticleBuffer;
		NxFluid* mFluid;		
		NxU32 mMaxParticles;
		bool mTrackUserData;
		MyParticle*	mMyParticleBuffer;
		NxU32 mCreatedParticleIdsNum;
		NxU32* mCreatedParticleIds;
		NxU32 mDeletedParticleIdsNum;
		NxU32* mDeletedParticleIds;
		float* mRenderBuffer;
		float* mRenderBufferUserData;	
		osg::ref_ptr<osg::Geode> m_geode;
		osg::ref_ptr<osg::Geometry> m_pointsGeom;
		osg::ref_ptr<osg::Vec3Array> m_vertices;		
		osg::ref_ptr<osg::Vec3Array> m_normals;		
		osg::ref_ptr<osg::Vec4Array> m_colors;	
		osg::ref_ptr<osg::StateSet> m_stateSet;    	
		osg::ref_ptr<osg::Point> m_point;	
		osg::ref_ptr<osg::PointSprite> m_sprite;
		osg::ref_ptr<osg::DrawArrays> m_draw_arrays;
};

NxVec3 RandNormalVec(const NxVec3& mu, NxReal sig);

// particle updater
static float vmax = 5.0f;


void update_fluid(CPhysXFluid* p_fluid, osg::Drawable* p_drawable);

struct FluidUpdateCallback : public osg::Drawable::UpdateCallback
{
	private:
		CPhysXFluid* m_fluid;
		NxScene* m_scene;
	public:
		FluidUpdateCallback( NxScene* scene, CPhysXFluid* fluid) : m_scene(scene), m_fluid(fluid)
		{  }
		virtual void update(osg::NodeVisitor*, osg::Drawable* p_drawable)
		{	
			update_fluid(m_fluid, p_drawable);
		}
	
};


#endif