#ifndef _OSG_Physx_2_8
#define _OSG_Physx_2_8

#include "NxPhysics.h"
#include "ErrorStream.h"

class COSG_PhysX
{
	public:	
		NxPhysicsSDK* gPhysicsSDK;
		NxScene* gScene;
		NxVec3 gDefaultGravity;
		ErrorStream gErrorStream;		
		NxSceneDesc sceneDesc;
		bool gCookingInitialized;
		bool InitNx(void);	
		void ReleaseNx(void);
		void ResetNx(void);
		bool InitCooking(NxUserAllocator* allocator, NxUserOutputStream* outputStream);
		void CloseCooking(void);
};

#endif