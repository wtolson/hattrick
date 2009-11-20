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

class hatparams
{
	public:
		int N, stepType;
		double k, G;
		double *M, *y;
		double t0, t1, h0, h1, accr, printSkip;
		bool SUCCESS;
		
		hatparams(int argc, char** argv);
		
		bool orbits() {
			return (printSkip==-1.0);
		};
		
		void print(double t);
		
		double x (int i, int k) {
			return y[9*i + 3*k];
		};
		
		double x (int i, int k, const double y[]) {
			return y[9*i + 3*k];
		};
		
		double v (int i, int k) {
			return y[9*i + 3*k + 1];
		};
		
		double v (int i, int k, const double y[]) {
			return y[9*i + 3*k + 1];
		};
		
		double r (int i, int j) {
			return sqrt( (this->xHat(i,j,0)*this->xHat(i,j,0)) +
						 (this->xHat(i,j,1)*this->xHat(i,j,1)) +
						 (this->xHat(i,j,2)*this->xHat(i,j,2)) );
		};
		
		double r (int i, int j, const double y[]) {
			return sqrt( (this->xHat(i,j,0,y)*this->xHat(i,j,0,y)) +
						 (this->xHat(i,j,1,y)*this->xHat(i,j,1,y)) +
						 (this->xHat(i,j,2,y)*this->xHat(i,j,2,y)) );
		};
		
		double xHat (int i, int j, int k) {
			return (this->x(i,k) - this->x(j,k));
		};
		
		double xHat (int i, int j, int k, const double y[]) {
			return (this->x(i,k,y) - this->x(j,k,y));
		};
		
		double vHat (int i, int j, int k) {
			return (this->v(i,k) - this->v(j,k));
		};
		
		double vHat (int i, int j, int k, const double y[]) {
			return (this->v(i,k,y) - this->v(j,k,y));
		};
		
		double rDotv (int i, int j) {
			return ( (this->xHat(i,j,0)*this->vHat(i,j,0)) +
					 (this->xHat(i,j,1)*this->vHat(i,j,1)) +
					 (this->xHat(i,j,2)*this->vHat(i,j,2)) );
		};
		
		double rDotv (int i, int j, const double y[]) {
			return ( (this->xHat(i,j,0,y)*this->vHat(i,j,0,y)) +
					 (this->xHat(i,j,1,y)*this->vHat(i,j,1,y)) +
					 (this->xHat(i,j,2,y)*this->vHat(i,j,2,y)) );
		};
		
		int vi (int i, int k) {
			return (9*i + 3*k);
		};
		int ai (int i, int k) {
			return (9*i + 3*k + 1);
		};
					
	private:		
		void printHelp();
};

#endif /* HATPARAMS_H */ 
