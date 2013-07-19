#include "OSG_Physx_2_8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "NxCooking.h"
#include "NxPhysicsSDK.h"
#include "NxPMap.h"
#include "PhysXLoader.h"

#include "SampleParticleFluid.h"
#include "Utilities.h"
#include "UserAllocator.h"
#include "SamplesVRDSettings.h"


bool COSG_PhysX::InitNx(void)
{
	gDefaultGravity = NxVec3(0,-9.8,0);
	// Initialize PhysicsSDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, &gErrorStream, desc, &errorCode);
	if(gPhysicsSDK == NULL) 
	{
		//printf("\nSDK create error (%d - %s).\nUnable to initialize the PhysX SDK, exiting the sample.\n\n", errorCode, getNxSDKCreateError(errorCode));
		return false;
	}

	//static NxCookingInterface *gCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	if (!gCookingInitialized)
	{
		gCookingInitialized = true;
		if (!InitCooking(NULL, &gErrorStream)) 
		{
			printf("\nError: Unable to initialize the cooking library, exiting the sample.\n\n");
			return false;
		}
	}	

	// setup the scene
	
	sceneDesc.gravity = gDefaultGravity;
	gScene = gPhysicsSDK->createScene(sceneDesc);
	if(gScene == NULL) 
	{
		printf("\nError: Unable to create a PhysX scene, exiting the sample.\n\n");
		return false;
	}

	// Create the default material
	NxMaterialDesc m; 
	m.restitution = 0.5f;
	m.staticFriction = 0.2f;
	m.dynamicFriction= 0.2f;
	NxMaterial * mat = gScene->getMaterialFromIndex(0);
	mat->loadFromDesc(m); 

	gScene->setTiming(1.0f/60.0f, 5, NX_TIMESTEP_FIXED);


	
	return true;
}

bool COSG_PhysX::InitCooking(NxUserAllocator* allocator, NxUserOutputStream* outputStream)
{
#ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if ( !gCooking ) return false;
  return gCooking->NxInitCooking(allocator, outputStream);
#else
  return NxInitCooking(allocator, outputStream);
#endif
}


void COSG_PhysX::CloseCooking(void)
{
#ifdef COOKING_INTERFACE
	if ( !gCooking ) return;
	gCooking->NxCloseCooking();
#else
	return NxCloseCooking();
#endif
}


// releases physx
void COSG_PhysX::ReleaseNx(void)
{
	if (gCookingInitialized) CloseCooking();
	gCookingInitialized = false;
	if(gPhysicsSDK != NULL)
	{
		if(gScene != NULL) gPhysicsSDK->releaseScene(*gScene);
		gScene = NULL;
		NxReleasePhysicsSDK(gPhysicsSDK);
		gPhysicsSDK = NULL;
	}
}

// physx reset
void COSG_PhysX::ResetNx(void)
{
	ReleaseNx();
	if (!InitNx()) exit(0);
}