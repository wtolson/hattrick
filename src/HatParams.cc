// ***********************************************************************
// HatParams.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "HatParams.h"

using namespace std;

HatParams::HatParams(int argc, char** argv) :
	success(true), p(0), em(0) {

	p = new Planets;

	if (argc == 1) {
		printHelp();
		return;
		//throw HatExceptions((string("No Arguments.")));
	}

	ifstream ifs(argv[1], ifstream::in);

	if (!ifs.is_open()) {
		cerr << "The file \"" << argv[1] << " does not exist." << endl;
		printHelp();
		return;
	}

	int n1, n2 = 0, numEvents;

	ifs >> n1 >> t0 >> t1 >> hmin >> hmax >> accr >> stepType >> numEvents;

	if (stepType != 0 && stepType != 1 && stepType != 2) {
		printHelp("Invalid integration step type.");
		return;
	}

	if (numEvents <= 0) {
		cerr << "Warning: No events specified." << endl;
	}

	if (argc >= 3)
		n2 = atoi(argv[2]);

	if (argc >= 3 && argc != (3 + 2 * n2)) {
		if (argc > (3 + 2 * n2))
			printHelp("Too many arguments.");
		if (argc < (3 + 2 * n2))
			printHelp("Too few arguments.");
		return;
	}

	p = new Planets(n1 + n2);
	for (int i = 0; i < n1; i++) {
		int cs;
		double m;
		ifs >> cs >> m;
		bool status;
		switch (cs) {
		case 0:
			double x[3], v[3];

			for (int k = 0; k < 3; k++)
				ifs >> x[k];
			for (int k = 0; k < 3; k++)
				ifs >> v[k];

			status = p->AddPlanet(m, x, v);
			break;

		case 1:
			KeplerianElements ke;

			ifs >> ke.a >> ke.e >> ke.inc >> ke.node >> ke.w >> ke.M;

			status = p->AddPlanet(m, ke);
			break;

		default:
			cerr << "Unrecognized coordinate system for planet " << i + 1
					<< " in \"" << argv[1] << "\"." << endl;
			printHelp();
			return;
		}
		if (!status) {
			cerr << "Error adding planet " << i + 1 << " in \"" << argv[1]
					<< "\"." << endl;
			printHelp();
		}
	}

	for (int i = n1; i < (n1 + n2); i++) {
		bool status = p->AddPlanet(atof(argv[3 + 2 * (i - n1)]), atof(argv[4
				+ 2 * (i - n1)]));
		if (!status) {
			cerr << "Error adding planet " << i - n1 + 1
					<< " from the command line." << endl;
			printHelp();
		}
	}

	em = new EventsManager(numEvents);

	Action action;
	Event *event;

	for (int i = 0; i < numEvents; i++) {
		int eventType;
		ifs >> eventType;
		switch (eventType) {
		case 0: {
			//Create timed event.
			double printSkip, initialPrint;
			ifs >> printSkip >> initialPrint;
			event = new TimedEvent(printSkip, initialPrint);
			break;
		}
		case 1: {
			//Create All Orbits event.
			double planetIndex;
			bool findAll;
			ifs >> planetIndex >> findAll;
			if (findAll) {
				event = new OrbitEvent(planetIndex, findAll);
				break;
			}
			double orbitToFind;
			ifs >> orbitToFind;
			event = new OrbitEvent(planetIndex, findAll, orbitToFind);
			break;
		}
		default: {
			cerr << "Unrecognized event type (" << eventType << ") for event "
					<< i + 1 << " in \"" << argv[1] << "\"." << endl;
			printHelp();
			return;
		}
		}
		ifs >> action;
		if (action == NULL) {
			delete event;
			cerr << "Unrecognized print type for event " << i + 1 << " in \""
					<< argv[1] << "\"." << endl;
			printHelp();
			return;
		}
		bool status = em->AddEvent(*event, action);
		delete event;
		if (!status) {
			cerr << "Error adding event " << i + 1 << " in \"" << argv[1]
					<< "\"." << endl;
			printHelp();
		}
	}
	ifs.close();
	p->MoveToCM();
}

HatParams::HatParams(const HatParams& hp) :
	N(hp.N), stepType(hp.stepType), t0(hp.t0), t1(hp.t1), hmin(hp.hmin), hmax(
			hp.hmax), accr(hp.accr), success(hp.success), p(new Planets(*hp.p)),
			em(new EventsManager(*hp.em)) {
}

HatParams::~HatParams() {
	delete em;
	delete p;
}

const Planets& HatParams::GetPlanets() const {
	return *p;
}

double HatParams::GetStep(double t, double h, const Planets &p) const {

	em->CheckEvents(t, p);

	double hTemp = em->GetNextEvent();
	if (hTemp != -1) {
		h = min(h, hTemp);
	}
	h = min(h, hmax);
	h = max(h, hmin);

	return h;
}

istream& operator>>(istream& i, Action& a) {
	int type = -1;
	i >> type;
	switch (type) {
	case 0:
		a = &HatParams::PrintTime;
		break;
	case 1:
		a = &HatParams::PrintStateVectors;
		break;
	case 2:
		a = &HatParams::PrintKeplerianElements;
		break;
	case 3:
		a = &HatParams::PrintDiagnostics;
		break;
	default:
		cout << "type: " << type << endl;
		a = NULL;
		break;
	}
	return i;
}

