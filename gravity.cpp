/***********************************************************************
 * gravity.cpp
 *
 * Copyright 2009 William Olson <wtolson@gmail.com>
 * GNU General Public License v3
 * 
 * The System of equations describing the forces between the particles.
 * 
 */

#include <iostream>
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_errno.h>
#include "hatparams.h"
#include "gravity.h"

using namespace std;

int func (double t, const double y[], double f[], void *params)
{
	hatparams * hp = (hatparams *) params;
	int N = hp->N;
	
	for(int i=0; i<6*N; i++) f[i] = 0.0;
	
	for (int i=1; i<N; i++) {		
		
		for (int k=0; k<3; k++) f[hp->fa(i,k)] = 0.0;
		
		for (int j=0; j<i; j++) {
			double rcube = hp->r(i,j,y);
			rcube = (rcube*rcube*rcube);
			for(int k=0; k<3; k++) {
				// Set velocities.
				f[hp->fv(i,k)] = hp->v(i,k,y);
				f[hp->fv(j,k)] = hp->v(j,k,y);
				
				// Set accelerations.
				double ftemp = hp->xHat(i,j,k,y) / rcube;
				f[hp->fa(i,k)] -= hp->M[j]*ftemp;
				f[hp->fa(j,k)] += hp->M[i]*ftemp;
			}
		}
	}
	
	return GSL_SUCCESS;
}

int jac (double t, const double *y, double *dfdy, double *dfdt, void *params)
{
	hatparams * hp = (hatparams *) params;
	int N = hp->N;
	
	gsl_matrix_view dfdy_mat
		= gsl_matrix_view_array (dfdy, 6*N, 6*N);
	gsl_matrix * m = &dfdy_mat.matrix;
	gsl_matrix_set_zero (m);
	
	for (int i=0; i<N; i++) {
		for (int a=0; a<3; a++) {
			gsl_matrix_set(m, hp->fv(i,a), hp->yv(i,a), 1.0);

			for (int k=0; k<N; k++) {
				for (int b=0; b<3; b++) {
					double Gtemp = 0.0;
					for (int j=0; j<N; j++) {
						if (j != i) {							
							double r = hp->r(i,j);
							Gtemp -= hp->M[j] * (delta(i, k) - delta(j, k)) / (r*r*r) * \
								(delta(a, b) - 3.0 * (hp->xHat(i,j,a,y))*(hp->xHat(i,j,b,y))/(r*r));
						}
					}
					gsl_matrix_set(m, hp->fa(i,a), hp->yx(k,b), Gtemp);
				}
			}
		}
	}
	
	for(int i=0; i<6*N; i++) dfdt[i] = 0.0;
	
	for (int i=1; i<N; i++) {
		for (int j=0; j<i; j++) {
			for(int k=0; k<3; k++) {
				// Set accelerations.
				double r = hp->r(i,j,y);
				double ftemp = hp->xHat(i,j,k,y) / (r*r*r);
				dfdt[hp->fv(i,k)] -= hp->M[j]*ftemp;
				dfdt[hp->fv(j,k)] += hp->M[i]*ftemp;
				
				// Set Jerks.
				ftemp = ( hp->vHat(i,j,k,y) - 3 * ( hp->rDotv(i,j,y) / (r*r)) * hp->xHat(i,j,k,y) ) / (r*r*r);
				dfdt[hp->fa(i,k)] -= hp->M[j]*ftemp;
				dfdt[hp->fa(j,k)] += hp->M[i]*ftemp;
			}
		}
	}
	
	/* Debug Stuff
	for (int i=0; i<6*N; i++) {
		for (int j=0; j<6*N; j++) {
			cout << gsl_matrix_get (m, i, j) << "\t";
		}
		cout << endl;
	}
	cout << endl;
	*/
	
	
	return(GSL_SUCCESS);
}

double delta(int a, int b) {
	if (a==b) return 1.0;
	return 0.0;
}
