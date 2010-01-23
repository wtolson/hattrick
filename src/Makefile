CXX      = g++
CXXFLAGS = -Wall -O3 -lgsl -lm -lgslcblas
SOURCE   = hattrick.cc gravity.cc Hatparams.cc voodoomagic.cc
OBJECTS  = $(SOURCE:.cc=.o)
VERSION  = 1.0

all: hattrick.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o hattrick $(OBJECTS)

package:
	tar -czf hattrick-$(VERSION).tar.gz *

debug:
	$(CXX) -Wall -O0 -g -lgsl -lm -lgslcblas -c $(SOURCE)
	$(CXX) -Wall -O0 -g -lgsl -lm -lgslcblas -o hattrick $(OBJECTS)

deb:
	checkinstall -y -D --pkgname=hattrick --pkgversion=1.1 --pkgrelease=1 --pkggroup=hattrick --maintainer="William Olson \<wtolson@gmail.com\>" --requires="gsl-bin" --provides="hattrick"

install:
	mv ./hattrick /usr/bin

clean:
	rm -f *.o
