// ***********************************************************************
// HatExceptions.cc
//
// Copyright 2010 William Olson <wtolson@gmail.com>
// GNU General Public License v3
#include <iostream>
#include "HatExceptions.h"

using namespace std;
HatExceptions::HatExceptions(string message) throw () :
	itsMessage(message) {
}

HatExceptions::~HatExceptions() throw () {
}

const char* HatExceptions::what() const throw () {
	return itsMessage.c_str();
}

