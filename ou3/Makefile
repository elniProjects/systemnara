PROG    =	mish
MISH    =	mish.o
PARSER  =	parser.o
EXECUTE =	execute.o
SIGHANT =	sighant.o
CC      =	gcc
CFLAGS  =	-g -Wall -Wextra -Werror -Wmissing-declarations \
		-Wmissing-prototypes -Werror -Wimplicit-function-declaration \
		-Wreturn-type -Wparentheses -Wunused -Wold-style-definition \
		-Wundef -Wshadow -Wstrict-prototypes -Wswitch-default \
		-Wunreachable-code


all : $(PROG)

$(PROG) : $(MISH) $(PARSER) $(EXECUTE) $(SIGHANT)
	$(CC) $(CFLAGS) -o $(PROG) $(MISH) $(PARSER) $(EXECUTE) $(SIGHANT)

$(MISH) : mish.h

$(PARSER) : parser.h

$(EXECUTE) : execute.h

$(SIGHANT) : sighant.h
