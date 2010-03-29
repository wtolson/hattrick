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

class Event {
public:
	Event();
	virtual ~Event();

	virtual Event *NewCopy() = 0;
	virtual bool Check() = 0;

	double GetNextEvent();
	static void SetSnapShot(double time, Planets *p);

	virtual double GetTime();
	virtual Planets *GetPlanets();

protected:
	double nextEvent;
	static double time;
	static Planets *p;
};

class OrbitEvent: public Event {
public:
	OrbitEvent();
	~OrbitEvent();

	Event *NewCopy();
	bool Check();

	double GetTime();
	Planets *GetPlanets();

private:
	static const int paramType = 3;

	bool first; // True if uninitialized
	bool findAll;
	bool found;
	int planetIndex; // Planet index #
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

class TimedEvent: public Event {
public:
	TimedEvent(double printSkip, double firstPrint = 0);
	~TimedEvent();

	Event *NewCopy();
	bool Check();

private:
	double printSkip; // The time to skip between prints.
	double tPrint; // Time to print
};

//Inline functions:

inline double Event::GetNextEvent() {
	return nextEvent;
}

inline void Event::SetSnapShot(double time, Planets *p) {
	Event::time = time;
	Event::p = p;
}

inline double Event::GetTime() {
	return time;
}

inline Planets *Event::GetPlanets() {
	return p;
}

inline double OrbitEvent::GetTime() {
	return tLast;
}

inline Planets *OrbitEvent::GetPlanets() {
	return &pLast;
}

#endif /* EVENT_H_ */
