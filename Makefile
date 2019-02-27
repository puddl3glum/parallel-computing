
project=conway
src= ./src/main.c ./src/main.h ./src/game.c ./src/game.h
CC = gcc

target = bin/$(project)

all : $(target)

$(target) : $(src) 
	$(CC) -O3 -fopenmp -std=gnu99 -o $(target) $(dflags)  $(src)

debug : dflags = -DDEBUG
debug : $(target)

clean:
	rm bin/*
