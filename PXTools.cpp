#include "PXTools.h"


/**
Just a little helper to get more interesting start conditions.
Probably not the most efficient method.
*/
NxVec3 RandNormalVec(const NxVec3& mu, NxReal sig)
{
	NxReal x, y, z;
	NxReal s;

	//choose direction, uniformly distributed.
	do
	{
		x = NxMath::rand(-1.0f, 1.0f);
		y = NxMath::rand(-1.0f, 1.0f);
		z = NxMath::rand(-1.0f, 1.0f);
		s = x*x + y*y + z*z;
	} while (s >= 1.0f);

	NxVec3 dir(x,y,z);
	dir.normalize();

	
	//compute normal distributed magnitude.
	do
	{
		x = NxMath::rand(-1.0f, 1.0f);
		y = NxMath::rand(-1.0f, 1.0f);
		s = x*x + y*y;
	} while (s >= 1.0f);

	NxReal temp = NxMath::logE(1-s)/(1-s);
	NxReal r = sig * NxMath::sqrt(-2.0f*temp);

	return mu + dir * r*x;
}

