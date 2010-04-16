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
	enum StepType {
		RKF45, RK8PD, BSIMP
	};

	typedef int (* function)(double t, const double y[], double dydt[],
			void * params);

	typedef int (* jacobian)(double t, const double y[], double * dfdy,
			double dfdt[], void * params);

	Integrator(int stepType, int dim, double eps_abs, double t, double t1,
			double *y, function f, jacobian j, void *params);
	virtual ~Integrator();

	bool Evolve(double &t, double &h, double y[]);
	double *GetError() const;
	double *GetStepError() const;
	int GetSteps() const;

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
