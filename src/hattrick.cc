// ***********************************************************************
// hattrick.cpp
// Version 1.0
//
// Copyright 2010 William Olson <wtolson@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.


// C++ Libraries
#include <iostream>
#include <cmath>

// GSL Libraries
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

// My Libraries
#include "gravity.h"
#include "Hatparams.h"
#include "voodoomagic.h"

// Nothing to see here.

using namespace std;

int main (int argc, char** argv)
{
    // Read in parameters.
    Hatparams hp = Hatparams (argc, argv);
    if (!hp.SUCCESS) return 1;

    // Set integration step type.
    const gsl_odeiv_step_type * T;
    switch (hp.stepType) {
        case 1:
            T = gsl_odeiv_step_rk8pd;
            break;

        case 2:
            T = gsl_odeiv_step_bsimp;
            break;

        default:
            T = gsl_odeiv_step_rkf45;
    }

    // Allocate memory for GSL ODE solver.
    gsl_odeiv_step * s
        = gsl_odeiv_step_alloc (T, 6*hp.N);
    gsl_odeiv_control * c
        = gsl_odeiv_control_y_new (hp.accr, 0.0);
    gsl_odeiv_evolve * e
        = gsl_odeiv_evolve_alloc (6*hp.N);

    gsl_odeiv_system sys = {func, jacnot, 6*hp.N, &hp};

    // Set some initials.
    double h=hp.hmax, tPrint=hp.t;
    int steps = 0;
    sacrificeChicken();

    // Initial print out.
    //hp.print();

    while (hp.t < hp.t1)
    {
        // Take a step.  Get new h.
        int status = gsl_odeiv_evolve_apply (e, c, s, &sys, &hp.t, hp.t1,
                                             &h, hp.y);

        if (status != GSL_SUCCESS)
            break;

        if (hp.dmbf())
            break;

        // Print or check for orbit.
        if(!hp.orbit() && hp.t >= tPrint) {
            tPrint += hp.printSkip;
            hp.print();
        }


        steps++;

        // Step size control.  Smaller steps near orbits.
        if (hp.xHat(1,0,1)<0.0 && hp.vHat(1,0,1)>0.0) h = min(h,-hp.xHat(1,0,1)/(2*hp.vHat(1,0,1)));
        h = min(h, hp.hmax);
        h = max(h, hp.hmin);
    }

    //cout << "Steps:" << steps << endl;

    // Free GSL memory.
    gsl_odeiv_evolve_free (e);
    gsl_odeiv_control_free (c);
    gsl_odeiv_step_free (s);
    return 0;
}
