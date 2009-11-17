/*
 * The System of equations describing the gravity between the particles. 
 */

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_errno.h>
#include <math.h>
#include "hatparams.h"

int
func (double t, const double y[], double f[],
	  void *params)
{
	hatparams hp = *(hatparams *) params;
	double * M = hp.M;
	int N = *hp.N;
	int i,j;
	for(i=0; i<9*N; i++)
	{
		if ((i%9%3) == 0)
		{
			f[i] = y[i+1];
		} else if ((i%9%3) == 1) {
			f[i] = 0;
			for(j=(i%9); j<9*N; j+=9){
				if (i/9!=j/9)
				{
					double x[3];
					x[0] = y[9*(j/9)] - y[9*(i/9)];
					x[1] = y[9*(j/9)+3] - y[9*(i/9)+3];
					x[2] = y[9*(j/9)+6] - y[9*(i/9)+6];
					double r = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);

					f[i] += M[j/9] * (y[j-1]-y[i-1]) / (r*r*r);
				}
			}
		} else {
			f[i] = 0.0;
		}
	}
	return GSL_SUCCESS;
}

int
jac (double t, const double y[], double *dfdy,
     double dfdt[], void *params)
{
	int N = 2;
	double * M = (double *) params;
	gsl_matrix_view dfdy_mat
		= gsl_matrix_view_array (dfdy, 9*N, 9*N);
	gsl_matrix * m = &dfdy_mat.matrix;


	int i,j,k;
	for (i=0; i<9*N; i++)
	{
		for (j=0; j<9*N; j++)
		{
			if ((i%9%2) == 0)
			{
				if (i+1 == j)
				{
					gsl_matrix_set (m, i, j, 1.0);
				} else {
					gsl_matrix_set (m, i, j, 0.0);
				}
			} else {
				if ((j%9%2) == 1) {
					gsl_matrix_set (m, i, j, 0.0);
				} else if (i == j+1){
					double ftemp = 0;
					for(k=(i%9); k<9*N; k+=9)
					{
						if (i/9!=k/9)
						{
							double x[3];
							x[0] = y[9*(k/9)] - y[9*(i/9)];
							x[1] = y[9*(k/9)+3] - y[9*(i/9)+3];
							x[2] = y[9*(k/9)+6] - y[9*(i/9)+6];
							double r = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);

							ftemp -= 3 * M[j/9] * (y[k-1]-y[i-1]) * (y[k-1]-y[i-1]) / (r*r*r*r*r);
							ftemp += M[j/9] * y[i-1] / (r*r*r);
						}
					}
					gsl_matrix_set (m, i, j, ftemp);
				} else if (i/9 == j/9) {
					double ftemp = 0;
					for(k=(i%9); k<9*N; k+=9)
					{
						if (i/9!=k/9)
						{
							double x[3];
							x[0] = y[9*(k/9)] - y[9*(i/9)];
							x[1] = y[9*(k/9)+3] - y[9*(i/9)+3];
							x[2] = y[9*(k/9)+6] - y[9*(i/9)+6];
							double r = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
							ftemp -= 3 * M[j/9] * (y[k-1]-y[i-1]) * (y[9*(i/9) + j%9]-y[j]) / (r*r*r*r*r);
						}
					}
					gsl_matrix_set (m, i, j, ftemp);
				} else if (i%9 == (j+1)%9) {
						double x[3];
						x[0] = y[9*(j/9)] - y[9*(i/9)];
						x[1] = y[9*(j/9)+3] - y[9*(i/9)+3];
						x[2] = y[9*(j/9)+6] - y[9*(i/9)+6];
						double r = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
						gsl_matrix_set (m, i, j, (M[j/9]/(r*r*r)) * (3*(y[j]-y[i-1])*(y[j]-y[i-1])/(r*r) - y[j]));
				} else {
					double x[3];
					x[0] = y[9*(j/9)] - y[9*(i/9)];
					x[1] = y[9*(j/9)+3] - y[9*(i/9)+3];
					x[2] = y[9*(j/9)+6] - y[9*(i/9)+6];
					double r = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
					gsl_matrix_set (m, i, j, (M[j/9]/(r*r*r)) * (3*(y[9*(j/9) + (i-1)%9]-y[i-1])*(y[j]-y[9*(i/9) + j%9])/(r*r)));
				}
			}
		}
	}

	for (i=0; i<9*N; i++)
	{
		dfdt[i] = 0.0;
	}

	return GSL_SUCCESS;
}
