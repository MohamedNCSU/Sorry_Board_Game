#ifndef _game_info_h_
#define _game_info_h_

#include <vector>
#include "board.h"
#include "player.h"

class game_board;
class player; // forward declaring for compiler

struct game_info {
    game_board *board;
    std::vector<player*> players;
};

#endif