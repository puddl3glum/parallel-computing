
cudadir = /usr/local/cuda

# CUDACC = nvcc
# CC = $(CUDACC) -ccbin gcc -Xcompiler "-std=c99"
CC = nvcc
# CC = gcc

# cflags = --std=gnu11

# lflags = -lsfml-graphics -lsfml-window -lsfml-system
# lflags = -I$(cudadir)/include -L$(cudadir)/lib64 -L$(cudadir)/lib64/stubs -lcudart -lcuda

files = main game
srcs = $(addprefix src/, $(addsuffix .cu, $(files)))

# cudafiles = board
# cudasrcs = $(addprefix src/, $(addsuffix .cu, $(cudafiles)))

# objdir = objects

# cudaobjects = $(addprefix objects/, cuda.o)

project = conway

all :
	$(CC) $(cflags) $(lflags) -o bin/$(project) $(srcs)

$(cudaobjects) : | $(objdir)
$(cudaobjects) : $(cudasrcs)
	$(CC) -c -o $(objdir)/cuda.o $(cudasrcs)

$(objdir):
	mkdir -p $(objdir)

debug : cflags += -g -DDEBUG
debug : all

release : cflags += -O2
release : all

