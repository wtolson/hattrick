// ***********************************************************************
// Universe.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Header file for Universe.cpp

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "Planets.h"
#include "Integrator.h"

class Universe {
public:
	Planets p;

	Universe(HatParams &params);
	~Universe();

	void BigBang();

	double GetTime();

	//Diagnostics
	double * P();
	double * L();
	double K();
	double U();
	double E();

private:
	HatParams &params;
	Integrator integrator;
	double t, h;

	//friend double HatParams::GetStep(Universe * u);
};

#endif /* UNIVERSE_H_ */
