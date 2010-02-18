// ***********************************************************************
// gravity.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Header file for gravity.cpp


#ifndef GRAVITY_H
#define GRAVITY_H

class Gravity {
public:
	//Constants
	static const double k;
	static const double G;

	//Functions
	static int Acceleration(double t, const double y[], double dydt[],
			void *params);
	static int Jerk(double t, const double y[], double *dfdy, double dfdt[],
			void *params);
	static int NullJac(double t, const double *y, double *dfdy, double *dfdt,
			void *params);

private:
	Gravity();
	static double Delta(int a, int b);

};

#endif /* GRAVITY_H */
