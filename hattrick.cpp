//      hattrick.cpp
//
//      Copyright 2009 William Olson <wtolson@gmail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_const_mksa.h>
#include <math.h>
#include "gravity.h"
#include "hatparams.h"
#include "voodoomagic.h"

//#define N 2


int
main (void)
{
	
	double k = 0.01720209895;
	double G = k*k;
	const int N = 2;
	double M[N];
	M[0] = G;
	M[1] = G/328900.56;
	
	hatparams hp = hatparams(&N, M);
	
	const gsl_odeiv_step_type * T
		//= gsl_odeiv_step_rkf45;
		= gsl_odeiv_step_bsimp;

	gsl_odeiv_step * s
		= gsl_odeiv_step_alloc (T, 8*N);
	gsl_odeiv_control * c
		= gsl_odeiv_control_y_new (1e-2, 0.0);
	gsl_odeiv_evolve * e
		= gsl_odeiv_evolve_alloc (8*N);


	gsl_odeiv_system sys = {func, jac, 8*N, &hp};
	//gsl_odeiv_system sys = {func, NULL, 8*N, M};

	double t = 0.0, t1 = 365.25;
	double h0 = 1E0;
	double h = h0;
	double sp = 0.0;
	double y[8*N] = { 0.0, 0.0, sp, 0.0, 0.0, sp, 0.0, 0.0, 1, 0.0, sp, 0.0, k, sp, 0.0, 0.0 };	
	
	sacrificeChicken();

	while (t < t1)
	{
		int status = gsl_odeiv_evolve_apply (e, c, s, &sys, &t, t1, &h, y);

		if (status != GSL_SUCCESS)
			break;

		printf("%f", t);
		//h = h0;
		int i;
		for(i = 0; i < 8*N; i++) {
			if (i!=2 && i!=5)
			printf (" %f", y[i]);
		}
		printf("\n");
	}

	gsl_odeiv_evolve_free (e);
	gsl_odeiv_control_free (c);
	gsl_odeiv_step_free (s);
	return 0;
}
