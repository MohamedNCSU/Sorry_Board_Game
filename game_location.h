#ifndef _game_location_h_
#define _game_location_h_

#include "game_color.h"
#include "pawn.h"

class pawn; // forward declared for compiler

class game_location {
public:

    enum location_type {
        START, FINISH, SAFETY, SAFETY_ENTRANCE, SLIDE_START, SLIDE_END, SLIDE, NONE
    };

    location_type type;
    game_color color;

    game_location *next;
    game_location *prev;
    game_location *safety;

    int id;

    pawn *curr_pawn;

    game_location(location_type type, game_color color, int id,game_location *next = nullptr, game_location *prev = nullptr, game_location *safety = nullptr); 
};

#endif