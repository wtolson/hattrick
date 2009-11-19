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
		hatparams(int argc, char** argv);
		bool success() {return SUCCESS;};
		bool orbits() {return (printSkip==-1.0);};
		void print(double t);
		double x (int i, int j) { return y[9*i + 3*j]; };
		double v (int i, int j) { return y[9*i + 3*j + 1]; };
		double r (int i, int j) {
			return sqrt( this->xHat(i,j,1)*this->xHat(i,j,1) +
						 this->xHat(i,j,2)*this->xHat(i,j,2) +
						 this->xHat(i,j,3)*this->xHat(i,j,3) )
			;};
		double xHat (int i, int j, int k) { return (this->x(i,k) - this->x(j,k));};
		int vi (int i, int j) { return (9*i + 3*j); };
		int ai (int i, int j) { return (9*i + 3*j + 1); };
					
	private:		
		bool SUCCESS;
		void printHelp();
};

#endif /* HATPARAMS_H */ 
