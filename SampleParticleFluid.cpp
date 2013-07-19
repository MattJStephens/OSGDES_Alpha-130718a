
#include "SampleParticleFluid.h"
#include "Utilities.h"
#include "UserAllocator.h"
#include "SamplesVRDSettings.h"




ErrorStream			gErrorStream;

// Physics SDK globals
NxPhysicsSDK*		gPhysicsSDK = NULL;
NxScene*			gScene = NULL;

NxVec3				gDefaultGravity(0,-9.8,0);
bool				gSceneRunning = false;

UserAllocator*		gAllocator = NULL;

#ifdef NX_DISABLE_HARDWARE
bool				gHardwareSimulation = false;
#else
bool				gHardwareSimulation = true;
#endif

TerrainData*		gTerrainData = 0;
NxActor*			gTerrain = 0;
bool				gCookingInitialized = false;

// Samples
unsigned			gSampleIndex = 0;
NxActor*			gSelectedActor = NULL;

NxActor*			gGroundPlane = NULL;
NxArray<SampleParticleFluid*> gSamples;
NxArray<MyFluid*>	gMyFluids; 


// Time globals
NxReal				gTime;
NxReal				gLastTime;

// Display globals
int					gMainHandle;
bool				gHelp = false;
char				gDisplayString[512] = "";
char				gTitleString[512] = "";
char				gHelpString[512] = "";

// Camera globals
NxReal gCameraFov = 40.0f;
NxVec3 gCameraPos(0,5,15);
NxVec3 gCameraForward(0,0,-1);
NxVec3 gCameraRight(1,0,0);
const NxReal gCameraSpeed = 0.2;
int gViewWidth = 0;
int gViewHeight = 0;

// MouseGlobals
int mx = 0;
int my = 0;

bool gRotateCamera = false;
NxDistanceJoint* gMouseJoint = NULL;
NxActor* gMouseSphere = NULL;
NxReal gMouseDepth = 0.0f;

// Keyboard globals
#define MAX_KEYS 256
bool gKeys[MAX_KEYS];

// Simulation globals
bool gPause = false;
bool gShadows = true;
bool gWireframeMode = false;

// fps
int			gFrameCounter = 0;
float       gPreviousTime = getCurrentTime();

void WaitForPhysics()
{
	if (gSceneRunning)
	{
		NxU32 error;
		gScene->flushStream();
		gScene->fetchResults(NX_RIGID_BODY_FINISHED, true, &error);
		assert(error == 0);
		gSceneRunning = false;
	}
}


// ------------------------------------------------------------------------------------
NxReal UpdateTime()
{
    NxReal deltaTime;
    gTime = getTime()*0.001f;  // Get current time in seconds
    deltaTime = gTime - gLastTime;
    gLastTime = gTime;
    return deltaTime;
}



void RunPhysics()
{
	if (gSceneRunning)
		return;

	// Update the time step
	NxReal deltaTime = UpdateTime();

	deltaTime = 0.02f;	// todo: may go back to real time at some point

	// Run collision and dynamics for delta time since the last frame
#if 1
	NxSceneDesc desc;
	gScene->saveToDesc(desc);
	gScene->simulate(desc.maxTimestep);
#else
	gScene->simulate(deltaTime);
#endif

	gSceneRunning = true;
	WaitForPhysics();

	gSamples[gSampleIndex]->update();
	gFrameCounter++;

}



// ------------------------------------------------------------------------------------

void SetTitleString(const char *demoName)
{
#ifdef __PPCGEKKO__
	sprintf(gTitleString, "%s (%s)\nHOME for help\n", demoName, gHardwareSimulation ? "hardware" : "software");
#else
	sprintf(gTitleString, "%s CPhysXFluid Sample (%s)\nF1 for help\n", demoName, gHardwareSimulation ? "hardware" : "software");
#endif	
}

// ------------------------------------------------------------------------------------

void SetHelpString(const char *demoKeyString)
{
	char tempString[512];
	sprintf(gHelpString, "\nGeneral:\n");
#ifdef __PPCGEKKO__
	sprintf(gHelpString, "    +,-: choose scene\n");
	strcat(gHelpString, "    arrows: move/strafe\n");
	strcat(gHelpString, "    b: shoot sphere\n");
	strcat(gHelpString, "    HOME: toggle help\n");
	strcat(gHelpString, "    1: Reset scene\n");
	if(gSampleIndex == 6) strcat(gHelpString, "    2: toggle debug rendering\n");
#else	
	sprintf(tempString, "    1-%d: choose scene\n", gSamples.size());
	strcat(gHelpString, tempString);
	strcat(gHelpString, "    p: pause\n");
	strcat(gHelpString, "    o: single step\n");
#ifndef NX_DISABLE_HARDWARE
	strcat(gHelpString, "    h: hardware on/off\n");
#endif
	strcat(gHelpString, "    w,a,s,d: move/strafe\n");
	strcat(gHelpString, "    q,e: move up/down\n");
	strcat(gHelpString, "    mouse right: pick\n");
	strcat(gHelpString, "    space: shoot sphere\n");
	strcat(gHelpString, "    x: toggle shadows\n");
	strcat(gHelpString, "    n: toggle wireframe\n");
	strcat(gHelpString, "    F1: toggle help\n");
	strcat(gHelpString, "    F3,F4: pick push/pull\n");
	strcat(gHelpString, "    F5: toggle debug rendering\n");
	strcat(gHelpString, "    F9: toggle vsync\n");
	strcat(gHelpString, "    F10: Reset scene\n");
#endif

	if (demoKeyString)
	{
		strcat(gHelpString, "\nDemo specific:\n");
		strcat(gHelpString, demoKeyString);
	}
}