GCC     = icc
CFLAGS  = -Wall -L/uaopt/gsl/1.10/include/ -lgsl -lm 
SOURCE  = hattrick.cpp gravity.cpp hatparams.cpp voodoomagic.cpp
OBJECTS = $(SOURCE: .c=.o)

all:
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -o hattrick $(OBJECTS)

clean:
	rm -f *.o
