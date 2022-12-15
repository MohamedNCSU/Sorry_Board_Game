#include <iostream>
#include <string>
#include "board.h"
#include "cards.h"

int main(int argc, char *argv[]) {
    sorry_game *game;
    if (argc == 1) {
        game = new sorry_game();
    } else if (argc == 2) {
        int num = atoi(argv[0]);
        game = new sorry_game(num);
    }
    game->play();
}