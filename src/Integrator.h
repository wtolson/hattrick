// ***********************************************************************
// Integrator.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Header file for Integrator.cpp

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <gsl/gsl_odeiv.h>

class Integrator {
public:
	const static int RKF45;
	const static int RK8PD;
	const static int BSIMP;

	Integrator(int stepType, int dim, double eps_abs, double t,
			double t1, double * y, int(* function)(double t, const double y[],
					double dydt[], void * params), int(* jacobian)(double t,
					const double y[], double * dfdy, double dfdt[], void * params),
			void * params);
	virtual ~Integrator();

	bool Evolve(double &t, double &h, double y[]);
	double * GetError();
	double * GetStepError();
	int GetSteps();

private:
	gsl_odeiv_step * step;
	gsl_odeiv_control * con;
	gsl_odeiv_evolve * evolve;
	gsl_odeiv_system sys;

	double * totalError;
	double t, t1;
	double *y;
	int dim;

	void SumError(double yerr[]);
};

#endif /* INTEGRATOR_H_ */
