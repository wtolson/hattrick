// ***********************************************************************
// hattrick.cpp
// Version 1.0
//
// Copyright 2010 William Olson <wtolson@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.


// C++ Libraries
// Nothing to see here.
//#include <iostream>
//using namespace std;

// My Libraries
#include "HatParams.h"
#include "Universe.h"
#include "voodoomagic.h"

int main(int argc, char** argv) {

	HatParams params(argc, argv);  // Read in parameters.

	Universe *theUniverse;

	if (params.IsAwesome()) {
		theUniverse = new Universe(params);
	} else {
		return 1;  // Exit if things suck.
	}

	Voodoo::sacrificeChicken();
	theUniverse->BigBang();

	delete theUniverse;
	return 0;
}
