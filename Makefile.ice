CXX     = icc
GSL_INC  = /uaopt/gsl/1.10/include/
GSL_LIB  = /uaopt/gsl/1.10/lib/
CXXFLAGS  = -Wall -O3 -wd981 -I$(GSL_INC) -L$(GSL_LIB) -lgsl -lgslcblas -lm 
SOURCE  = hattrick.cc gravity.cc hatparams.cc voodoomagic.cc
OBJECTS = $(SOURCE:.cc=.o)

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o hattrick $(OBJECTS)

clean:
	rm -f *.o
