#ifndef _game_h_
#define _game_h_


#include "board.h"
#include "game_info.h"
#include "cards.h"
#include <vector>

class sorry_game {
public:
    game_info info;
    cards deck;
    sorry_game();
    sorry_game(int num_console_players);
    player* play();
};

#endif