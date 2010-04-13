// ***********************************************************************
// Planet.cpp
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Planet data type.

#include <cmath>
#include "Gravity.h"
#include "Planets.h"

#include <iostream>
using namespace std;

#include "HatUtil.h"

#define SMALLNUM 1E-10

Planets::Planets() {
	capacity = 0;
	numPlanets = 0;
	mass = 0;
	y = 0;
}

Planets::Planets(unsigned int N) {
	capacity = N;
	numPlanets = 0;
	mass = new double[capacity];
	y = new double[6 * capacity];
}

Planets::Planets(const Planets& p) {
	this->capacity = p.capacity;
	this->numPlanets = p.numPlanets;
	this->mass = new double[capacity];
	this->y = new double[6 * capacity];

	for (int i = 0; i < numPlanets; i++)
		this->mass[i] = p.mass[i];

	for (int i = 0; i < 6 * numPlanets; i++)
		this->y[i] = p.y[i];
}

Planets::~Planets() {
	delete[] mass;
	mass = 0;
	delete[] y;
	y = 0;
}

Planets& Planets::operator=(const Planets& p) {
	delete[] mass;
	mass = 0;
	delete[] y;
	y = 0;

	this->capacity = p.capacity;
	this->numPlanets = p.numPlanets;
	this->mass = new double[capacity];
	this->y = new double[6 * capacity];

	for (int i = 0; i < numPlanets; i++)
		this->mass[i] = p.mass[i];

	for (int i = 0; i < 6 * numPlanets; i++)
		this->y[i] = p.y[i];

	return *this;
}

bool Planets::AddPlanet(double mass, double x[3], double v[3]) {
	if (numPlanets == capacity)
		return false;
	this->mass[numPlanets] = Gravity::G * mass;
	for (int i = 0; i < 3; i++)
		X(numPlanets, i) = x[i];
	for (int i = 0; i < 3; i++)
		V(numPlanets, i) = v[i];
	numPlanets++;
	return true;
}

bool Planets::AddPlanet(double mass, double a, double e, double inc,
		double node, double w, double M) {
	if (numPlanets == capacity)
		return false;
	this->mass[numPlanets] = Gravity::G * mass;

	if (numPlanets == 0) {
		for (int i = 0; i < 3; i++) {
			X(numPlanets, i) = 0.0;
			V(numPlanets, i) = 0.0;
		}
		numPlanets++;
		return true;
	}

	double E = M, E0;
	do {
		E0 = E;
		E = E0 - (E0 - e * sin(E0) - M) / (1 - e * cos(E0));
	} while (abs(E - E0) > SMALLNUM);

	double P[3] = { cos(w) * cos(node) - sin(w) * cos(inc) * sin(node), cos(w)
			* sin(node) + sin(w) * cos(inc) * cos(node), sin(w) * sin(inc) };
	double Q[3] = { -sin(w) * cos(node) - cos(w) * cos(inc) * sin(node),
			-sin(w) * sin(node) + cos(w) * cos(inc) * cos(node), sin(inc)
					* cos(w) };
	double A = a * (cos(E) - e);
	double B = a * sqrt(1 - e * e) * sin(E);

	for (int i = 0; i < 3; i++)
		X(numPlanets, i) = X(0,i) + A * P[i] + B * Q[i];

	double Edot = sqrt(this->mass[0] / (a * a * a)) / (1 - e * cos(E));
	A = -a * sin(E) * Edot;
	B = a * sqrt(1 - e * e) * cos(E) * Edot;

	for (int i = 0; i < 3; i++)
		V(numPlanets, i) = V(0,i) + A * P[i] + B * Q[i];
	numPlanets++;
	return true;
}

bool Planets::AddPlanet(double mass, KeplerianElements ke) {
	return AddPlanet(mass, ke.a, ke.e, ke.inc, ke.node, ke.w, ke.M);
}

