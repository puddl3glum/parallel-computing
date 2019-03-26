
project=conway
CC = gcc

src = main.c game.c
srcs = $(addprefix src/, $(src))

target = bin/$(project)

cflags = -Wall
ompflags = -fopenmp

all : $(target)

$(target) : $(srcs) include/*
	$(CC) -Iinclude $(cflags) $(ompflags) -std=gnu99 -o $(target) $(dflags)  $(srcs)

intel : ompflags = -qopenmp -qopenmp-link static
intel : CC = icc
intel : $(target)

intel-debug : debug intel

intel-release : release intel

release : cflags = -O2
release : $(target)

debug : dflags = -DDEBUG -g
debug : $(target)

clean:
	rm bin/*
