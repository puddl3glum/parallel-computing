
project=conway
# CC = gcc
CC = mpicc

src = main.c game.c
srcs = $(addprefix src/, $(src))

target = bin/$(project)

cflags = -Wall

all : $(target)

$(target) : $(srcs) include/*
	$(CC) -Iinclude $(cflags) -std=gnu11 -o $(target) $(dflags)  $(srcs)

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
