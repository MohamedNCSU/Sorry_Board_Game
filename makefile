objects = game.o board.o game_location.o pawn.o cards.o main.o player.o
CC = g++
CPPFLAGS = -Wall -g -O0

all : $(objects)
	$(CC) $(CPPFLAGS) -o main $(objects)

run : $(objects)
	./main 0

main.o : main.cpp board.h cards.h
board.o : board.cpp board.h
pawn.o : pawn.cpp pawn.h
game_location.o : game_location.cpp game_location.h game.h
game.o : game.cpp game.h player.h
cards.o : cards.cpp cards.h
player.o: player.cpp player.h
clean :
	rm -f $(objects) main