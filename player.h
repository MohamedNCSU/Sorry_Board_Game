#ifndef _player_h_
#define _player_h_

#include <set>
#include <string>
#include "game_location.h"
#include "pawn.h"
#include "game_location.h"

struct game_info;
class pawn;
class game_location; // forward declaring for compiler

using namespace std;

class player {

public:

    game_color color; //color of pawn
    set<pawn*> pawns;

    game_info *info;


    string name; // name of the Player

    player(string name, game_color color, game_info*);

    game_location* try_move_next(pawn* p, int n);

    game_location* try_move_prev(pawn* p, int n);
    //This function will deal with moving pawns forward/backward according to the card at play
    bool move(int card_drawn, pawn* selected_pawn, int player_choice, pawn* other_pawn );
    //This function will deal with pawns landing on slide spaces.
    void slide(pawn* p);
    //This function will deal with pawns that land on each other.
    bool pawn_collision(game_location *loc, pawn* new_pawn, bool isSliding);

    virtual void pick_action(int card) = 0;
};

class console_player: public player {
public:
    console_player(string name, game_color color, game_info*);
    void pick_action(int card);
};

class ai_player: public player {
public:
    ai_player(string Player_name,game_color color, game_info*);
    void pick_action(int card);
};

#endif