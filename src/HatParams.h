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
	double GetStep(double t, double h, Planets *p);

	static void PrintTime(EventParams *params);
	static void PrintStateVectors(EventParams * params);
	static void PrintKeplerianElements(EventParams *params);
	static void PrintDiagnostics(EventParams *params);

	Planets GetPlanets();

private:
	bool success;
	Planets * p;
	EventsManager * em;

	Action GetPrint(int printType);
	void printHelp(string errMssg = "");
};

#endif /* HATPARAMS_H */
