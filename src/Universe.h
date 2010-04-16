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

	void BigBang();  // Bang!

	double GetTime() const;

	// Some diagnostics tools:
	double * P() const;
	double * L() const;
	double K() const;
	double U() const;
	double E() const;

private:
	const HatParams &params;  // Only gets a reference to the parameters so don't mess with it.
	Integrator integrator;
	double t, h;
};

inline double Universe::GetTime() const {  // It's relative.
	return t;
}

#endif /* UNIVERSE_H_ */
