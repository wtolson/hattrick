GCC     = gcc
CFLAGS  = -Wall -lgsl -lm -lgslcblas
SOURCE  = hattrick.c

all:
	$(GCC) $(CFLAGS) -o hattrick $(SOURCE)
