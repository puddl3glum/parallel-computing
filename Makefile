
PROJECT = conway

CCFLAGS = -O3 -std=c18 -Weverything

all: executable

debug: CCFLAGS += -DDEBUG -gdwarf -TDEBUG
debug: executable

executable: main/main.o
	$(CC) -o $(PROJECT) main/main.o

main.o: main/main.c
	$(CC) -c main/main.c

clean:
	rm main/main.o
