/* Part of PhysX */
#ifndef __CONTROLLERHITREPORT_H__
#define __CONTROLLERHITREPORT_H__

#include "NxPhysics.h"
#include "NxCapsuleController.h"

enum GameGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
};

#define MAX_NB_PTS	100
static NxU32 gNbPts = 0;
static NxVec3 gPts[MAX_NB_PTS];

#define SKINWIDTH	0.2f


class ControllerHitReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit)
	{		

		if(1 && hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			if(group!=GROUP_COLLIDABLE_NON_PUSHABLE)
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					if ((gPts[gNbPts].x != hit.worldPos.x) || (gPts[gNbPts].y != hit.worldPos.y) || (gPts[gNbPts].z != hit.worldPos.z))
					{
						gPts[gNbPts++].x = hit.worldPos.x;
						gPts[gNbPts++].y = hit.worldPos.y;
						gPts[gNbPts++].z = hit.worldPos.z;
						if (gNbPts==MAX_NB_PTS)  gNbPts = 0;
					}

					if(hit.dir.z==0.0f)
					{
						NxF32 coeff = actor.getMass() * hit.length * 10.0f;
						actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
						//						actor.addForceAtPos(hit.dir*coeff, hit.controller->getPosition(), NX_IMPULSE);
						//						actor.addForceAtPos(hit.dir*coeff, hit.worldPos, NX_IMPULSE);
					}
				}
			}
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
	{
		return NX_ACTION_NONE;
	}

};

#endif