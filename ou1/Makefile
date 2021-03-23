CC=gcc
CFLAGS=-Wall -g -Wextra -Werror -std=gnu11 \
-Wmissing-declarations -Wmissing-prototypes \
-Werror-implicit-function-declaration \
-Wreturn-type -Wparentheses -Wunused \
-Wold-style-definition -Wundef -Wshadow \
-Wstrict-prototypes -Wswitch-default -Wunreachable-code

all: mpasswdsort

mpasswdsort: mpasswdsort.o list.o
	$(CC) -o mpasswdsort mpasswdsort.o list.o

mpasswdsort.o: mpasswdsort.c list.h
	$(CC) -c mpasswdsort.c $(CFLAGS)

list.o: list.c list.h
	$(CC) -c list.c $(CFLAGS)
