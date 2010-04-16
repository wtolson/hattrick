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
	bool AddPlanet(double mass, const KeplerianElements &ke);

	operator double *() const;
	int N() const;
	double M(int i) const;

	//Get values for current Planets.
	double x(int i, int k) const;
	double v(int i, int k) const;

	KeplerianElements *GetKeplerian(int i) const;
	double TotalMass() const;
	double CM(int k) const;
	double VCM(int k) const;
	bool IsCM() const;
	void MoveToCM();

	//Diagnostics
	double * P(int i) const;
	double * L(int i) const;
	double K(int i) const;
	double U(int i) const;
	double E(int i) const;

private:
	int numPlanets;
	int capacity;
	double *mass, *y;

	void grow();
};

inline Planets::operator double *() const {
	return y;
}

inline int Planets::N() const {
	return numPlanets;
}

inline double Planets::M(int i) const {
	return mass[i];
}

inline double Planets::x(int i, int k) const {
	return y[6 * i + k];
}

inline double Planets::v(int i, int k) const {
	return y[6 * i + k + 3];
}

#endif /* PLANET_H_ */
