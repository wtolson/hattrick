//      hatparams.h
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
		bool orbits() {return (printSkip==-1.0);};
		void print(double t);
		double x (int i, int k) { return y[9*i + 3*k]; };
		double x (int i, int k, const double y[]) { return y[9*i + 3*k]; };
		double v (int i, int k) { return y[9*i + 3*k + 1]; };
		double v (int i, int k, const double y[]) { return y[9*i + 3*k + 1]; };
		double r (int i, int j) {
			return sqrt( this->xHat(i,j,0)*this->xHat(i,j,0) +
						 this->xHat(i,j,1)*this->xHat(i,j,1) +
						 this->xHat(i,j,2)*this->xHat(i,j,2) )
			;};
		double r (int i, int j, const double y[]) {
			return sqrt( this->xHat(i,j,0,y)*this->xHat(i,j,0,y) +
						 this->xHat(i,j,1,y)*this->xHat(i,j,1,y) +
						 this->xHat(i,j,2,y)*this->xHat(i,j,2,y) )
			;};
		double xHat (int i, int j, int k) { return (this->x(i,k) - this->x(j,k));};
		double xHat (int i, int j, int k, const double y[]) { return (this->x(i,k,y) - this->x(j,k,y));};
		double vHat (int i, int j, int k) { return (this->v(i,k) - this->v(j,k));};
		double vHat (int i, int j, int k, const double y[]) { return (this->v(i,k,y) - this->v(j,k,y));};
		double rDotv (int i, int j) {
			return ( this->xHat(i,j,0)*this->vHat(i,j,0) +
					 this->xHat(i,j,1)*this->vHat(i,j,1) +
					 this->xHat(i,j,2)*this->vHat(i,j,2) )
			;};
		double rDotv (int i, int j, const double y[]) {
			return ( this->xHat(i,j,0,y)*this->vHat(i,j,0,y) +
					 this->xHat(i,j,1,y)*this->vHat(i,j,1,y) +
					 this->xHat(i,j,2,y)*this->vHat(i,j,2,y) )
			;};
		int vi (int i, int k) { return (9*i + 3*k); };
		int ai (int i, int k) { return (9*i + 3*k + 1); };
					
	private:		
		void printHelp();
};

#endif /* HATPARAMS_H */ 
