/***********************************************************************
 * hatparams.cpp
 *
 * Copyright 2009 William Olson <wtolson@gmail.com>
 * GNU General Public License v3
 * 
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "hatparams.h"

using namespace std;

hatparams::hatparams(int argc, char** argv)
{
	k = 0.01720209895;
	G = k*k;
	orbits = 0;
	SUCCESS = true;
	
	if (argc == 1) {
		SUCCESS = false;
		printHelp("");
		return;
	}
	
	SUCCESS = true;
	
	ifstream ifs ( argv[1] , ifstream::in );
	
	if (!ifs.is_open()) {
		cerr << "The file " << argv[1] << " does not exist." << endl;
		printHelp("");
		return;
	}
	
	int n1, n2=0;
	
	ifs >> n1;
	ifs >> t0;
	t = t0;
	ifs >> t1;
	ifs >> hmin;
	ifs >> hmax;
	ifs >> accr;
	ifs >> printSkip;
	ifs >> stepType;
	
	if (stepType!=0 && stepType!=1 && stepType!=2) {
		printHelp("Invalid integration step type.");
		return;
	}
	
	if (argc >= 3)
		n2 = atoi(argv[2]);
	
	if (argc >= 3 && argc != (3+2*n2)) {
		if (argc > (3+2*n2))
			printHelp("Too many arguments.");
		if (argc < (3+2*n2))
			printHelp("Too few arguments.");
		return;
	}
	
	N = n1+n2;
	y = new double[6*N];
	M = new double[N];
	for(int i = 0; i < n1; i++)
	{
		ifs >> M[i];
		M[i] *= G;
		for(int k=0; k<3; k++) ifs >> y[yx(i,k)];
		for(int k=0; k<3; k++) ifs >> y[yv(i,k)];
	}
	ifs.close();
	
	for(int i=n1; i<N; i++) {
		M[i] = G*atof(argv[3+2*(i-n1)]);            // M
		for (int k=0; k<3; k++) {
			y[yx(i,k)] = 0.0;
			y[yv(i,k)] = 0.0;
		}
		y[yx(i,0)] = atof(argv[4+2*(i-n1)]);  // x
		y[yv(i,1)] = sqrt(M[0]/r(0,i));       // vy
	}
		
	xLast = xHat(1,0,1);	
	lastOrbit = t0;
	
	diagnostics(&initialE, &initialL);
	
	pertR = x(n1,0);	
	cout << "E0:" << initialE << " L0:" << initialL << endl;
}

bool hatparams::orbit() {
	if (printSkip >= 0) return false;
	if (printSkip == -1) return true;
	double xThis = xHat(1,0,1);
	if (xThis>=0.0 && xLast<0.0)  {
		orbits++;
		cout << "Orbit " << orbits << ": " << (t-lastOrbit) << " days" << endl;
		lastOrbit = t;
		print();
	}
	xLast = xThis;
	return true;
}


bool hatparams::dmbf() {
	if (printSkip != -1) return false;
	double xThis = xHat(1,0,1);
	if (xThis>=0.0 && xLast<0.0)  {
		double weWant = 5106.81015;
		orbits++;
		if (abs(t-weWant) > abs(lastOrbit-weWant)) {
			double E, L;
			diagnostics(&E, &L);
			double dE = (E - initialE) / initialE;
			double dL = (E - initialE) / initialE;
			cout.precision(10);
			cout << (M[2]/G) << " " << pertR << " " << lastOrbit << " " << dE << " " << dL << endl;
			return true;
		}
		lastOrbit = t;
	}
	xLast = xThis;
	return false;
}


void hatparams::diagnostics(double * E, double * L) {
	*L = 0.0;
	for (int i=0; i<N; i++) {
		*L += sqrt( (x(i,1)*v(i,2) + x(i,2)*v(i,1))*(x(i,1)*v(i,2) + x(i,2)*v(i,1)) + \
				   (x(i,2)*v(i,0) + x(i,0)*v(i,2))*(x(i,2)*v(i,0) + x(i,0)*v(i,2)) + \
				   (x(i,0)*v(i,1) + x(i,1)*v(i,0))*(x(i,0)*v(i,1) + x(i,1)*v(i,0)) );
	}
	
	*E = 0.0;
	for (int i=0; i<N; i++) {
		*E += 0.5 * (M[i] / G) * (v(i,0)*v(i,0) + v(i,1)*v(i,1) + v(i,2)*v(i,2));
		for (int cheetos=0; cheetos<N; cheetos++) {
			if ( i != cheetos ) *E -= M[cheetos]*M[i]/(G*r(i,cheetos));
		}
	}
}


void hatparams::print() {
	//cout.setf(0,ios::floatfield);
	cout.precision(10);
	cout << t << " ";
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < 3; k++) cout << " " << x(i,k);
		for (int k = 0; k < 3; k++) cout << " " << v(i,k);
	}
	cout << endl;
}

	
void hatparams::printHelp(string errMssg) {
	SUCCESS = false;
	if (!errMssg.empty()) cerr << errMssg << endl;
	cerr << "Input format:" << endl;
	cerr << "    ./hattrick baseSys" << endl;	
	cerr << "    ./hattrick baseSys n m1 r1 ... mn rn"  << endl << endl;
	
	cerr << "Where:"  << endl;
	cerr << "    baseSys: Name of the file specifying the base system." << endl;
	cerr << "    n (optional): Number of perturbing bodies." << endl;
	cerr << "    mn (optional): Mass of the nth perturbing body." << endl;
	cerr << "    rn (optional): Radius of the nth perturbing body." << endl << endl;
	
	cerr << "Base system file format:" << endl;
	cerr << "    n t0 t1 h0 hmin hmax accr printSkip stepType" << endl;
	cerr << "    m1 x1 y1 z1 vx1 vy1 vz1" << endl;
	cerr << "    m2 x2 y2 z2 vx2 vy2 vz2" << endl;
	cerr << "        ..." << endl;
	cerr << "    mn xn yn zn vxn vyn vzn" << endl << endl;
	
	cerr << "Where:"  << endl;
	cerr << "    n: Number of bodies in the base system." << endl;
	cerr << "    t0: Initial system time." << endl;
	cerr << "    t1: Final system time." << endl;
	cerr << "    hmin: Minimum time step." << endl;
	cerr << "    hmax: Max time step.  Also the initial step size." << endl;
	cerr << "    accr: Accuracy paramater." << endl;
	cerr << "    skipPrint: The time skiped between prints." << endl;
	cerr << "        -1 for print orbits only." << endl;
	cerr << "    stepType: 0 for rk45, 1 for rk8pd, 2 for bsimp." << endl;
}

hatparams::~hatparams() {
	if (M == NULL) delete[] M;
	M = 0;
	if (y == NULL) delete[] y;
	y = 0;
}
