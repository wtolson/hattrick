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

Universe::Universe(const HatParams &params) : // In the beginning there was nothing...
	p(params.GetPlanets()), // Then God created the planets...
			params(params), // Referenced the params...
			integrator(params.stepType, 6 * p.N(), params.accr, params.t0,
					params.t1, p, Gravity::Acceleration, Gravity::Jerk, p), // Set up the ode solver...
			t(params.t0), h(params.GetStep(t, params.hmax, p)) {// And created time and steps...
	// And God saw that it was good.
}

Universe::~Universe() {
//
//  If we feel like some debugging:
//
//	cout << integrator.GetSteps() << " ";
//	for (int i = 0; i < 6 * p.N(); i++) {
//		cout << integrator.GetError()[i] << " ";
//	}
}

void Universe::BigBang() { // Bang!
	while (t < params.t1) {  // This is basically the whole program:
		integrator.Evolve(t, h, p);  // Do math.
		h = params.GetStep(t, h, p);  // Yay for adaptive step sizing with automatic event control!
	}
}

// The momentum of the universe.
double * Universe::P() const {
	double * P = new double[3];
	for (int cheetos = 0; cheetos < 3; cheetos++)
		P[cheetos] = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++) {
		double * tempP = p.P(funyuns);
		for (int cheetos = 0; cheetos < 3; cheetos++)
			P[cheetos] += tempP[cheetos];
		delete tempP;
	}
	return P;
}

// The angular momentum of the universe.
double * Universe::L() const {
	double * L = new double[3];
	for (int cheetos = 0; cheetos < 3; cheetos++)
		L[cheetos] = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++) {
		double * tempL = p.L(funyuns);
		for (int cheetos = 0; cheetos < 3; cheetos++)
			L[cheetos] += tempL[cheetos];
		delete tempL;
	}
	return L;
}

// The kinetic energy of the universe.
double Universe::K() const {
	double K = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++)
		K += p.K(funyuns);
	return K;
}

// The potential energy of the universe.
double Universe::U() const {
	double U = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++)
		U += p.U(funyuns);
	return U;
}

// The total energy of the universe.
double Universe::E() const {
	return K() + U();
}
