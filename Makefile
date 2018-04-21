CC=gcc
# warnings all and debugging
CFLAGS=-c -Wall -g
CURRENT_DIR := $(shell basename `pwd`)
THIS_FILE := $(lastword $(MAKEFILE_LIST))

all: hw5

hw5: mem.o main.o
	$(CC) mem.o main.o -o hw5

hw5.o:	hw5.c mem.h mem.c
	$(CC) $(CFLAGS) hw5.c

clean:
	/bin/rm -f hw5 *.o *~

run:
	./hw5

tarball:
	@$(MAKE) -f $(THIS_FILE) clean
	tar -cvzf ../$(CURRENT_DIR).tgz -C.. $(CURRENT_DIR)
