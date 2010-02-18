// ***********************************************************************
// HatParams.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Class to easily add and manage events within the simulation.

#include <algorithm>
#include <cmath>
#include "EventsManager.h"

#include <iostream>
using namespace std;

const int EventsManager::TIMED_PRINT = 0;
const int EventsManager::ALL_ORBITS = 1;
const int EventsManager::CLOSEST_ORBIT = 2;
const int EventsManager::DELTA_V = 3;

EventsManager::EventsManager() {
	capacity = 0;
	numEvents = 0;

	params = 0;
	tests = 0;
	actions = 0;
}

EventsManager::EventsManager(int initialCapacity) {
	capacity = initialCapacity;
	numEvents = 0;

	params = new EventParams*[capacity];
	tests = new Test[capacity];
	actions = new Action[capacity];
}

EventsManager::~EventsManager() {
	delete[] actions;
	delete[] tests;
	for (int i = 0; i < numEvents; i++) {
		switch (params[i]->customParamsType) {
		case 1:
			delete (int *) params[i]->customParams;
			break;
		case 2:
			delete (double *) params[i]->customParams;
			break;
		case 3:
			delete (AllOrbitsParams *) params[i]->customParams;
			break;
		case 4:
			delete(PrintParams *) params[i]->customParams;
			break;
		case 5:
			delete (ClosestOrbitParams *) params[i]->customParams;
			break;
		default:
			break;
		}
		delete params[i];
	}
	delete[] params;
}

//bool EventsManager::AddEvent(Test test, Action action, double params[],
//		int numParams) {
//	if (numEvents == capacity)
//		Grow();
//
//	this->numParams[numEvents] = numParams;
//	this->params[numEvents] = new double[numParams];
//	for (int i; i < numParams; i++)
//		this->params[numEvents][i] = params[i];
//	tests[numEvents] = test;
//	actions[numEvents] = action;
//
//	numEvents++;
//	return true;
//}

bool EventsManager::AddEvent(int eventType, EventParams *params, Action action) {

	switch (eventType) {
	case TIMED_PRINT:
		return AddTimedPrintEvent(params, action);

	case ALL_ORBITS:
		return AddAllOrbitsEvent(params, action);

	case CLOSEST_ORBIT:
		return AddClosestOrbitEvent(params, action);

	case DELTA_V:
		return AddDeltaVEvent(params, action);

	default:
		return false;
	}
	return false;
}

bool EventsManager::AddAllOrbitsEvent(EventParams *params, Action action) {
	if (numEvents == capacity)
		Grow();

	this->params[numEvents] = params;
	tests[numEvents] = &EventsManager::CheckAllOrbits;
	actions[numEvents] = action;

	numEvents++;
	return true;
}

bool EventsManager::AddClosestOrbitEvent(EventParams *params, Action action) {
	if (numEvents == capacity)
		Grow();

	this->params[numEvents] = params;
	//this->params[numEvents][0] = -1;
	//this->params[numEvents][9] = 0;
	tests[numEvents] = &EventsManager::CheckClosestOrbit;
	actions[numEvents] = action;

	numEvents++;
	return true;
}

bool EventsManager::AddTimedPrintEvent(EventParams *params, Action action) {
	if (numEvents == capacity)
		Grow();

	this->params[numEvents] = params;
	tests[numEvents] = &EventsManager::CheckTimedPrint;
	actions[numEvents] = action;

	numEvents++;
	return true;
}

bool EventsManager::AddDeltaVEvent(EventParams *params, Action action) {
	if (numEvents == capacity)
		Grow();

	this->params[numEvents] = params;
	tests[numEvents] = &EventsManager::CheckDeltaV;
	actions[numEvents] = action;

	numEvents++;
	return true;
}

bool EventsManager::CheckEvents(double t, Planets * p) {
	bool status = false;
	for (int i = 0; i < numEvents; i++) {
		params[i]->time = t;
		params[i]->nextEvent = -1;
		params[i]->p = p;
		if (tests[i](params[i])) {
			(actions[i])(params[i]);
			status = true;
		}
	}
	return status;
}

double EventsManager::GetNextEvent() {
	int count = 0;
	for (; count < numEvents; count++) {
		if (params[count]->nextEvent != -1.0)
			break;
	}
	if (count==numEvents)
		return -1.0;

	double nextEvent = params[count]->nextEvent;
	for (; count < numEvents; count++) {
		if (params[count]->nextEvent != -1.0)
			nextEvent = min(nextEvent, params[count]->nextEvent);
	}

	return nextEvent;
}

bool EventsManager::CheckAllOrbits(EventParams *ep) {
	AllOrbitsParams *op = (AllOrbitsParams *) ep->customParams;
	Planets &p = *ep->p;

	if (op->first) {
		op->xLast = (p.x(op->planetIndex, 1) - p.x(0, 1));
		op->first = false;
		return false;
	}

	bool success = false;
	double xThis = (p.x(op->planetIndex, 1) - p.x(0, 1));
	if (xThis >= 0.0 && op->xLast < 0.0) {
		op->orbits++;
		op->tLast = ep->time;
		success = true;
	}

	double vThis = (p.v(op->planetIndex, 1) - p.v(0, 1));

	if (xThis < 0.0 && vThis > 0.0) {
		ep->nextEvent = -xThis / (2 * vThis);
	} else {
		ep->nextEvent = -1.0;
	}

	op->xLast = xThis;
	return success;
}

bool EventsManager::CheckClosestOrbit(EventParams *ep) {
	ClosestOrbitParams *op = (ClosestOrbitParams *) ep->customParams;
	Planets &p = *ep->p;

	if (op->first == 0) {
		op->xLast = (p.x(op->planetIndex, 1) - p.x(0, 1));
		op->first = 1;
		return false;
	}

	if (op->first != 1)
		return false;

	double xThis = (p.x(op->planetIndex, 1) - p.x(0, 1));
	if (xThis >= 0.0 && op->xLast < 0.0) {
		op->orbits++;
		if (abs(ep->time - op->orbitToFind) > abs(op->tLast
				- op->orbitToFind)) {
			op->first = 2;
			ep->time = op->tLast;
			ep->p = &op->pLast;
			return true;
		}
		op->tLast = ep->time;
		op->pLast = Planets(p);
	}

	double vThis = (p.v(op->planetIndex, 1) - p.v(0, 1));

	if (xThis < 0.0 && vThis > 0.0) {
		ep->nextEvent = -xThis / (2 * vThis);
	} else {
		ep->nextEvent = -1.0;
	}
	op->xLast = xThis;
	return false;
}

bool EventsManager::CheckTimedPrint(EventParams *ep) {
	PrintParams *pp = (PrintParams *) ep->customParams;

	bool success = false;
	if (ep->time >= pp->tPrint) {
		pp->tPrint += pp->printSkip;
		success = true;
	}

	ep->nextEvent = pp->tPrint - ep->time;
	return success;
}

bool EventsManager::CheckDeltaV(EventParams *params) {
	//TODO
	return false;
}

void EventsManager::Grow() {
	capacity++;

	EventParams **paramsTemp = params;
	Test *testsTemp = tests;
	Action *actionsTemp = actions;

	params = new EventParams*[capacity];
	tests = new Test[capacity];
	actions = new Action[capacity];

	for (int i = 0; i < numEvents; i++) {
		params[i] = paramsTemp[i];
		tests[i] = testsTemp[i];
		actions[i] = actionsTemp[i];
	}

	delete[] actionsTemp;
	delete[] testsTemp;
	delete[] paramsTemp;

}
