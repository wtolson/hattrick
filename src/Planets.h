// ***********************************************************************
// Planet.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Header file for Planet.cpp

#ifndef PLANET_H_
#define PLANET_H_

#include "KeplerianElements.h"

class Planets {
public:
	Planets();
	Planets(unsigned int N);
	Planets(const Planets& p);
	virtual ~Planets();

	bool AddPlanet(double mass, double x[3], double v[3], bool isDynamic = false);
	bool AddPlanet(double mass, double a, double e = 0.0, double inc = 0.0,
			double node = 0.0, double w = 0.0, double M = 0.0, bool isDynamic = true);
	bool AddPlanet(double mass, KeplerianElements ke, bool isDynamic = true);

	double * PlanetsPointer() {
		return y;
	}
	;
	int N() {
		return numPlanets;
	}
	;
	double M(int i) {
		return mass[i];
	}
	;

	//Get values for current Planets.
	double x(int i, int k) {
		return y[6 * i + k];
	}
	;
	double v(int i, int k) {
		return y[6 * i + k + 3];
	}
	;

	KeplerianElements *GetKeplerian(int i);
	double CM(int k);
	double VCM(int k);
	double TotalMass();

	//Diagnostics
	double * P(int i);
	double * L(int i);
	double K(int i);
	double U(int i);
	double E(int i);

private:
	int numPlanets;
	int capacity;
	double *mass, *y;
	bool *isDynamic;
	KeplerianElements **dynamicConstants;


	void grow();
};

#endif /* PLANET_H_ */
