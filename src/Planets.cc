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

Planets::Planets() {
	capacity = 0;
	numPlanets = 0;
	mass = 0;
	y = 0;
	isDynamic = 0;
	dynamicConstants = 0;
}

Planets::Planets(unsigned int N) {
	capacity = N;
	numPlanets = 0;
	mass = new double[capacity];
	y = new double[6 * capacity];
	isDynamic = new bool[capacity];
	dynamicConstants = new KeplerianElements*[capacity];
}

Planets::Planets(const Planets& p) {
	this->capacity = p.capacity;
	this->numPlanets = p.numPlanets;
	this->mass = new double[capacity];
	this->y = new double[6 * capacity];
	this->isDynamic = new bool[capacity];
	this->dynamicConstants = new KeplerianElements*[capacity];

	for (int i = 0; i < numPlanets; i++) {
		this->mass[i] = p.mass[i];
		this->isDynamic[i] = p.isDynamic[i];
		this->dynamicConstants[i] = p.dynamicConstants[i];
	}

	for (int i = 0; i < 6 * numPlanets; i++)
		this->y[i] = p.y[i];
}

Planets::~Planets() {
	delete[] mass;
	mass = 0;
	delete[] y;
	y = 0;
	delete[] isDynamic;
	isDynamic = 0;
	for (int i = 0; i < numPlanets; i++)
		delete dynamicConstants[i];
	delete[] dynamicConstants;
	dynamicConstants = 0;
}

bool Planets::AddPlanet(double mass, double x[3], double v[3], bool isDynamic) {
	if (numPlanets == capacity)
		return false;
	this->mass[numPlanets] = Gravity::G * mass;
	for (int i = 0; i < 3; i++)
		X(numPlanets, i) = x[i];
	for (int i = 0; i < 3; i++)
		V(numPlanets, i) = v[i];
	this->isDynamic[numPlanets] = isDynamic;
	if (isDynamic)
		this->dynamicConstants[numPlanets] = this->GetKeplerian(numPlanets);
	numPlanets++;

	return true;
}

bool Planets::AddPlanet(double mass, double a, double e, double inc,
		double node, double w, double M, bool isDynamic) {
	if (numPlanets == capacity)
		return false;
	this->mass[numPlanets] = Gravity::G * mass;
	this->isDynamic[numPlanets] = isDynamic;
	if (isDynamic) {
		this->dynamicConstants[numPlanets]->a = a;
		this->dynamicConstants[numPlanets]->e = e;
		this->dynamicConstants[numPlanets]->inc = inc;
		this->dynamicConstants[numPlanets]->node = node;
		this->dynamicConstants[numPlanets]->w = w;
		this->dynamicConstants[numPlanets]->M = M;
	}

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
	} while (abs(E - E0) > 1E-5);

	double P[3] = { cos(w) * cos(node) - sin(w) * cos(inc) * sin(node), cos(w)
			* sin(node) + sin(w) * cos(inc) * cos(node), sin(w) * sin(inc) };
	double Q[3] = { -sin(w) * cos(node) - cos(w) * cos(inc) * sin(node),
			-sin(w) * sin(node) + cos(w) * cos(inc) * cos(node), sin(inc)
					* cos(w) };
	double A = a * (cos(E) - e);
	double B = a * sqrt(1 - e * e) * sin(E);

	for (int i = 0; i < 3; i++)
		X(numPlanets, i) = CM(i) + A * P[i] + B * Q[i];

	double Edot = sqrt(this->mass[0] / (a * a * a)) / (1 - e * cos(E));
	A = -a * sin(E) * Edot;
	B = a * sqrt(1 - e * e) * cos(E) * Edot;

	for (int i = 0; i < 3; i++)
		V(numPlanets, i) = VCM(i) + A * P[i] + B * Q[i];
	numPlanets++;

	return true;
}

bool Planets::AddPlanet(double mass, KeplerianElements ke, bool isDynamic) {
	return AddPlanet(mass, ke.a, ke.e, ke.inc, ke.node, ke.w, ke.M, isDynamic);
}

//TODO: Make in-line
//double * Planets::PlanetsPointer() {
//	return y;
//}
//
//inline int Planets::N() {
//	return numPlanets;
//}
//
//double Planets::M(int i) {
//	return mass[i];
//}
//
//double Planets::x(int i, int k) {
//	return X(i,k);
//}
//
//double Planets::v(int i, int k) {
//	return V(i,k);
//}

KeplerianElements *Planets::GetKeplerian(int i) {
	double xHat[3] = { 1.0, 0.0, 0.0 };
	//double yHat[3] = { 0.0, 1.0, 0.0 };
	double zHat[3] = { 0.0, 0.0, 1.0 };

	//TODO: ROTATE COORDS.
	//double refDir[3] = { 1, 0, 0 };

	double imu = 1 / TotalMass();
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

double Planets::CM(int k) {
	double cm = 0.0, mtot = TotalMass();

	if (mtot == 0.0)
		return 0.0;

	for (int i = 0; i < numPlanets; i++)
		cm += mass[i] * X(i,k);

	return cm / mtot;
}

double Planets::VCM(int k) {
	double vcm = 0.0, mtot = TotalMass();

	if (mtot == 0.0)
		return 0.0;

	for (int i = 0; i < numPlanets; i++)
		vcm += mass[i] * V(i,k);

	return vcm / mtot;
}

double Planets::TotalMass() {
	double mtot = 0.0;
	for (int i = 0; i < numPlanets; i++)
		mtot += mass[i];
	return mtot;
}

double * Planets::P(int i) {
	double * P = new double[3];
	P[0] = (mass[i] / Gravity::G) * V(i,0);
	P[1] = (mass[i] / Gravity::G) * V(i,1);
	P[2] = (mass[i] / Gravity::G) * V(i,2);
	return P;
}

double * Planets::L(int i) {
	double * L = new double[3];
	L[0] = (mass[i] / Gravity::G) * (X(i,1) * V(i,2) - X(i,2) * V(i,1));
	L[1] = (mass[i] / Gravity::G) * (X(i,2) * V(i,0) - X(i,0) * V(i,2));
	L[2] = (mass[i] / Gravity::G) * (X(i,0) * V(i,1) - X(i,1) * V(i,0));
	return L;
}

double Planets::K(int i) {
	return 0.5 * (mass[i] / Gravity::G) * (V(i,0) * V(i,0) + V(i,1) * V(i,1)
			+ V(i,2) * V(i,2));
}

double Planets::U(int i) {
	double U = 0.0;
	for (int cheetos = 0; cheetos < numPlanets; cheetos++) {
		if (i != cheetos) {
			double r[3] = R(i,cheetos);
			U -= mass[cheetos] * mass[i] / (Gravity::G * sqrt(DOT(r,r)));
		}
	}
	return U;
}

double Planets::E(int i) {
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
