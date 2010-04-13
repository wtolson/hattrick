// ***********************************************************************
// HatParams.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Header file for HatParams.cc


#ifndef HATPARAMS_H
#define HATPARAMS_H

#include <string>
#include "EventsManager.h"
#include "Planets.h"

using namespace std;

class HatParams {
public:
	int N, stepType;
	double t0, t1, hmin, hmax, accr;

	HatParams(int argc, char** argv);
	~HatParams();

	bool IsAwesome();
	double GetStep(double t, double h, const Planets &p);

	static void PrintTime(Event *event);
	static void PrintStateVectors(Event *event);
	static void PrintKeplerianElements(Event *event);
	static void PrintDiagnostics(Event *event);

	Planets GetPlanets();

private:
	bool success;
	Planets * p;
	EventsManager * em;

	Action GetPrint(int printType);
	void printHelp(string errMssg = "");
};

istream& operator>>(istream& i, Action& a);

#endif /* HATPARAMS_H */
