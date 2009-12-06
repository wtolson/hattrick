/***********************************************************************
 * hatparams.h
 *
 * Copyright 2009 William Olson <wtolson@gmail.com>
 * GNU General Public License v3
 * 
 * Header file for hatparams.cpp
 * 
 */


#ifndef HATPARAMS_H
#define HATPARAMS_H

using namespace std;

class Hatparams
{
	public:
		int N, stepType;
		double k, G;
		double *M, *y;
		double t, t0, t1, hmin, hmax, accr, printSkip;
		bool SUCCESS;
		
		Hatparams(int argc, char** argv);
		
		~Hatparams();
		
		bool orbit();
		
		bool dmbf();
		
		void print();
		
		double x (int i, int k) {
			return y[yx(i,k)];
		};
		
		double x (int i, int k, const double y[]) {
			return y[yx(i,k)];
		};
		
		double v (int i, int k) {
			return y[yv(i,k)];
		};
		
		double v (int i, int k, const double y[]) {
			return y[yv(i,k)];
		};
		
		double r (int i, int j) {
			return sqrt( (xHat(i,j,0)*xHat(i,j,0)) + \
						 (xHat(i,j,1)*xHat(i,j,1)) + \
						 (xHat(i,j,2)*xHat(i,j,2)) );
		};
		
		double r (int i, int j, const double y[]) {
			return sqrt( (xHat(i,j,0,y)*xHat(i,j,0,y)) + \
						 (xHat(i,j,1,y)*xHat(i,j,1,y)) + \
						 (xHat(i,j,2,y)*xHat(i,j,2,y)) );
		};
		
		double xHat (int i, int j, int k) {
			return (x(i,k) - x(j,k));
		};
		
		double xHat (int i, int j, int k, const double y[]) {
			return (x(i,k,y) - x(j,k,y));
		};
		
		double vHat (int i, int j, int k) {
			return (v(i,k) - v(j,k));
		};
		
		double vHat (int i, int j, int k, const double y[]) {
			return (v(i,k,y) - v(j,k,y));
		};
		
		double rDotv (int i, int j) {
			return ( (xHat(i,j,0)*vHat(i,j,0)) + \
					 (xHat(i,j,1)*vHat(i,j,1)) + \
					 (xHat(i,j,2)*vHat(i,j,2)) );
		};
		
		double rDotv (int i, int j, const double y[]) {
			return ( (xHat(i,j,0,y)*vHat(i,j,0,y)) + \
					 (xHat(i,j,1,y)*vHat(i,j,1,y)) + \
					 (xHat(i,j,2,y)*vHat(i,j,2,y)) );
		};
		
		int yx (int i, int k) {
			return (6*i + k);
		};
		
		int yv (int i, int k) {
			return (6*i + k + 3);
		};
		
		int fv (int i, int k) {
			return (6*i + k);
		};
		
		int fa (int i, int k) {
			return (6*i + k + 3);
		};
		
		void diagnostics(double * E, double * L);
									
	private:		
		void printHelp(string errMssg);
		double xLast, lastOrbit;
		double initialE, initialL, pertR;
		int orbits;
};

#endif /* HATPARAMS_H */ 
