#include "OSG_PX_Cloth.h"

#include "PxScene.h"
#include "PxCloth.h"
#include "PxClothMeshDesc.h"
//#include "PXClothTypes.h"

using namespace physx;

// -----------------------------------------------------------------------
void Clothinit(PxScene *scene, PxClothDesc &desc, PxClothMeshDesc &meshDesc)
{
	//mScene = scene;

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
