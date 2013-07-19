//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#ifndef _INCL_OSG_Tools
#define _INCL_OSG_Tools

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <osg/MatrixTransform>



float lerp_f(float x0, float x1, float t) ;
double lerp_lf(double x0, double x1, double t);
osg::Vec3d lerp_Vec3d(osg::Vec3d v0, osg::Vec3d v1, double t);
osg::Vec4d lerp_Vec4d(osg::Vec4d v0, osg::Vec4d v1, double t);
osg::Quat lerp_Quat(osg::Quat v0, osg::Quat v1, double t);


template <typename T> 
std::string to_string(const T& value) 
{ 
    std::ostringstream s; 
    s << value; 
    return s.str(); 
} 

template <class myType>
myType GetMax (myType a, myType b) 
{
	 return (a>b?a:b);
}


//Gravitational constant (N m^2 / kg^2) 
const double Ggrav = 6.6720e-11;
const double G= 6.67259e-11;

//pi: 
const double pi = 3.1415926535897932384626433832795;
const double PI= 3.1415926535897932384626433832795;

//Atomic mass unit (kg) 
/* unified atomic mass unit (u) (10) kg   */
	const double u_kg= 1.6605402E-27;
	const double amu = 1.6605655e-27;

//e: 
	const double e = 2.718281828459045235360287471;

	//speed of light in vacuo (m/s) 
/* speed of light in vacuum (def) m/s		*/
	//const double c= 299792458;//  m.(s^-1)
	//const double clight = 2.99792458e+8;
	const double c=         2.99792458e8;

//Planck's constant (J.s) 	
/* Planck constant (40)J s                 */
const double h= 6.6260755E-34;
const double hPlanck = 6.626176e-34;

/* Planck constant=, reduced (63) J s       */
const double h_BAR= 1.05457266E-34;

/* Planck constant, reduced (20) MeV s       */
const double h_BAR_MeVs= 6.5821220E-22; 

/* electron charge magnitude (49) C      */	
//Quantum of charge (C) 
const double Qcharge = 1.6021892e-19;
const double e_C= 1.60217733E-19;

/* electron charge magnitude (15) esu      */
const double e_ESU= 4.8032068E-10;

/* conversion constant hbar*c (59) MeV Fm            */
const double hBARc= 197.327053;

/* conversion constant (hbar*c)^2 (23) GeV^2 mbarn            */
const double hBARc2= 0.38937966;

/* electron mass (54) kg                  */
//mass of electron (kg) 
const double elecmass = 9.109534e-31;
const double m_e_kg= 9.1093897E-31;

/* electron mass (15) MeV/c^2                   */
const double m_e_MeV= 0.51099906;

/* proton mass (28) MeV/c^2                     */
const double m_P_MeV= 938.27231;

/* proton mass (12) u                    */
const double m_P_u= 1.007276470;

/* proton mass (10) kg                    */
const double m_P_kg= 1.6726231E-27;

/* proton mass (37) m_e                    */
const double m_P_M_E= 1836.152701;

/* deuteron mass (57) MeV/c^2                  */
const double m_D_MeV= 1875.61339;
	
/* unified atomic mass unit (u) (28) MeV/c^2   */
const double u_MeV= 931.49432;	

/* permittivity of free space F/m      */
//permittivity of free space (C^2/N/m^2) 
const double epsilon0 = 8.854187818e-12;
const double EPSILON_0= 8.854187817E-12;

//permeability of free space (N/A^2), (defined as pi * 4e-7) 
/* permeability of free space N/A^2      */
const double MU_0= 12.566370614E-7;
	const double mu0 = 1.256637061435917295385057353e-6;

/* fine-structure constant (61)        */
const double ALPHA= 1/137.0359895;

/* classical electron radius (38) m     */
const double r_e= 2.81794092E-15;

/* electron Compton wavelength (35) m   */
const double LAMBDA_BAR_e= 3.86159323E-13;

/* Bohr radius (mnucleus= infty) (24) m     */
const double a_0= 0.529177249E-10;

/* wavelength of 1 eV/c particle (37) m   */
const double LAMBDA_1EV= 1.23984244E-6;

/* Rydberg energy (mnucleus = infinity) (40) eV                 */
const double R_INFINITY_EV= 13.6056981;

/* Thomson cross section (18) barn          */
const double SIGMA_0_BARN= 0.66524616;

/* Bohr magneton (52)  MeV/T                 */
const double MU_B_MeV_T= 5.78838263E-11;

/* nuclear magneton (28) MeV/T               */
const double MU_N_MeV_T= 3.15245166E-14;

/* electron cyclotron freq./field (53) C/kg (rad/sT) */
const double E_M_e= 1.75881962E11;

/* proton cyclotron freq./field (29) C/kg (rad/sT) */
const double E_M_P= 9.5788309E7;

/* gravitational constant (85) m^3/kgs^2         */
const double G_SI= 6.67259E-11;

/* gravitational constant (86) h_bar c (GeV/c^2)^{-2}       */
const double G_P= 6.70711E-39;

/* standard grav. accel., sea level m/s^2 */
const double g= 9.80665;	

/* Boltzmann constant (12) J/K             */	
//Boltzmann's constant (J/K) 
const double kBoltzmann = 1.380662e-23;	
const double K_B= 1.380658E-23;

/* Boltzmann constant (73) eV/K            */
const double K_B_EV= 8.617385E-5;

/* molar volume, ideal gas at STP (19) m^3/mole  */
const double V_MOLAR= 22.41410E-3;

/* Wien displacement law constant (24) m K */
const double LAMBDAT= 2.897756E-3;

