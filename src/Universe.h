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

	Universe(const HatParams &params);
	~Universe();

	void BigBang();

	double GetTime() const;

	//Diagnostics
	double * P() const;
	double * L() const;
	double K() const;
	double U() const;
	double E() const;

private:
	const HatParams &params;
	Integrator integrator;
	double t, h;

	//friend double HatParams::GetStep(Universe * u);
};

#endif /* UNIVERSE_H_ */
