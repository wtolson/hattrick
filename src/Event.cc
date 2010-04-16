// ***********************************************************************
// Event.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
//

#include "Event.h"

const Planets *Event::p = 0;
double Event::time = 0;

Event::Event() :
	nextEvent(-1) {
}

Event::~Event() {
}

TimedEvent::TimedEvent(double printSkip, double initialPrint) :
	printSkip(printSkip), tPrint(initialPrint) {
}

TimedEvent::~TimedEvent() {
}

Event *TimedEvent::NewCopy() const {
	return new TimedEvent(*this);
}

bool TimedEvent::Check() {
	bool success = false;
	if (time >= tPrint) {
		tPrint += printSkip;
		success = true;
	}

	nextEvent = tPrint - time;
	return success;
}

OrbitEvent::OrbitEvent(double planetIndex, bool findAll, double orbitToFind) :
	first(true), found(false), planetIndex(planetIndex), findAll(findAll),
			orbitToFind(orbitToFind), xLast(0), tLast(0), pLast(0) {
}

OrbitEvent::~OrbitEvent() {
}

Event *OrbitEvent::NewCopy() const {
	return new OrbitEvent(*this);
}

bool OrbitEvent::Check() {

	nextEvent = -1;

	if (first) {
		xLast = (p->x(planetIndex, 1) - p->x(0, 1));
		first = false;
		return false;
	}

	if (found)
		return false;

	bool success = false;
	double xThis = (p->x(planetIndex, 1) - p->x(0, 1));
	if (xThis >= 0.0 && xLast < 0.0) {
		orbits++;
		success = findAll;

		if (!success) {
			if (abs(time - orbitToFind) > abs(tLast - orbitToFind)) {
				first = 2;
				return true;
			}
			pLast = *p;
		}

		tLast = time;
	}

	double vThis = (p->v(planetIndex, 1) - p->v(0, 1));

	if (xThis < 0.0 && vThis > 0.0) {
		nextEvent = -xThis / (2 * vThis);
	}

	xLast = xThis;
	return success;
}
