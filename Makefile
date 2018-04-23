CC=gcc
# warnings all and debugging
CFLAGS=-s -c -Wall -g
CURRENT_DIR := $(shell basename `pwd`)
THIS_FILE := $(lastword $(MAKEFILE_LIST))

all: final

final: main.o
	$(CC) main.o -s -o final.out

final.o:	final.c
	$(CC) $(CFLAGS) final.c

clean:
	/bin/rm -f final *.o *~

run:
	./final

tarball:
	@$(MAKE) -f $(THIS_FILE) clean
	tar -cvzf ../$(CURRENT_DIR).tgz -C.. $(CURRENT_DIR)
