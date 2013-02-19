/****************************************************************************
# FroboMind gpgga_to_utm
# Copyright (c) 2011-2013, editor Kjeld Jensen <kjeld@frobolab.org>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#	* Redistributions of source code must retain the above copyright
#  	notice, this list of conditions and the following disclaimer.
#	* Redistributions in binary form must reproduce the above copyright
#  	notice, this list of conditions and the following disclaimer in the
#  	documentation and/or other materials provided with the distribution.
#	* Neither the name FroboMind nor the
#  	names of its contributors may be used to endorse or promote products
#  	derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#include "gps_conversions_lib/gps_conversions_lib.h"

#include "math.h"

#define deg_rad      0.01745329251994 /* 2pi/360 */

/***************************************************************************/
void latlon2utm (double lat, double lon,
	int *znum, char *zlet, double *n, double *e)
{
	double flat = 1/298.257223563;	/* WGS84 flat */
	double a = 6378137;				/* WGS84 equatorial radius */
	double k0 = 0.9996;
	double latr = lat * deg_rad;
	double lonr = lon * deg_rad;
	double lonr_center;
	double es;						/* eccentricity^2 */
	double eps;
	double N, T, C, A, M;

	/* test if the UTM projection is defined for this latitude and longitude */
	if (lat <= 84 && lat >= -80)
	{
		/* determine the UTM zone number */
		*znum = (int) ((lon + 180)/6) + 1;

		if( lat >= 56.0 && lat < 64.0 && lon >= 3.0 && lon < 12.0 )
			*znum = 32;

	  	/* Take care of zone numbers for Svalbard */
		if( lat >= 72.0 && lat < 84.0 )
		{
		  if(lon >=  0.0 && lon <  9.0) *znum = 31;
		  else if( lon >=  9.0 && lon < 21.0 ) *znum = 33;
		  else if( lon >= 21.0 && lon < 33.0 ) *znum = 35;
		  else if( lon >= 33.0 && lon < 42.0 ) *znum = 37;
		}

		/* determine the UTM zone letter */
		if     (( 84.0 >= lat) && (lat >=  72.0)) *zlet = 'X';
		else if(( 72.0 >  lat) && (lat >=  64.0)) *zlet = 'W';
		else if(( 64.0 >  lat) && (lat >=  56.0)) *zlet = 'V';
		else if(( 56.0 >  lat) && (lat >=  48.0)) *zlet = 'U';
		else if(( 48.0 >  lat) && (lat >=  40.0)) *zlet = 'T';
		else if(( 40.0 >  lat) && (lat >=  32.0)) *zlet = 'S';
		else if(( 32.0 >  lat) && (lat >=  24.0)) *zlet = 'R';
		else if(( 24.0 >  lat) && (lat >=  16.0)) *zlet = 'Q';
		else if(( 16.0 >  lat) && (lat >=   8.0)) *zlet = 'P';
		else if((  8.0 >  lat) && (lat >=   0.0)) *zlet = 'N';
		else if((  0.0 >  lat) && (lat >=  -8.0)) *zlet = 'M';
		else if(( -8.0 >  lat) && (lat >= -16.0)) *zlet = 'L';
		else if((-16.0 >  lat) && (lat >= -24.0)) *zlet = 'K';
		else if((-24.0 >  lat) && (lat >= -32.0)) *zlet = 'J';
		else if((-32.0 >  lat) && (lat >= -40.0)) *zlet = 'H';
		else if((-40.0 >  lat) && (lat >= -48.0)) *zlet = 'G';
		else if((-48.0 >  lat) && (lat >= -56.0)) *zlet = 'F';
		else if((-56.0 >  lat) && (lat >= -64.0)) *zlet = 'E';
		else if((-64.0 >  lat) && (lat >= -72.0)) *zlet = 'D';
		else if((-72.0 >  lat) && (lat >= -80.0)) *zlet = 'C';

		/* calculate UTM northing and easting */
		es = 2*flat-flat*flat;
		eps = (es)/(1-es);

		/* find the center longitude for the UTM zone */
		lonr_center = ((*znum -1)*6-180+3) * deg_rad;

		N = a/sqrt(1-es*sin(latr)*sin(latr));
		T = tan(latr)*tan(latr);
		C = eps*cos(latr)*cos(latr);
		A = cos(latr)*(lonr-lonr_center);

		M = a*((1 - es/4 - 3*es*es/64 - 5*es*es*es/ 256)*latr
			- (3* es/8 + 3*es*es/32 + 45*es*es*es/1024)*sin(2*latr)
			+ (15*es*es/256 + 45*es*es*es/1024)*sin(4*latr)
			-(35*es*es*es/3072)*sin(6*latr));

		*e = (k0*N*(A+(1-T+C)*A*A*A/6
			+ (5-18*T+T*T+72*C-58*eps)*A*A*A*A*A/120)
			+ 500000.0);

		*n = (k0*(M+N*tan(latr)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
			+ (61-58*T+T*T+600*C-330*eps)*A*A*A*A*A*A/720)));

		if(lat < 0)
			*n += 10000000.0; /* 10000000 meter offset for southern hemisphere */
	}
	else
      *znum = -1;
}
