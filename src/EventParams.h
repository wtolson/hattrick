// ***********************************************************************
// EventParams.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Definition of the EventParams and the customParams used by the built in tests.

#ifndef EVENTPARAMS_H_
#define EVENTPARAMS_H_

struct EventParams {
	double time;
	double nextEvent;
	Planets *p;
	int customParamsType;
	void *customParams;
};

struct AllOrbitsParams {
	static const int paramType = 3;
	double xLast; // Last delta x
	double tLast; // Time of the last orbit
	int planetIndex; // Planet index #
	int orbits; // # Orbits
	//TODO: Implement direction support
	double dx; // Direction
	double dy;
	double dz;
	bool first; // True if uninitialized
};

struct ClosestOrbitParams {
	static const int paramType = 5;
	double xLast; // Last delta x
	double tLast; // Time of the last orbit
	Planets pLast; // Planets at last orbit
	int planetIndex; // Planet index #
	double orbitToFind; // Time of orbit to find
	int orbits; // # Orbits
	//TODO: Implement direction support
	double dx; // Direction
	double dy;
	double dz;
	int first; // Uninitialized (0=true, 1=searching)
};

struct PrintParams {
	static const int paramType = 4;
	double nextEvent; // Next time event is expected to occur
	double printSkip; // The time to skip between prints.
	double tPrint; // Time to print
};

#endif /* EVENTPARAMS_H_ */
