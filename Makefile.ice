GCC     = icc
CFLAGS  = -Wall -lgsl -lm
SOURCE  = hattrick.cpp gravity.cpp hatparams.cpp voodoomagic.cpp
OBJECTS = $(SOURCE: .c=.o)
VERSION = 0.7

all:
	module load intel
	module load gsl
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -o hattrick $(OBJECTS)

package:
	tar -czf hattrick-$(VERSION).tar.gz *

clean:
	rm *.o
