// ***********************************************************************
// EventsManager.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Header file for EventsManager.cc

#ifndef EVENTSMANAGER_H_
#define EVENTSMANAGER_H_

#include "Planets.h"
#include "EventParams.h"

typedef void (* Action)(EventParams *params);
typedef bool (* Test)(EventParams *params);

class EventsManager {
public:
	EventsManager();
	EventsManager(int initialCapacity);
	~EventsManager();

	bool AddEvent(Test test, Action action, EventParams *params, int numParams);
	bool AddEvent(int eventType, EventParams *params, Action action);

	bool AddAllOrbitsEvent(EventParams *params, Action action);
	bool AddClosestOrbitEvent(EventParams *params, Action action);
	bool AddTimedPrintEvent(EventParams *params, Action action);
	bool AddDeltaVEvent(EventParams *params, Action action);

	bool CheckEvents(double t, Planets *p);
	double GetNextEvent();

	static const int ALL_ORBITS;
	static const int CLOSEST_ORBIT;
	static const int TIMED_PRINT;
	static const int DELTA_V;

private:
	int capacity;
	int numEvents;

	EventParams **params;
	Test *tests;
	Action *actions;

	static bool CheckAllOrbits(EventParams *params);
	static bool CheckClosestOrbit(EventParams *params);
	static bool CheckTimedPrint(EventParams *parmas);
	static bool CheckDeltaV(EventParams *params);

	void Grow();

};

#endif /* EVENTSMANAGER_H_ */
