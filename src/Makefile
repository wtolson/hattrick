CXX        = g++
CXXFLAGS   = -Wall -O3 -ffast-math #Release
#CXXFLAGS   = -Wall -O0 -g #Debug
#CXXFLAGS  += -pg
CXXLIBS    = -lgsl -lgslcblas -lm
SOURCE     = hattrick.cc Event.cc EventsManager.cc Gravity.cc HatParams.cc Integrator.cc Planets.cc Universe.cc voodoomagic.cc
OBJECTS    = $(SOURCE:.cc=.o)
VERSION    = 2.1
REQUIRES   = libgsl0ldbl

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -o hattrick $(OBJECTS)

package:
	cd ..
	tar -czf hattrick-$(VERSION).tar.gz *

deb:
	checkinstall -D --pkgname=hattrick --pkgversion=$(VERSION) --pkgrelease=1 --pkggroup=hattrick --maintainer="William Olson \<wtolson@gmail.com\>" --requires="$(REQUIRES)" --provides=hattrick

install:
	cp ./hattrick /usr/bin
	cp ../doc/hattrick.1.gz /usr/share/man/man1

clean:
	rm -f *.o
