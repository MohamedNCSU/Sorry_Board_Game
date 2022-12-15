#include "game_location.h"

game_location::game_location(location_type type, game_color color, int id,game_location *next, game_location *prev, game_location *safety) {
        this->type = type;
        this->color = color;
        this->next = next;
        this->prev = prev;
        this->safety = safety;
        this->id = id;
}

