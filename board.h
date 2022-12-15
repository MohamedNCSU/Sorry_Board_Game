#ifndef _board_h_
#define _board_h_

#include "game.h"
#include "pawn.h"
#include "game_location.h"
#include "game_info.h"
#include <iostream>
#include <vector>

class game_location;
 
class game_board {
    public:
    game_location *start[4];

    game_board();

    void create_board();

    void print(game_info *info = nullptr);
};

#endif