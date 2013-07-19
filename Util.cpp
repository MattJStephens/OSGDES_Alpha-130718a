#include "Util.h"

// constrained random number gen.

double randMinMax(double rmin, double rmax)
{
	double r = (((double) rand() / 
		(double) RAND_MAX) * rmax + rmin);
	return r;
}