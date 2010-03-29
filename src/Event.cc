// ***********************************************************************
// Event.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
//

#include <algorithm>
#include "Event.h"

Planets *Event::p = 0;

Event::Event() :
	nextEvent(-1) {

}

Event *TimedEvent::NewCopy() {
	return new TimedEvent(*this);
}

TimedEvent::TimedEvent(double printSkip, double firstPrint) :
	printSkip(printSkip), tPrint(firstPrint) {

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

bool OrbitEvent::Check() {

	nextEvent = -1;

	if (first) {
		xLast = (p->x(planetIndex, 1) - p->x(0, 1));
		first = false;
		found = false;
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
			pLast = Planets(*p);
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
