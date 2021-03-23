CC=gcc

OUT_MAIN=   mfind
DLIST   =   dlist.o



CFLAGS = -std=c11 -Wall -Wextra -Wmissing-declarations\
	-Wmissing-prototypes -Werror-implicit-function-declaration\
	-Wreturn-type -Wparentheses -Wunused -Wold-style-definition\
	-Wundef -Wshadow -Wstrict-prototypes -Wswitch-default\
	-Wstrict-prototypes -Wunreachable-code -pthread


all: mfind

mfind: mfind.o dlist.o
	$(CC) $(CFLAGS) -o mfind mfind.o dlist.o


mfind.o: mfind.c mfind.h dlist.h
	$(CC) $(CFLAGS) -c mfind.c mfind.h -g

dlist.o: dlist.c dlist.h
	$(CC) $(CFLAGS) -c dlist.c dlist.h

clean:
	rm -f *.o *.o core
