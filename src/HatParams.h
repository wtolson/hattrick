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
	HatParams(const HatParams& hp);
	~HatParams();

	bool IsAwesome() const;
	double GetStep(double t, double h, const Planets &p) const;

	static void PrintTime(const Event *event);
	static void PrintStateVectors(const Event *event);
	static void PrintKeplerianElements(const Event *event);
	static void PrintDiagnostics(const Event *event);

	const Planets& GetPlanets() const;

private:
	bool success;
	Planets * p;
	EventsManager * em;

	Action GetPrint(int printType);
	void printHelp(string errMssg = "");
};

istream& operator>>(istream& i, Action& a);

inline bool HatParams::IsAwesome() const {
	return success;
}

#endif /* HATPARAMS_H */
