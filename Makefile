GCC     = g++
CFLAGS  = -Wall -lgsl -lm -lgslcblas
SOURCE  = hattrick.c
OBJECTS = $(SOURCE: .c=.o)

all:
	$(GCC) $(CFLAGS) -o hattrick $(SOURCE)

clean:
	rm *.~
