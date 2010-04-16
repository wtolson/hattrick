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

typedef void (* Action)(const Event *event);

class EventsManager {
public:

	EventsManager();
	EventsManager(int initialCapacity);
	EventsManager(const EventsManager& em);
	~EventsManager();

	bool AddEvent(const Event& event, Action action);

	bool CheckEvents(double t, const Planets &p);
	double GetNextEvent() const;

private:
	int capacity;
	int numEvents;

	Event **events;
	Action *actions;

	void Grow();

};

#endif /* EVENTSMANAGER_H_ */
