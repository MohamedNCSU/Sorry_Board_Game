#ifndef _pawn_h_
#define _pawn_h_

#include "game.h"
#include "game_location.h"
#include <iostream>

class game_location;

class pawn {
public:
    int id;
    int at_play;
    game_color color;
    game_location* location;

    pawn(game_location* location, game_color color, int id);

    friend std::ostream& operator<<(std::ostream& os, const pawn& b);
};

#endif