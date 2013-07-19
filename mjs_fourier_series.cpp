#include "mjs_fourier_series.h"
#include "OSG_Tools.h"


double FAn_square(double n, double x)
{
	return 0;
}

double FBn_square(double n, double x)
{
	if(n!= 0) { return (1- cos(n*PI))/(n*PI); }
	return 0;
}


double nPIx_p(double n, double x, double p)
{
	if(p!= 0) { return (n*PI*x)/p; }
	return 0;
}


double fourier_sum(long N, double x, double p)
{
	double accumulator=0;
	long n=0;
		
	for(n= 0; n< N ; n++)
	{
		double An= FAn_square(n,x);
		double Bn= FBn_square(n,x);
		double nPIxP = nPIx_p(n,x,p);
		accumulator+= ((An* cos(nPIxP))+ (Bn* sin(nPIxP)));	
	}
	
	return accumulator;
}


double square_wave_sum(double x, double m)
{	
	return (2.0* sin(m*x))/ (m*PI);
}

double fourier_square_wave_sum(long N, double x)
{
	double accumulator=0;
	long n= 1;		
	double m= 1.0;
	for(n= 1; n<= N ; n++)
	{	
		accumulator+= square_wave_sum(x,m);
		m += 2.0;
	}	
	return accumulator;
}


double square_wave_fourier_series(double x, long N)
{
	return fourier_square_wave_sum(N, x);
}

double square_wave_fourier_series_0_1(double x, long N)
{
	return (1.0/2.0) + fourier_square_wave_sum(N, x);
}