KeplerianElements *Planets::GetKeplerian(int i) const {
	double xHat[3] = { 1.0, 0.0, 0.0 };
	//double yHat[3] = { 0.0, 1.0, 0.0 };
	double zHat[3] = { 0.0, 0.0, 1.0 };

	//TODO: ROTATE COORDS.
	//double refDir[3] = { 1, 0, 0 };

	double imu = 1 / mass[0];
	double r[3] = { X(i,0) - CM(0), X(i,1) - CM(1), X(i,2) - CM(2) };
	double rMag = sqrt(DOT(r,r));
	double v[3] = { V(i,0) - VCM(0), V(i,1) - VCM(1), V(i,2) - VCM(2) };
	//cout << "rMag:" << rMag << endl;
	KeplerianElements *ke = new KeplerianElements;
	double h[3] = CROSS(r,v);
	double hMag = sqrt(DOT(h,h));

	double e[3];
	for (int i = 0; i < 3; i++)
		e[i] = (imu * DOT(v,v) * r[i]) - (imu * DOT(r,v) * v[i])
				- (r[i] / rMag);

	ke->e = sqrt(DOT(e,e));

	ke->a = imu * DOT(h,h) / (1 - (ke->e * ke->e));

	ke->inc = acos(DOT(zHat,h) / hMag);

	double n[3] = CROSS(zHat,h);
	double nMag = sqrt(DOT(n,n));

	ke->node = acos(DOT(xHat,n) / nMag);
	if (n[1] < 0.0)
		ke->node = 2 * M_PI - ke->node;

	ke->w = acos(DOT(n,e) / (nMag * ke->e));
	if (e[2] < 0.0)
		ke->w = 2 * M_PI - ke->w;

	//True Anomaly
	double theta = acos(DOT(r,e) / (rMag * ke->e));
	if (DOT(r,v) < 0.0)
		theta = 2 * M_PI - theta;

	//Eccentric Anomaly
	double E = acos((ke->e + cos(theta)) / (1 + ke->e * cos(theta)));
	if ((theta < 2 * M_PI) && (theta > M_PI))
		E = 2 * M_PI - E;

	ke->M = E - ke->e * sin(E);
	return ke;
}

double Planets::TotalMass() const {
	double mtot = 0.0;
	for (int i = 0; i < numPlanets; i++)
		mtot += mass[i];
	return mtot;
}

double Planets::CM(int k) const {
	double cm = 0.0, mtot = TotalMass();

	if (mtot == 0.0)
		return 0.0;

	for (int i = 0; i < numPlanets; i++)
		cm += mass[i] * X(i,k);

	return cm / mtot;
}

double Planets::VCM(int k) const {
	double vcm = 0.0, mtot = TotalMass();

	if (mtot == 0.0)
		return 0.0;

	for (int i = 0; i < numPlanets; i++)
		vcm += mass[i] * V(i,k);

	return vcm / mtot;
}

bool Planets::IsCM() const {
	bool ans = true;
	for (int i = 0; i < 3; i++)
		if (abs(CM(i)) > SMALLNUM || abs(VCM(i)) > SMALLNUM) {
			ans = false;
			break;
		}
	return ans;
}

void Planets::MoveToCM() {
	double cm[3] = { CM(0), CM(1), CM(2) };
	double vcm[3] = { VCM(0), VCM(1), VCM(2) };

	for (int i = 0; i < numPlanets; i++) {
		for (int j = 0; j < numPlanets; j++) {
			X(i,j) -= cm[i];
			V(i,j) -= vcm[i];
		}
	}
}

double * Planets::P(int i) const {
	double * P = new double[3];
	P[0] = (mass[i] / Gravity::G) * V(i,0);
	P[1] = (mass[i] / Gravity::G) * V(i,1);
	P[2] = (mass[i] / Gravity::G) * V(i,2);
	return P;
}

double * Planets::L(int i) const {
	double * L = new double[3];
	L[0] = (mass[i] / Gravity::G) * (X(i,1) * V(i,2) - X(i,2) * V(i,1));
	L[1] = (mass[i] / Gravity::G) * (X(i,2) * V(i,0) - X(i,0) * V(i,2));
	L[2] = (mass[i] / Gravity::G) * (X(i,0) * V(i,1) - X(i,1) * V(i,0));
	return L;
}

double Planets::K(int i) const {
	return 0.5 * (mass[i] / Gravity::G) * (V(i,0) * V(i,0) + V(i,1) * V(i,1)
			+ V(i,2) * V(i,2));
}

double Planets::U(int i) const {
	double U = 0.0;
	for (int cheetos = 0; cheetos < numPlanets; cheetos++) {
		if (i != cheetos) {
			double r[3] = R(i,cheetos);
			U -= mass[cheetos] * mass[i] / (Gravity::G * sqrt(DOT(r,r)));
		}
	}
	return U;
}

double Planets::E(int i) const {
	return K(i) + U(i);
}

void Planets::grow() {
	capacity++;

	double *massTemp = mass;
	double *yTemp = y;

	mass = new double[capacity];
	y = new double[6 * capacity];

	for (int i = 0; i < numPlanets; i++)
		mass[i] = massTemp[i];

	for (int i = 0; i < 6 * numPlanets; i++)
		y[i] = yTemp[i];

	delete[] massTemp;
	delete[] yTemp;
}
