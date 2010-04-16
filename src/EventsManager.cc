// ***********************************************************************
// HatParams.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Class to easily add and manage events within the simulation.


#include <cmath>
#include "EventsManager.h"

#include <iostream>
using namespace std;

EventsManager::EventsManager() {
	capacity = 0;
	numEvents = 0;

	events = 0;
	actions = 0;
}

EventsManager::EventsManager(int initialCapacity) {
	capacity = initialCapacity;
	numEvents = 0;

	events = new Event*[capacity];
	actions = new Action[capacity];
}

EventsManager::EventsManager(const EventsManager& em) :
	capacity(em.capacity), numEvents(em.numEvents),
			events(new Event*[capacity]), actions(new Action[capacity]) {
	for (int i = 0; i < numEvents; i++) {
		events[i] = em.events[i]->NewCopy();
		actions[i] = em.actions[i];
	}
}

EventsManager::~EventsManager() {
	delete[] actions;
	for (int i = 0; i < numEvents; i++)
		delete events[i];
	delete[] events;
}

bool EventsManager::AddEvent(const Event& event, Action action) {
	if (numEvents == capacity)
		Grow();

	events[numEvents] = event.NewCopy();
	actions[numEvents] = action;

	numEvents++;
	return true;
}

bool EventsManager::CheckEvents(double t, const Planets &p) {
	bool status = false;
	Event::SetSnapShot(t, p);
	for (int i = 0; i < numEvents; i++) {
		if (events[i]->Check()) {
			(actions[i])(events[i]);
			status = true;
		}
	}
	return status;
}

double EventsManager::GetNextEvent() const {
	int count = 0;
	for (; count < numEvents; count++) {
		if (events[count]->GetNextEvent() != -1.0)
			break;
	}
	if (count == numEvents)
		return -1.0;

	double nextEvent = events[count]->GetNextEvent();
	for (; count < numEvents; count++) {
		if (events[count]->GetNextEvent() != -1.0)
			nextEvent = min(nextEvent, events[count]->GetNextEvent());
	}

	return nextEvent;
}

void EventsManager::Grow() {
	capacity++;

	Event **eventsTemp = events;
	Action *actionsTemp = actions;

	events = new Event*[capacity];
	actions = new Action[capacity];

	for (int i = 0; i < numEvents; i++) {
		events[i] = eventsTemp[i];
		actions[i] = actionsTemp[i];
	}

	delete[] actionsTemp;
	delete[] eventsTemp;
}

