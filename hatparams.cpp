//      hatparams.cpp
//      
//      Copyright 2009 William Olson <wto@wto-virtualbox>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <iostream>
#include <fstream>
#include <cmath>
#include "hatparams.h"

using namespace std;

hatparams::hatparams(int argc, char** argv, double * t, double * t1,
					 double * h0, double * h1, double * accr, double y[])
{
	k = 0.01720209895;
	G = k*k;
	if (argc != 4) {
		this->SUCCESS = false;
		printHelp();
	} else {
		this->SUCCESS = true;
		
		ifstream ifs ( argv[1] , ifstream::in );
		if (ifs.is_open())
		{
			ifs >> N;
			ifs >> *t;
			ifs >> *t1;
			ifs >> *h0;
			ifs >> *h1;
			ifs >> *accr;
			
			y = new double[9*N];
			M = new double[N];
			for( int i = 0; i < N; i++)
			{
				ifs >> M[i];
				M[i] *= G;
				ifs >> y[9*i];
				ifs >> y[9*i + 3];
				ifs >> y[9*i + 6];
				ifs >> y[9*i + 1];
				ifs >> y[9*i + 4];
				ifs >> y[9*i + 7];
				y[9*i + 2] = 0.0;
				y[9*i + 5] = 0.0;
				y[9*i + 8] = 0.0;
			}
			ifs.close();				
		}
	}
}
	
void hatparams::printHelp() {
	cout << "Input format:" << endl;
	cout << "    ./hattrick baseSys r m"  << endl << endl;
	
	cout << "Where:"  << endl;
	cout << "    baseSys: Name of the file specifying the base system." << endl;
	cout << "    r: Radius of the perturbing body." << endl;
	cout << "    m: Mass of the perturbing body." << endl << endl;
	
	cout << "Base system file format:" << endl;
	cout << "    n t0 t1 h0 h1 accr" << endl;
	cout << "    m1 x1 y1 z1 vx1 vy1 vz1" << endl;
	cout << "    m2 x2 y2 z2 vx2 vy2 vz2" << endl;
	cout << "        ..." << endl;
	cout << "    mn xn yn zn vxn vyn vzn" << endl << endl;
	
	cout << "Where:"  << endl;
	cout << "    n: Number of bodies in the base system." << endl;
	cout << "    t0: Initial system time." << endl;
	cout << "    t1: Final system time." << endl;
	cout << "    h0: Initial time step." << endl;
	cout << "    h1: Max time step." << endl;
	cout << "    accr: Accuracy paramater." << endl;		
}


