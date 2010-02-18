// ***********************************************************************
// gravity.cpp
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// The System of equations describing the forces between the particles.


//#include <iostream>
#include <cmath>
#include <gsl/gsl_errno.h>
#include "Planets.h"
#include "Gravity.h"

#define GRAVITY
#include "HatUtil.h"

const double Gravity::k = 0.01720209895;
const double Gravity::G = 0.0002959122082855911025;

int Gravity::Acceleration(double t, const double y[], double dydt[], void *params) {
	//cout << "In func" << endl;
	Planets * p = (Planets *) params;

	for (int i = 0; i < 6 * N; i++)
		dydt[i] = 0.0;

	for (int i = 1; i < N; i++) {

		for (int k = 0; k < 3; k++)
			dVdT(i,k) = 0.0;

		for (int j = 0; j < i; j++) {
			double r[3] = R(i,j);
			double rcube = sqrt(DOT(r,r));
			rcube = (rcube * rcube * rcube);
			for (int k = 0; k < 3; k++) {
				// Set velocities.
				dXdT(i,k) = V(i,k);
				dXdT(j,k) = V(j,k);

				// Set accelerations.
				double ftemp = r[k] / rcube;
				dVdT(i,k) -= M(j) * ftemp;
				dVdT(j,k) += M(i) * ftemp;
			}
		}
	}

	return GSL_SUCCESS;
}

int Gravity::Jerk(double t, const double *y, double *dfdy, double *dfdt,
		void *params) {
	//cout << "In jac" << endl;
	Planets * p = (Planets *) params;

	for (int i = 0; i < (6 * N * 6 * N); i++)
		dfdy[i] = 0.0;

	for (int i = 0; i < N; i++) {
		for (int a = 0; a < 3; a++) {
			dVdV(i,a,i,a) = 1.0;

			for (int k = 0; k < N; k++) {
				for (int b = 0; b < 3; b++) {
					double Jtemp = 0.0;
					for (int j = 0; j < N; j++) {
						if (j != i) {
							double r[3] = R(i,j);
							double rcube = sqrt(DOT(r,r));
							rcube = (rcube * rcube * rcube);

							Jtemp -= M(j) * (Delta(i, k) - Delta(j, k)) * rcube
									* (Delta(a, b) - 3.0 * r[a] * r[b]
											/ DOT(r,r));
						}
					}
					dAdX(i,a,k,b) = Jtemp;
				}
			}
		}
	}

	for (int i = 0; i < 6 * N; i++)
		dfdt[i] = 0.0;

	for (int i = 1; i < N; i++) {
		for (int j = 0; j < i; j++) {
			double r[3] = R(i,j);
			double rcube = sqrt(DOT(r,r));
			rcube = (rcube * rcube * rcube);
			double vrel[3] = VREL(i,j);
			for (int k = 0; k < 3; k++) {
				// Set accelerations.
				double ftemp = r[k] / rcube;
				A(i,k) -= M(j) * ftemp;
				A(j,k) += M(i) * ftemp;

				// Set Jerks.
				ftemp = (vrel[k] - 3 * (DOT(r,vrel) / DOT(r,r)) * r[k]) / rcube;
				J(i,k) -= M(j) * ftemp;
				J(j,k) += M(i) * ftemp;
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

	return (GSL_SUCCESS);
}

int Gravity::NullJac(double t, const double *y, double *dfdy, double *dfdt,
		void *params) {
	Planets * p = (Planets *) params;

	for (int i = 0; i < (6 * N * 6 * N); i++)
		dfdy[i] = 0.0;

	for (int i = 0; i < 6 * N; i++)
		dfdt[i] = 0.0;

	return (GSL_SUCCESS);
}

inline double Gravity::Delta(int a, int b) {
	return (a == b);
}
