

int func (double t, const double y[], double f[], void *params);

int functwo (double t, const double y[], double f[], void *params);

int jacnot (double t, const double y[], double *dfdy, double dfdt[],
		 void *params);

int jac (double t, const double y[], double *dfdy, double dfdt[],
		 void *params);