/* Stefan-Boltzmann constant (19) W/m^2K^4       */
//Stefan-Boltzmann constant (W/m^2/K^4) 
const double sigmaStefBoltz = 5.67032e-8;
const double SIGMA_SB= 5.67051E-8;

/* Fermi coupling constant (2)  GeV^{-2}        */
const double G_F= 1.16639E-5;

/* weak mixing angle  (5)  at M_Z           */
const double SIN2_THETA_W= 0.23192;

/* W boson mass (26) GeV/c^2                  */
const double M_W= 80.22;

/* Z_0 boson mass (7) GeV/c^2                  */
const double M_Z0= 91.187;

/* strong coupling constant (5))  at M_Z     */
const double G_S= 0.117;	

//Euler-Mascheroni constant: 
const double eulmasch = 0.577215664901532861;

//Avogadro's number: 
/* Avogadro constant (36)  /mole            */
const double N_A =6.0221367E23; 

const double avogadro = 6.022045e+23;

	//METERS
inline double meter_to_yard(double meter)
{
	return meter* 1.093613;
}

inline double meter_to_fathom(double meter)
{
	return meter* 0.546807;
}

inline double meter_to_vara(double meter)//??
{
	return meter* 1.1811;
}

inline double meter_to_rod(double meter)
{
	return meter* 0.198839;
}

inline double meter_to_feet(double meter)
{
	return meter* 3.28084;
}

inline double meter_to_cubit(double meter)
{
	return meter* 2.187227;
}

inline double meter_to_chain(double meter)
{
	return meter* 0.04971;
}

inline double meter_to_mile(double meter)
{
	return meter* 0.000621;
}

inline double meter_to_nautical_mile(double meter)
{
	return meter* 0.00054;
}

inline double meter_to_league(double meter)
{
	return meter* 0.000207;
}

inline double meter_to_nautical_league(double meter)
{
	return meter* 0.00018;
}

inline double meter_to_light_year(double meter)
{
	return meter/ (9.4650* pow(10.0, 15.0));
}

inline double meter_to_parsec(double meter)
{
	return meter/ (3.086* pow(10.0, 16.0));
}

inline double meter_to_AU(double meter)
{
	return meter/ 149597870691;//??
}



//PARSECS
inline double parsec_to_AU(double parsec)
{
	return parsec* 206264.555293;
}

inline double parsec_to_light_year(double parsec)
{
	return parsec* 3.261556;
}

inline double parsec_to_meter(double parsec)
{
	return parsec* 30856780000000000;
}

inline double parsec_to_kilometer(double parsec)
{
	return parsec* 30856780000000;
}

inline double parsec_to_mile(double parsec)
{
	return parsec* 19173514177205.12;
}


//AU
inline double AU_to_parsec(double AU)
{
	return AU* (1.0/206265);
}


inline double AU_to_kilometer(double AU)
{
	return AU* 149597870.691;
}


inline double AU_to_meter(double AU)
{
	return AU* 149597870691;//??
}

inline double light_year_to_parsec(double light_year)
{
	return light_year* 0.306602;
}


inline double light_year_to_meter(double light_year)
{
	return light_year* 9460753090819000;
}

//degrees
inline double degree_to_radian(double degree)
{
	return ((pi * degree) / 180);
}	

inline double degree_to_minute(double degree)
{
	return degree* 60;
}

inline double degree_to_second(double degree)
{
	return degree* 3600;
}

inline double degree_to_arcsec(double arcsec)
{
	return arcsec* 3600;
}

inline double degree_to_mil(double degree)
{
	return degree* 17.777778;
}

inline double degree_to_point(double degree)
{
	return degree* 0.088889;
}


//radians
inline double radian_to_degree(double radians)
{
	return ((180 *radians) /pi);
}

//arcsec
// if 1 degree = 3060 arcsec 
inline double arcsec_to_degree(double arcsec)
{
	return arcsec/3600;
}

//arcmin
// if 1 degree = 60 arcmin 
inline double arcmin_to_degree(double arcsec)
{
	return arcsec/60;
}

//milliarcsc
// if 1 degree = 1000 milliarcsec 
inline double milliarcsec_to_degree(double arcsec)
{
	return arcsec_to_degree(arcsec/1000.0);
}

//hour
//if 24 hours= 360 degree s
inline double hour_to_degree(double hour)
{
	return hour* 15;//??
}

//minutes
// if 60 min= 1 degree
inline double minute_to_degree(double minute)
{
	return minute* (1.0/60.0);//60 min = 1 deg
}

//seconds
// if 360 sec= 1 degree
inline double second_to_degree(double second)
{
	return second* (1.0/360.0);//60 min = 1 deg
}			
//RA minutes
//if there are 1440 min in 24 hours and 24 hours makes 360 degrees
inline double RA_minute_to_degree(double minute)
{
	//24* 60= 1440
	//1 min = 360/1440 deg
	return minute* 0.25;	
}	
//RA seconds
//if there are 86400 seconds in 24 hours and 24 hours makes 360 degrees
inline double RA_second_to_degree(double second)
{
	//24 * 3600= 86400 
	//1 sec = 360/86400 deg
	return second* 0.00416666;		
}

//mil
inline double mil_to_degree(double mil)
{
	return mil* 0.05625;
}
		
inline int round_double(double n)
{
	return static_cast<int>(floor(n+= 0.5));
}


inline int round_float(float n)
{
	return static_cast<int>(floor(n+= 0.5));
}

inline float random_float(float max)
{
	float r = (float)rand()/(float)RAND_MAX; 
	return r*max;
}





#endif