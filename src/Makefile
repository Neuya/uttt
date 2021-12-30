CC=gcc                      
CFLAGS=-Wall -g -std=c99 -I../include
LDLIBS=-lm
CPPFLAGS=
LDFLAGS=

all: help utictactoe clean

utictactoe: utictactoe.o model.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

help:
	@echo 'On compile les sources du projet'

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ toto
