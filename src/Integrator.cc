// ***********************************************************************
// Integrator.cpp
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Class to encapsulate a gsl ODE integrator.

#include "Integrator.h"
#include <cmath>

#include <iostream>
using namespace std;

// stepType, dim, eps_abs, t, t1, y, funct, jac, params
Integrator::Integrator(int stepType, int dim, double eps_abs, double t,
		double t1, double *y, function f, jacobian j, void *params) :
	t(t), t1(t1), y(y), dim(dim) {

	switch (stepType) {
	case 1:
		step = gsl_odeiv_step_alloc(gsl_odeiv_step_rk8pd, dim);
		break;

	case 2:
		step = gsl_odeiv_step_alloc(gsl_odeiv_step_bsimp, dim);
		break;

	default:
		step = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, dim);
	}

	// Allocate memory for GSL ODE solver.
	con = gsl_odeiv_control_y_new(eps_abs, 0.0);
	evolve = gsl_odeiv_evolve_alloc(dim);

	sys.function = f;
	sys.jacobian = j;
	sys.dimension = dim;
	sys.params = params;

	totalError = new double[dim];
	for (int i = 0; i < dim; i++)
		totalError[i] = 0.0;
}

Integrator::~Integrator() {
	gsl_odeiv_evolve_free(evolve);
	gsl_odeiv_control_free(con);
	gsl_odeiv_step_free(step);

	delete[] totalError;
}

bool Integrator::Evolve(double &t, double &h, double y[]) {

	int status = gsl_odeiv_evolve_apply(evolve, con, step, &sys, &t, t1, &h, y);
	SumError(evolve->yerr);
	return status;
}

double * Integrator::GetStepError() const {
	return evolve->yerr;
}

double * Integrator::GetError() const {
	return totalError;
}

int Integrator::GetSteps() const {
	return evolve->count;
}

void Integrator::SumError(double yerr[]) {
	for (int i = 0; i < dim; i++)
		totalError[i] += yerr[i] * yerr[i];
}

