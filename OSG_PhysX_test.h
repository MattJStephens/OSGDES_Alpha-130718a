#ifndef _OSG_PhysX_Test
#define _OSG_PhysX_Test

#include "PhysXCube.h"
#include "PhysXPlane.h"

class COSGPhysXTest
{	
	public:
		CPhysXCube* p_cube;
		CPhysXPlane * p_plane;
		void init(NxScene* scene);
		void render(void);
		void destroy(void);
};

#endif