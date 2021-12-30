CC=gcc
CFLAGS=-Wall -g -std=c99
LDLIBS=-lm
CPPFLAGS=
LDFLAGS=

# cible principale (par défaut)
all: help build

# règle spécifique pour générer l'exécutable
build:
	cd src && make

help:
	@echo 'On fait un make sur le projet dans son intégralité'

.PHONY: clean
clean:
	rm ./src/*.o
