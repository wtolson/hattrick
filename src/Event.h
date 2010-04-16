// ***********************************************************************
// Event.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Definition of the Event base class and several custom event types used by the built in tests.

#ifndef EVENT_H_
#define EVENT_H_

#include "Planets.h"
#include <istream>
using namespace std;

// Purely virtual base class for the events our 'events manager' will handle.
class Event {
public:
	virtual ~Event();

	virtual Event *NewCopy() const = 0; // Creates a new copy of the derived class on the heap.
	virtual bool Check() = 0; // Member function to check if the event has happened.

	// Returns the time till this event happens next.
	// '-1' equates to 'I don't know.
	double GetNextEvent() const;

	// Set a shared snapshot of the time and planets
	// for the events to check against.
	static void SetSnapShot(double time, const Planets &p);

	// Accessors:
	virtual double GetTime() const;
	virtual const Planets *GetPlanets() const;

protected:
	// Purely virtual. Do not create one!
	Event();

	double nextEvent;
	static double time;
	static const Planets *p;
};


// An event that triggers after a timed amount.
class TimedEvent: public Event {
public:
	TimedEvent(double skipTime, double initialEvent);
	~TimedEvent();

	Event *NewCopy() const;
	bool Check();

private:
	const double skipTime; // The time to skip between events.
	double tTrigger; // Next event time.
};


// Event that triggers when a planet orbits the star (planet indexed 0).
class OrbitEvent: public Event {
public:
	OrbitEvent(double planetIndex, bool findAll, double orbitToFind = 0);
	~OrbitEvent();

	Event *NewCopy() const;
	bool Check();

	double GetTime() const;
	const Planets *GetPlanets() const;

private:
	static const int paramType = 3;

	bool first; // True if uninitialized
	bool found;

	int planetIndex; // Planet index #
	bool findAll;
	double orbitToFind; // Time of orbit to find


	double xLast; // Last delta x
	double tLast; // Time of the last orbit
	Planets pLast; // Planets at last orbit
	int orbits; // # Orbits


	//TODO: Implement direction support
	double dx; // Direction
	double dy;
	double dz;
};

//Inline for Events:

inline double Event::GetNextEvent() const {
	return nextEvent;
}

inline double Event::GetTime() const {
	return time;
}

inline const Planets *Event::GetPlanets() const {
	return p;
}

inline void Event::SetSnapShot(double time, const Planets &p) {
	Event::time = time;
	Event::p = &p;
}

//Inline for OrbitEvent:

inline double OrbitEvent::GetTime() const {
	if (findAll) {
		return time;
	} else {
		return tLast;
	}
}

inline const Planets *OrbitEvent::GetPlanets() const {
	if (findAll) {
		return p;
	} else {
		return &pLast;
	}
}

//Random inline:

inline double abs(double num) {
	if (num < 0.0) {
		num = -num;
	}
	return num;
}

#endif /* EVENT_H_ */
