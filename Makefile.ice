GCC     = icc
INCGSL  = /uaopt/gsl/1.10/include/
LIBGSL  = /uaopt/gsl/1.10/lib/
CFLAGS  = -Wall -O3 -wd981 -static -I$(INCGSL) -L$(LIBGSL) -lgsl -lgslcblas -lm 
SOURCE  = hattrick.cpp gravity.cpp hatparams.cpp voodoomagic.cpp
OBJECTS = $(SOURCE: .c=.o)

all:
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -o hattrick $(OBJECTS)

clean:
	rm -f *.o