void HatParams::PrintTime(const Event *event) {
	cout << event->GetTime() << endl;
}

void HatParams::PrintStateVectors(const Event *event) {
	const Planets &p = *event->GetPlanets();

	cout << event->GetTime() << " ";
	for (int i = 0; i < p.N(); i++) {
		for (int k = 0; k < 3; k++)
			cout << " " << p.x(i, k);
		for (int k = 0; k < 3; k++)
			cout << " " << p.v(i, k);
	}
	cout << endl;
}

void HatParams::PrintKeplerianElements(const Event *event) {
	const Planets &p = *event->GetPlanets();

	cout << event->GetTime() << " ";
	cout.setf(ios_base::scientific);
	for (int i = 1; i < p.N(); i++) {
		KeplerianElements *ke = p.GetKeplerian(i);
		cout << ke->a << " ";
		cout << ke->e << " ";
		cout << ke->inc << " ";
		cout << ke->node << " ";
		cout << ke->w << " ";
		cout << ke->M << " ";
		delete ke;
	}
	cout.unsetf(ios_base::scientific);
	cout << endl;
}

void HatParams::PrintDiagnostics(const Event *event) {
	const Planets &p = *event->GetPlanets();

	cout << event->GetTime() << " ";
	double L[3] = { 0.0, 0.0, 0.0 };

	for (int funyuns = 0; funyuns < p.N(); funyuns++) {
		double * tempL = p.L(funyuns);
		for (int cheetos = 0; cheetos < 3; cheetos++)
			L[cheetos] += tempL[cheetos];
		delete tempL;
	}
	cout.setf(ios_base::scientific);
	cout << sqrt(L[0] * L[0] + L[1] * L[1] + L[2] * L[2]) << " ";

	double E = 0.0;
	for (int funyuns = 0; funyuns < p.N(); funyuns++)
		E += p.E(funyuns);

	cout << E << endl;

	cout.unsetf(ios_base::scientific);
}

void HatParams::printHelp(string errMssg) {
	success = false;
	if (!errMssg.empty())
		cerr << errMssg << endl;
	cerr << "Input format:" << endl;
	cerr << "    ./hattrick baseSys" << endl;
	cerr << "    ./hattrick baseSys n m1 r1 ... mn rn" << endl << endl;

	cerr << "Where:" << endl;
	cerr << "    baseSys: Name of the file specifying the base system." << endl;
	cerr << "    n (optional): Number of perturbing bodies." << endl;
	cerr << "    mn (optional): Mass of the nth perturbing body." << endl;
	cerr << "    rn (optional): Radius of the nth perturbing body." << endl
			<< endl;

	cerr << "Base system file format:" << endl;
	cerr << "    n t0 t1 hmin hmax accr stepType numEvents" << endl;
	cerr << "    cs1 m1 x1 y1 z1 vx1 vy1 vz1" << endl;
	cerr << "    cs2 m2 a2 e2 i2 node2 w2 M2" << endl;
	cerr << "        ..." << endl;
	cerr << "    csn mn xn yn zn vxn vyn vzn" << endl;
	cerr << "    eventType_1 eventParams_1..." << endl;
	cerr << "        ..." << endl;
	cerr << "    eventType_n eventParams_n..." << endl << endl;

	cerr << "Where:" << endl;
	cerr << "    n: Number of bodies in the base system." << endl;
	cerr << "    t0: Initial system time." << endl;
	cerr << "    t1: Final system time." << endl;
	cerr << "    hmin: Minimum time step." << endl;
	cerr << "    hmax: Max time step.  Also the initial step size." << endl;
	cerr << "    accr: Accuracy paramater." << endl;
	cerr << "    stepType: 0 for rk45, 1 for rk8pd, 2 for bsimp." << endl;
	cerr << "    numEvents: Number of events to read in." << endl;
	cerr << "    cs: Coordinate system (0 for state vectors," << endl;
	cerr << "                           1 for keplarian elements)." << endl
			<< endl;
	cerr << "Events:" << endl;
	cerr << "    eventType: 0 for Timed Print," << endl;
	cerr << "               1 for All Orbits," << endl;
	cerr << "               2 for Closest Orbit." << endl;
	cerr << "    Timed Print eventParams:" << endl;
	cerr << "        printSkip printType" << endl;
	cerr << "    All Orbits eventParams:" << endl;
	cerr << "        planetIndex printType" << endl;
	cerr << "    Closest Orbits eventParams:" << endl;
	cerr << "        planetIndex orbitToFind printType" << endl << endl;

	cerr << "Where:" << endl;
	cerr << "    printType: Directions for printing." << endl;
	cerr << "        0) Time only" << endl;
	cerr << "        1) State Vectors" << endl;
	cerr << "        2) Keplerian Elements" << endl;
	cerr << "        3) Total Angular momentum and Energy" << endl;
	cerr << "    printSkip: Time to skip between print outs." << endl;
	cerr << "    planetIndex: Index of planet to watch." << endl;
	cerr << "    orbitToFind: Time for which to find the closest orbit."
			<< endl;
}

