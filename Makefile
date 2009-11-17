GCC     = g++
CFLAGS  = -Wall -lgsl -lm -lgslcblas
SOURCE  = hattrick.cpp gravity.cpp hatparams.cpp
OBJECTS = $(SOURCE: .c=.o)

all:
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -o hattrick $(OBJECTS)

clean:
	rm *.o
