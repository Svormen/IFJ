#
# Předmět  :   IFJ / IAL
# Soubor   :   Makefile
# Projekt  :   Implementace překladače imperativního jazyka IFJ17
# Tým č    :   21
# Varianta :   1
# Autoři   : xhribe02, David Hříbek
#            xkoval14, Marek Kovalčík
#            xvalus02, Ondřej Valušek
#            xrutad00, Dominik Ruta
#

PROJ=proj
FILES=main.c scanner.c string.c symtable.c stack.c instlist.c expression.c parser.c
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $(PROJ) $(FILES) -lm

test :
	./tests/tests_runner.sh

clean :
	$(RM) *.o $(PROJ)