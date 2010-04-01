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
	~Planets();

	Planets& operator=(const Planets& p);
	bool AddPlanet(double mass, double x[3], double v[3]);
	bool AddPlanet(double mass, double a, double e = 0.0, double inc = 0.0,
			double node = 0.0, double w = 0.0, double M = 0.0);
	bool AddPlanet(double mass, KeplerianElements ke);

	double * PlanetsPointer();
	int N();
	double M(int i);

	//Get values for current Planets.
	double x(int i, int k);
	double v(int i, int k);

	KeplerianElements *GetKeplerian(int i);
	double TotalMass();
	double CM(int k);
	double VCM(int k);
	bool IsCM();
	void MoveToCM();

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

	void grow();
};

inline double* Planets::PlanetsPointer() {
	return y;
}

inline int Planets::N() {
	return numPlanets;
}

inline double Planets::M(int i) {
	return mass[i];
}

inline double Planets::x(int i, int k) {
	return y[6 * i + k];
}

inline double Planets::v(int i, int k) {
	return y[6 * i + k + 3];
}

#endif /* PLANET_H_ */
