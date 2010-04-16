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

Universe::Universe(const HatParams &params) :
	p(params.GetPlanets()), params(params), integrator(params.stepType, 6 * p.N(),
			params.accr, params.t0, params.t1, p, Gravity::Acceleration,
			Gravity::Jerk, p), t(params.t0), h(params.hmax) {
}

Universe::~Universe() {
	cout << integrator.GetSteps() << " ";
	for (int i = 0; i < 6 * p.N(); i++) {
		cout << integrator.GetError()[i] << " ";
	}
}

void Universe::BigBang() {
	h = params.GetStep(t, h, p);
	while (t < params.t1) {
		integrator.Evolve(t, h, p);
		h = params.GetStep(t, h, p);
	}
}

double Universe::GetTime() const {
	return t;
}

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

double Universe::K() const {
	double K = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++)
		K += p.K(funyuns);
	return K;
}

double Universe::U() const {
	double U = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++)
		U += p.U(funyuns);
	return U;
}

double Universe::E() const {
	return K() + U();
}
