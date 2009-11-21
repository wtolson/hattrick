/***********************************************************************
 * hattrick.cpp 
 * Version 0.8
 *
 * Copyright 2009 William Olson <wtolson@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
*/

// C++ Libraries
#include <iostream>
#include <cmath>

// GSL Libraries
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

// My Libraries
#include "gravity.h"
#include "hatparams.h"
#include "voodoomagic.h"

// Nothing to see here.

using namespace std;

int main (int argc, char** argv)
{
	hatparams hp = hatparams (argc, argv);
	if (!hp.SUCCESS) return 1;
	
	const gsl_odeiv_step_type * T;

	switch (hp.stepType) {
		case 0:
			T = gsl_odeiv_step_rk8pd;
			break;
			
		case 1:
			T = gsl_odeiv_step_bsimp;
			break;
			
		default:
			T = gsl_odeiv_step_rkf45;
	}

	gsl_odeiv_step * s
		= gsl_odeiv_step_alloc (T, 6*hp.N);
	gsl_odeiv_control * c
		= gsl_odeiv_control_y_new (hp.accr, 0.0);
	gsl_odeiv_evolve * e
		= gsl_odeiv_evolve_alloc (6*hp.N);


	gsl_odeiv_system sys = {func, jac, 6*hp.N, &hp};
	
	double t=hp.t0, h=hp.h0, x=0.0, tPrint=hp.printSkip;

	sacrificeChicken();
	
	// Initial print out.
	hp.print(t);

	while (t < hp.t1)
	{
		double x0 = x;//, h0 = h;
		int status = gsl_odeiv_evolve_apply (e, c, s, &sys, &t, hp.t1,
											 &h, hp.y);

		

		if (status != GSL_SUCCESS)
			break;
		
		//h = min(h, hp.h1);
		
		x = hp.xHat(1,0,1);
		if(hp.orbits() && x>=0.0 && x0<0.0) {
			hp.print(t);
		}
		
		if(!hp.orbits() && t >= tPrint) {
			tPrint += hp.printSkip;
			hp.print(t);
		}
	}

	gsl_odeiv_evolve_free (e);
	gsl_odeiv_control_free (c);
	gsl_odeiv_step_free (s);
	return 0;
}
