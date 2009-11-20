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
	
	for(int i=0; i<9*N; i++) f[i] = 0.0;
	
	for (int i=1; i<N; i++) {
		for (int j=0; j<i; j++) {
			for(int k=0; k<3; k++) {
				// Set velocities.
				f[hp->vi(i,k)] = hp->v(i,k,y);
				f[hp->vi(j,k)] = hp->v(j,k,y);
				
				// Set accelerations.
				double r = hp->r(i,j,y);
				double ftemp = hp->xHat(i,j,k,y) / (r*r*r);
				f[hp->ai(i,k)] -= hp->M[j]*ftemp;
				f[hp->ai(j,k)] += hp->M[i]*ftemp;
			}
		}
	}
	
	return GSL_SUCCESS;
}

int jac (double t, const double y[], double *dfdy, double dfdt[],
		 void *params)
{
	hatparams * hp = (hatparams *) params;
	int N = hp->N;
	
	gsl_matrix_view dfdy_mat
		= gsl_matrix_view_array (dfdy, 9*N, 9*N);
	gsl_matrix * m = &dfdy_mat.matrix;
	gsl_matrix_set_zero (m);

	double mdtemp[N][3][3];
	for (int i=0; i<N; i++) {
		for(int k=0; k<3; k++) {
				for(int l=0; l<3; l++) mdtemp[i][k][l] = 0.0;
		}
		
		for (int j=0; j<i; j++) {
			for(int k=0; k<3; k++) {
				for(int l=0; l<3; l++) {
					double r = hp->r(i,j,y);
					double mtemp = - hp->xHat(i,j,k,y) * hp->xHat(i,j,l,y) / (r*r*r*r*r);
					if (k==l)mtemp += 1/(r*r*r);
					gsl_matrix_set (m, hp->ai(i,k), hp->vi(j,l), hp->M[j]*mtemp);
					gsl_matrix_set (m, hp->ai(j,k), hp->vi(i,l), hp->M[i]*mtemp);
					mdtemp[i][k][l] -= hp->M[j]*mtemp;
					mdtemp[j][k][l] -= hp->M[i]*mtemp;
				}			
			}
		}
	}
	
	for (int i=0; i<N; i++) {
		for(int k=0; k<3; k++) {
			gsl_matrix_set (m, hp->vi(i,k), hp->ai(i,k), 1.0);
			for(int l=0; l<3; l++)
				gsl_matrix_set (m, hp->ai(i,k), hp->vi(i,l), mdtemp[i][k][l]);
		}
	}
	/*
	for (int i=0; i<9*N; i++) {
		for (int j=0; j<9*N; j++) {
			cout << gsl_matrix_get (m, i, j) << "\t";
		}
		cout << endl;
	}
	cout << endl;
	*/
	
	for(int i=0; i<9*N; i++) dfdt[i] = 0.0;
	
	for (int i=1; i<N; i++) {
		for (int j=0; j<i; j++) {
			for(int k=0; k<3; k++) {
				// Set accelerations.
				double r = hp->r(i,j,y);
				double ftemp = hp->xHat(i,j,k,y) / (r*r*r);
				dfdt[hp->vi(i,k)] -= hp->M[j]*ftemp;
				dfdt[hp->vi(j,k)] += hp->M[i]*ftemp;
				
				// Set Jerks.
				ftemp = ( hp->vHat(i,j,k,y) - 3 * ( hp->rDotv(i,j,y) / (r*r)) * hp->xHat(i,j,k,y) ) / (r*r*r);
				dfdt[hp->ai(i,k)] -= hp->M[j]*ftemp;
				dfdt[hp->ai(j,k)] += hp->M[i]*ftemp;
			}
		}
	}

	return GSL_SUCCESS;
}
