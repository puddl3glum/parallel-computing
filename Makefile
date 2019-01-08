
PROJECT = conway

CCFLAGS = -std=c18 -Wall
LFLAGS  = -lSDL2
OBJECTS = main.o game.o

debug: CCFLAGS += -DDEBUG -gdwarf

debug: main.o game.o draw.o
	$(CC) -o $(PROJECT) $(CCFLAGS) $(LFLAGS) main.o game.o draw.o

release: CCFLAGS += -O3

release: main.o game.o
	$(CC) -o $(PROJECT) $(CCFLAGS) $(LFLAGS) main.o game.o

main.o: src/main.c
	$(CC) $(CCFLAGS) -c src/main.c

game.o: src/game.c
	$(CC) $(CCFLAGS) -c src/game.c

draw.o: src/draw.c
	$(CC) $(CCFLAGS) -c src/draw.c

clean:
	rm main.o
	rm game.o
	rm draw.o
