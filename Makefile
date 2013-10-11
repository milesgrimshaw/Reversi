CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

#####
# Instructions to make Reversi
#####

Reversi: Reversi.o getLine.o Tree.o
	${CC} ${CFLAGS} -o $@ $^
