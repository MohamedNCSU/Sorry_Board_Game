#include "pawn.h"

pawn::pawn(game_location* location, game_color color, int id) {//constructor
    this->location = location;
    this->color = color;
    this->id = id;
    this->at_play = 0;
}

std::ostream& operator<<(std::ostream& os, const pawn& b) {
    os << "Color - ";
    switch (b.color) {
        case RED:
        os << "Red";
        break;
        case BLUE:
        os << "Blue";
        break;
        case YELLOW:
        os << "Yellow";
        break;
        case GREEN:
        os << "Green";
        break;
    }
    os << " | Location # - " << b.location->id << " | Location Type - ";
    switch (b.location->type) {
        case game_location::START:
        os << "Start";
        break;
        case game_location::FINISH:
        os << "Home";
        break;
        case game_location::SAFETY:
        os << "Safety";
        break;
        case game_location::SAFETY_ENTRANCE:
        os << "Safety Entrance";
        break;
        case game_location::SLIDE_START:
        os << "Start of Slide";
        break;
        case game_location::SLIDE_END:
        os << "End of Slide";
        break;
        case game_location::SLIDE:
        os << "Slide";
        break;
        case game_location::NONE:
        os << "Normal";
        break;
    }
    return os;
}