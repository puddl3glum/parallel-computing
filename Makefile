
project=conway
src= ./src/main.c ./src/main.h ./src/game.c ./src/game.h
CC = gcc

target = bin/$(project)

ompflags = -fopenmp

all : $(target)

$(target) : $(src) 
	$(CC) $(cflags) $(ompflags) -std=gnu99 -o $(target) $(dflags)  $(src)

intel : ompflags = -qopenmp -qopenmp-link static
intel : CC = icc
intel : $(target)

intel-debug : debug intel

intel-release : release intel

release : cflags = -O3
release : $(target)

debug : dflags = -DDEBUG
debug : $(target)

clean:
	rm bin/*
