// ***********************************************************************
// Universe.cpp
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Grab bag class to hold the planets.

#include "HatParams.h"
#include "Gravity.h"
#include "Universe.h"

#include <iostream>
using namespace std;

Universe::Universe(HatParams * params) {
	this->params = params;
	p = new Planets(params->GetPlanets());
	t = params->t0;
	h = params->hmax;
	integrator = new Integrator(params->stepType, 6 * p->N(), params->accr,
			params->t0, params->t1, p->PlanetsPointer(), Gravity::Acceleration,
			Gravity::Jerk, p);

}

Universe::~Universe() {
	delete p;
	delete integrator;
}

void Universe::BigBang() {
	while (t < params->t1) {
		integrator->Evolve(t, h, p->PlanetsPointer());

		h = params->GetStep(t, h, p);
	}
}

double Universe::GetTime() {
	return t;
}

double * Universe::P() {
	double * P = new double[3];
	for (int cheetos = 0; cheetos < 3; cheetos++)
		P[cheetos] = 0.0;
	for (int funyuns = 0; funyuns < p->N(); funyuns++) {
		double * tempP = p->P(funyuns);
		for (int cheetos = 0; cheetos < 3; cheetos++)
			P[cheetos] += tempP[cheetos];
		delete tempP;
	}
	return P;
}

double * Universe::L() {
	double * L = new double[3];
	for (int cheetos = 0; cheetos < 3; cheetos++)
		L[cheetos] = 0.0;
	for (int funyuns = 0; funyuns < p->N(); funyuns++) {
		double * tempL = p->L(funyuns);
		for (int cheetos = 0; cheetos < 3; cheetos++)
			L[cheetos] += tempL[cheetos];
		delete tempL;
	}
	return L;
}

double Universe::K() {
	double K = 0.0;
	for (int funyuns = 0; funyuns < p->N(); funyuns++)
		K += p->K(funyuns);
	return K;
}

double Universe::U() {
	double U = 0.0;
	for (int funyuns = 0; funyuns < p->N(); funyuns++)
		U += p->U(funyuns);
	return U;
}

double Universe::E() {
	return K() + U();
}
