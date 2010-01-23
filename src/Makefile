CXX      = g++
CXXFLAGS = -Wall -O3 -lgsl -lm -lgslcblas
SOURCE   = hattrick.cc gravity.cc Hatparams.cc voodoomagic.cc
OBJECTS  = $(SOURCE:.cc=.o)
VERSION  = 1.1
REQUIRES = libgsl0ldbl

all: hattrick.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o hattrick $(OBJECTS)

package:
	tar -czf hattrick-$(VERSION).tar.gz *

debug:
	$(CXX) -Wall -O0 -g -lgsl -lm -lgslcblas -c $(SOURCE)
	$(CXX) -Wall -O0 -g -lgsl -lm -lgslcblas -o hattrick $(OBJECTS)

deb:
	checkinstall -D --pkgname=hattrick --pkgversion=$(VERSION) --pkgrelease=1 --pkggroup=hattrick --maintainer="William Olson \<wtolson@gmail.com\>" --requires="$(REQUIRES)" --provides=hattrick

install:
	cp ./hattrick /usr/bin
	cp ../doc/hattrick.1.gz /usr/share/man/man1

clean:
	rm -f *.o
