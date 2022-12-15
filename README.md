# ece-309-final-project
ECE 309 Final Project

We created the game “Sorry!” in C++. Sorry! is a board game where
players move their four pawns around the board in order to get all
of their pawns to “home” first. The game supports 2-4 players.
During each turn of the game, the player is responsible for
picking a card. The cards range from 1-11 (excluding 6 and 9)
and include a special “Sorry!” card. These cards give instructions
to the player on how to move their pawn, as well as the
conditions needed to move a given pawn. All pawns start in a
starting reservoir, which can only be exited once the player
pulls either a 1,2, or a “Sorry!” card (given Sorry can be used
legally). Once a pawn is outside of the reservoir, it can be
moved by the remaining cards. Additionally, movement is
restricted when entering their designated home. There is no
overshooting the home area, a player must draw the perfect
amount to get their pawn directly into home. There is also a
safety area that prevents any other player from touching pawns
within it.


In order to compile the game, a C++ compiler is necessary, preferably g++. A makefile for a Linux system has been provided to easily compile and/or run the program. make all compiles the program into a file named main. make run executes the program with all AI players. In a Linux system, you can run the program with the command ./main. The program can be executed without any command line arguments to be prompted which players will be controlled by a human or AI or no one. A singular command line integer argument can be provided to specify the number of human players.

An interesting feature is the user interface shown in the
board.cpp file. There are two interfaces, one with all of the
location numbers of the board, and one that’s more of a graphical
representation of the board, including where the pawns are, where
the safeties are, etc. All of the game events are displayed as they
occur, so the viewer is up to date on the state of the game.

Our software works just as we intended it to, with the game
working with console players and AI players.

