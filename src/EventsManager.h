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
#include "Event.h"

typedef void (* Action)(Event *event);

class EventsManager {
public:

	EventsManager();
	EventsManager(int initialCapacity);
	~EventsManager();

	bool AddEvent(Event& event, Action action);

	bool CheckEvents(double t, Planets *p);
	double GetNextEvent();

private:
	int capacity;
	int numEvents;

	Event **events;
	Action *actions;

	void Grow();

};

#endif /* EVENTSMANAGER_H_ */
