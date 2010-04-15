// ***********************************************************************
// HatExceptions.h
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3

#ifndef HATEXCEPTIONS_H_
#define HATEXCEPTIONS_H_

#include <exception>
#include <string>
using namespace std;

class HatExceptions: public exception {
public:
	HatExceptions(string message = "") throw ();

	~HatExceptions() throw ();

	const char* what() const throw();

private:
	string itsMessage;
};

#endif /* HATEXCEPTIONS_H_ */
