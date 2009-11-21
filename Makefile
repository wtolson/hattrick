GCC     = g++
CFLAGS  = -Wall -O3 -lgsl -lm -lgslcblas
SOURCE  = hattrick.cpp gravity.cpp hatparams.cpp voodoomagic.cpp
OBJECTS = $(SOURCE: .c=.o)
VERSION = 0.8

all:
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -o hattrick $(OBJECTS)

package:
	tar -czf hattrick-$(VERSION).tar.gz *

debug:
	$(GCC) -Wall -O0 -g -lgsl -lm -lgslcblas -c $(SOURCE)
	$(GCC) -Wall -O0 -g -lgsl -lm -lgslcblas -o hattrick $(OBJECTS)

clean:
	rm -f *.o
