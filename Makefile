
CC = gcc
CUDACC = /opt/cuda/bin/nvcc

# lflags = -lsfml-graphics -lsfml-window -lsfml-system
lflags = -I/opt/cuda/include -L/opt/cuda/lib -lcuda -l cudart

files = main game
srcs = $(addprefix src/, $(addsuffix .c, $(files)))

cudafiles = board
cudasrcs = $(addprefix src/, $(addsuffix .cu, $(cudafiles)))

objdir = objects

cudaobjects = $(addprefix objects/, cuda.o)

project = conway

all : $(cudaobjects)
	$(CC) $(cflags) $(lflags) -o bin/$(project) $(srcs) $(cudaobjects)

$(cudaobjects) : | $(objdir)
$(cudaobjects) : $(cudasrcs)
	$(CUDACC) -c -o $(objdir)/cuda.o $(cudasrcs)

$(objdir):
	mkdir -p $(objdir)

debug : cflags += -g -DDEBUG
debug : all

release : cflags += -O2
release : all

