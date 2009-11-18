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

// C++ Libraries
#include <iostream>
//#include <cmath>

// GSL Libraries
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

// My Libraries
#include "gravity.h"
#include "hatparams.h"
#include "voodoomagic.h"

using namespace std;

int
main (int argc, char** argv)
{
	double t, t1, h0, h1, accr, y[0];
	
	hatparams hp = hatparams (argc, argv, &t, &t1, &h0, &h1, &accr, y);
	if (!hp.success()) return 1;
	
	for(int i = 0; i < 9*hp.N; i++) {
			cout << " " << y[i];
	}		
	cout << endl;
	
	const gsl_odeiv_step_type * T
		= gsl_odeiv_step_rkf45;
		//= gsl_odeiv_step_bsimp;

	gsl_odeiv_step * s
		= gsl_odeiv_step_alloc (T, 9*hp.N);
	gsl_odeiv_control * c
		= gsl_odeiv_control_y_new (accr, 0.0);
	gsl_odeiv_evolve * e
		= gsl_odeiv_evolve_alloc (9*hp.N);


	//gsl_odeiv_system sys = {func, jac, 9*hp.N, &hp};
	gsl_odeiv_system sys = {func, NULL, 9*hp.N, &hp};
	
	double h = h0;
	
	sacrificeChicken();

	while (t < t1)
	{
		int status = gsl_odeiv_evolve_apply (e, c, s, &sys, &t, t1, &h, y);

		if (status != GSL_SUCCESS)
			break;

		//h = h0;
		cout << t;
		for(int i = 0; i < 9*hp.N; i++) {
			if (i%3!=2) cout << " " << y[i];
		}
		cout << endl;
	}

	gsl_odeiv_evolve_free (e);
	gsl_odeiv_control_free (c);
	gsl_odeiv_step_free (s);
	return 0;
}
