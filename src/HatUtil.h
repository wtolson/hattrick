// ***********************************************************************
// Planet.cpp
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
//
// Utilities for working with the y array.


#ifdef GRAVITY

#define N p->N()
#define M(i) p->M(i)

#define dXdT(i,k) dydt[6 * i + k]
#define dVdT(i,k) dydt[6 * i + k + 3]

#define dVdX(i,k,j,l) dfdy[((6 * i + k) * (6*N)) + (6 * j + l)]
#define dVdV(i,k,j,l) dfdy[((6 * i + k) * (6*N)) + (6 * j + l + 3)]
#define dAdX(i,k,j,l) dfdy[((6 * i + k + 3) * (6*N)) + (6 * j + l)]
#define dAdV(i,k,j,l) dfdy[((6 * i + k + 3) * (6*N)) + (6 * j + l + 3)]

#define A(i,k) dfdt[6 * i + k]
#define J(i,k) dfdt[6 * i + k + 3]

#endif

#define X(i,k) y[6 * i + k]
#define V(i,k) y[6 * i + k + 3]

#define R(i,j) {X(i, 0) - X(j, 0), X(i, 1) - X(j, 1), X(i, 2) - X(j, 2)}
#define VREL(i,j) {V(i, 0) - V(j, 0), V(i, 1) - V(j, 1), V(i, 2) - V(j, 2)}
#define DOT(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])
#define CROSS(a,b) {a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]}
