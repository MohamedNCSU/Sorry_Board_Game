#include "player.h"
#include "cards.h"
#include "pawn.h"

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

//This class, Player, is a base class for the two inheritted classes below: console and ai_player.
// It contains several functions to deal with pawn movement as well as a virtual function to handle
// inputs from both ai and real players.
player::player(string name, game_color color, game_info *info) : name(name), color(color), info(info) {
    game_location* start = info->board->start[color];
    for (int i = 0; i < 4; i++) {
        pawns.insert(new pawn(start, color, i));
    }
}
//Console player constructor (inherited from player)
console_player::console_player(string name, game_color color, game_info *info) : player(name, color, info) {}
//ai player constructor (inherited from player)
ai_player::ai_player(string name, game_color color, game_info *info) : player(name, color, info) {}


//This function deals with pawn movement in the forward direction
game_location* player::try_move_next(pawn* p, int n) {
    game_location* curr_loc = p->location;
    for (int i = 0; i < n; ++i) {
        if (curr_loc->next == nullptr) {
            return nullptr;
        }
        if (curr_loc->color == p->color && curr_loc->type == game_location::SAFETY_ENTRANCE) curr_loc = curr_loc->safety;
        else curr_loc = curr_loc->next;
    }

    return curr_loc;
}

//This function deals with pawn movement in the backward direction
game_location* player::try_move_prev(pawn* p, int n) {
    game_location* curr_loc = p->location;
    for (int i = 0; i < n; ++i) {
        if (curr_loc->prev == nullptr) {
            return nullptr;
        }
        curr_loc = curr_loc->prev;
    }
    return curr_loc;
}

//This function handles instances where pawns land on a location that is currently being occupied by a pawn
bool player::pawn_collision(game_location *loc, pawn* new_pawn, bool isSliding) {
    pawn* other = loc->curr_pawn;
    if (other) {
        if (other->color == new_pawn->color && !isSliding) return false;
        other->location = info->board->start[other->color];
    }
    new_pawn->location->curr_pawn = nullptr;
    new_pawn->location = loc;
    if (loc->type != game_location::FINISH) loc->curr_pawn = new_pawn;
    return true;
}

//This function handles cases where pawns land on the space that is associated with the beginning of a slide
void player::slide(pawn* p) {
    if (p->location->type != game_location::SLIDE_START || p->color == p->location->color) return;

    game_location *curr_loc = p->location;
    while (curr_loc->type != game_location::SLIDE_END) {
            pawn_collision(curr_loc, p, true);
            curr_loc = curr_loc->next;
        }
    pawn_collision(curr_loc, p, true);
}

//This function deals with the movement algorithm for ALL cards. Player_choice will serve different purposes
//depending on the card at play
bool player::move(int card_drawn, pawn* selected_pawn, int player_choice = -1, pawn* other_pawn = nullptr) {
        game_location *curr_loc = selected_pawn->location, *new_loc;
        switch (card_drawn) {
        case 0:         //This card represents the sorry card being picked
            if (curr_loc->type != game_location::START) return false;
            new_loc = other_pawn->location;
            if (new_loc->type == game_location::START || new_loc->type == game_location::SAFETY) return false;
            other_pawn->location = info->board->start[other_pawn->color];
            selected_pawn->location = new_loc;
            break;
        case 1:
            new_loc = try_move_next(selected_pawn, 1);
            if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            break;
        case 2:
            if (curr_loc->type == game_location::START) {
                new_loc = try_move_next(selected_pawn, 1);
                if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            } else {
                new_loc = try_move_next(selected_pawn, 2);
                if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            }
            break;
        case 3:
            if (curr_loc->type == game_location::START) return false;
            new_loc = try_move_next(selected_pawn, 3);
            if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            break;
        case 4:
            if (curr_loc->type == game_location::START) return false;
            new_loc = try_move_prev(selected_pawn, 4);
            if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            break;
        case 5:
            if (curr_loc->type == game_location::START) return false;
            new_loc = try_move_next(selected_pawn, 5);
            if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            break;
        case 7:
            if (curr_loc->type == game_location::START) return false;
            if (player_choice == 7) {
                new_loc = try_move_next(selected_pawn, 7);
                if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            } else {
                new_loc = try_move_next(selected_pawn, player_choice);
                game_location* new_loc2 = try_move_next(other_pawn, 7 - player_choice);
                if (!new_loc || !new_loc2 || !pawn_collision(new_loc, selected_pawn, false) || !pawn_collision(new_loc2, other_pawn, false)) return false;
            }
            break;
        case 8:
            if (curr_loc->type == game_location::START) return false;
            new_loc = try_move_next(selected_pawn, 8);
            if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            break;
        case 10:
            if (curr_loc->type == game_location::START) return false;
            if (player_choice) {
                new_loc = try_move_prev(selected_pawn, 1);
                if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            } else {
                new_loc = try_move_next(selected_pawn, 10);
                if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            }
            break;
        case 11:
            if (curr_loc->type == game_location::START) return false;
            if (player_choice) {
                new_loc = other_pawn->location;
                if (new_loc->type == game_location::SAFETY || new_loc->type == game_location::START) return false;
                selected_pawn->location = new_loc;
                new_loc->curr_pawn = selected_pawn;
                other_pawn->location = curr_loc;
                curr_loc->curr_pawn = other_pawn;
                slide(other_pawn);
            } else {
                new_loc = try_move_next(selected_pawn, 11);
                if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            }
            break;
        case 12:
            if (curr_loc->type == game_location::START) return false;
            new_loc = try_move_next(selected_pawn, 12);
            if (!new_loc || !pawn_collision(new_loc, selected_pawn, false)) return false;
            break;
    }

    slide(selected_pawn);

    // remove pawn if reached home
    if (selected_pawn->location->type == game_location::FINISH) {
        pawns.erase(selected_pawn);
    }

    return true;
}
//this function deals with printing pawns
void print_pawns(vector<pawn*> pawns) {
    for (int i = 0; i < pawns.size(); i++) {
        cout << i << ". " << *pawns[i] << '\n';
    }
}
//This function is what will handle all front end interactions. For example, assuring the user selected
//a legal pawn to move, making sure they can't make certain selections if its invalid, and other checks
//to make certain that there will be no illegal move. If a legal move turns out to be invalidated, that
//check will not be included here but in the move function above.
void console_player::pick_action(int card) {
    int input;
    int input2;
    int input3;
    pawn* start_pawn = nullptr;
    for (pawn* p : pawns) {
        if (p->location->type == game_location::START) {
            start_pawn = p;
            break;
        }
    }

    vector<pawn*> swap_pawns;
    for (player* pl : info->players) {
        if (pl == this) continue;
        for (pawn* p : pl->pawns) {
            game_location *loc = p->location;
            if (loc->type != game_location::START && loc->type != game_location::SAFETY) 
                swap_pawns.push_back(p);
        }
    }

    vector<pawn*> play_pawns;
    for (pawn* p : pawns) {
        if (p->location->type != game_location::START) play_pawns.push_back(p);
    }

    if (card == 0) {
        if (start_pawn == nullptr || swap_pawns.size() == 0) {
            cout << "No valid moves, must forfeit move\n";
        } else {
            while (true) {
                cout << "Which opponent pawn would you like to swap?, or -1 to give up\n";
                // print all pawn locations in list with numbers for each
                print_pawns(swap_pawns);
                cin >> input;
                if (input == -1) break;
                if (input < 0 || input >= (int) swap_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                pawn* swap_pawn = swap_pawns[input];
                if (move(card, start_pawn, 0, swap_pawn)) break;
                else cout << "Invalid move\n";
            }
            
        }
    } else if (card == 1 || card == 2) {
        while (true) {
            cout << "Press 0 to move a pawn at start, 1 to move a pawn already at play, or -1 to give up\n";
            cin >> input;

            if (input == 0) {
                if (start_pawn == nullptr) {
                    cout << "No pawns at start\n";
                    continue;
                }
                if (move(card, start_pawn)) break;
                else cout << "Invalid move\n";
            } else if (input == 1) {
                if (play_pawns.size() == 0) {
                    cout << "No pawns at play\n";
                    continue;
                }
                cout << "Please pick the pawn to move\n";
                print_pawns(play_pawns);
                cin >> input;
                if (input < 0 || input >= (int) play_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                if (move(card, play_pawns[input])) break;
                else cout << "Invalid move\n";
            } else if (input == -1) break;
        }
    } else if (card == 3 || card == 4 || card == 5 || card == 8 || card == 12) {
        if (play_pawns.size() == 0) {
            cout << "No pawns at play\n";
            return;
        }

        while (true) {
            cout << "Which pawn would you like to move? -1 to give up\n";
            print_pawns(play_pawns);
            cin >> input;
            if (input == -1) break;
            if (input < 0 || input >= (int) play_pawns.size()) {
                cout << "Invalid input, pick a valid pawn\n";
                continue;
            }
            if (move(card, play_pawns[input])) break;
            else cout << "Invalid move\n";
        }
    } else if (card == 7) {
        if (play_pawns.size() == 0) {
            cout << "No pawns at play\n";
            return;
        }
        while(true){
            cout << "Would you like to move one pawn forward 7 spaces or split the move between two pawns? 1 to move one pawn, 2 to split, -1 to give up\n";
            cin >> input;
            if(input == -1)break;
            else if(input == 1){
                cout << "Which pawn would you like to move?\n";
                print_pawns(play_pawns);
                cin >> input;
                if (input < 0 || input >= (int) play_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                if (move(card, play_pawns[input], 7)) break;
                else cout << "Invalid move\n"; 
            } else if(input == 2){
                cout << "What is the first pawn you would like to move?\n";
                print_pawns(play_pawns);
                cin >> input;
                if (input < 0 || input >= (int) play_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                cout << "How many spaces would you like to move the first pawn? Must pick value less than 7\n";
                cin >> input2;
                if(input2 < 0 || input2 > 6){
                    cout << "Invalid input, pick a valid move\n";
                    continue; 
                }
                cout << "Which pawn should receive your remaining spaces?\n";
                print_pawns(play_pawns);
                cin >> input3;
                if ((input3 < 0) || (input3 >= (int) play_pawns.size()) || (input3 == input)) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                if(move(card,play_pawns[input],input2,play_pawns[input3])) break;
                else cout << "Invalid move\n";
            }
        }
    } else if (card == 10) {
        if (play_pawns.size() == 0) {
            cout << "No pawns at play\n";
            return;
        }
        while(true){
            cout << "Would you like to move one pawn forward 10 spaces or move one pawn back a space? 0 for 10 forward, 1 for 1 backward, -1 to give up\n";
            cin >> input2;
            if(input2 == -1) break;
            else if(input2 == 0 || input2 == 1){
                cout << "Which pawn would you like to move?\n";
                print_pawns(play_pawns);
                cin >> input;
                if (input < 0 || input >= (int) play_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                if (move(card, play_pawns[input],input2)) break;
                else cout << "Invalid move\n";
            }
        }
    } else if (card == 11) {
        if (play_pawns.size() == 0) {
            cout << "No pawns at play\n";
            return;
        }
        while(true){
            cout << "Would you like to move a pawn forward 11 spaces or swap with an opponent pawn? 0 to move 11, 1 to swap, -1 to give up\n";
            cin >> input2;
            if(input2 == -1) break;
            else if(input2 == 0){
                cout << "Which pawn would you like to move?\n";
                print_pawns(play_pawns);
                cin >> input;
                if (input < 0 || input >= (int) play_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                if (move(card, play_pawns[input],input2)) break;
                else cout << "Invalid move\n";
            } else if(input2 == 1){
                if (swap_pawns.size() == 0) {
                    cout << "No opponent to swap with\n";
                    continue;
                }
                cout << "Which of your pawns would you like to swap?\n";
                print_pawns(play_pawns);
                cin >> input;
                if (input < 0 || input >= (int) play_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                cout << "Which of the opponent pawns would you like to swap?\n";
                print_pawns(swap_pawns);
                cin >> input3;
                if (input3 < 0 || input3 >= (int) swap_pawns.size()) {
                    cout << "Invalid input, pick a valid pawn\n";
                    continue;
                }
                if (move(card, play_pawns[input],input2,swap_pawns[input3])) break;
                else cout << "Invalid move\n";
            }
        }
    }
}

//This function performs the same purpose as pick action for console player, however, all of the actions are
//autonomous since there is no need for human input. The actions that the computer take, however, are still
//being displayed on the screen
void ai_player::pick_action(int card) {

    std::srand(std::time(nullptr));

    int rand_pawn = rand() % 4;
    int rand_num_7 = rand() % 7;

    int move_complete = 0;
    int pawn_start_loc;
    //vector<int> slide_loc;
    int safety_start;
    int safety_end;
    int start;
    vector<pawn*> swap_pawns;
    int all_in_start = 1;
    int all_in_play = 1;
    vector <pawn*> playable_pawns;
    pawn *closest;
    int no_swap = 1;
    vector <int> color_swap_order;
    pawn * pawn_for_sorry;

    // 44,59,66,81,0,15,22,37

    if (color == game_color::BLUE) {
        //slide_loc = {44,59,66,81,0,15};
        safety_start = 24;
        safety_end = 29;
        start = 32;
        color_swap_order = {0,3,2};
    }
    if (color == game_color::GREEN) {
        //slide_loc = {44,59,0,15,22,37};
        safety_start = 68;
        safety_end = 73;
        start = 76;
        color_swap_order = {2,1,0};
    }
    if (color == game_color::RED) {
        //slide_loc = {44,59,66,81,22,37};
        safety_start = 2;
        safety_end = 7;
        start = 10;
        color_swap_order = {3,2,1};
    }
    if (color == game_color::YELLOW) {
        //slide_loc = {66,81,0,15,22,37};
        safety_start = 46;
        safety_end = 51;
        start = 54;
        color_swap_order = {1,0,3};
    }
    int index = rand_pawn;
    game_color color_swap;

    pawn* selected_pawn = *pawns.begin();
    pawn* start_pawn = nullptr;
    for (pawn* p : pawns) {
        if (p->location->type != game_location::START) {
            int curr_dist = safety_end - selected_pawn->location->id;
            if (curr_dist < 0) curr_dist += 87;

            int new_dist = safety_end - p->location->id;
            if (new_dist < 0) new_dist += 87;

            if (new_dist < curr_dist) {
                selected_pawn = p;
            }
        } else {
            start_pawn = p;
        }
    }

    pawn* closest_swappable = nullptr;
    for (player* pl : info->players) {
        if (pl == this) continue;
        for (pawn* p : pl->pawns) {
            if (p->location->type == game_location::SAFETY || p->location->type == game_location::START) continue;

            if (!closest_swappable) {
                closest_swappable = p;
                continue;
            }

            int curr_dist = safety_end - closest_swappable->location->id;
            if (curr_dist < 0) curr_dist += 87;

            int new_dist = safety_end - p->location->id;
            if (new_dist < 0) new_dist += 87;

            if (new_dist < curr_dist) {
                closest_swappable = p;
            }
        }
    }

    switch (card) {
        case 1:
        case 2:
            if (move(card, selected_pawn)) {
                cout << name << " used card " << card << " on pawn " << selected_pawn->id + 1 << "\n";
            } else {
                for (pawn *p : pawns) {
                    if (move(card, p)) {
                        cout << name << " used card " << card << " on pawn " << p->id + 1 << "\n";
                        return;
                    }
                }
                cout << "AI lost its turn\n";
            }

            
            break;
        case 3:
        case 4:
        case 5:
        case 8:
        case 12:
            if (selected_pawn->location->type == game_location::START) {
                cout << "AI lost its turn\n";
                return;
            }
            if (move(card, selected_pawn)) {
                cout << name << " used card " << card << " on pawn " << selected_pawn->id + 1 << "\n";
            } else {
                for (pawn *p : pawns) {
                    if (move(card, p)) {
                        cout << name << " used card " << card << " on pawn " << p->id + 1 << "\n";
                        return;
                    }
                }
                cout << "AI lost its turn\n";
            }

            break;
        
        case 7:
            if (selected_pawn->location->type == game_location::START) {
                cout << "AI lost its turn\n";
                return;
            }
            if (move(card, selected_pawn, 7)) {
                cout << name << " used card " << card << " on pawn " << selected_pawn->id + 1 << "\n";
            } else {
                for (pawn *p : pawns) {
                    if (move(card, p, 7)) {
                        cout << name << " used card " << card << " on pawn " << p->id + 1 << "\n";
                        return;
                    }
                }
                cout << "AI lost its turn\n";
            }
            break;


        case 10:
        case 11:
            if (selected_pawn->location->type == game_location::START) {
                cout << "AI lost its turn\n";
                return;
            }
            if (move(card, selected_pawn, 0)) {
                cout << name << " used card " << card << " on pawn " << selected_pawn->id + 1 << "\n";
            } else {
                for (pawn *p : pawns) {
                    if (move(card, p, 0)) {
                        cout << name << " used card " << card << " on pawn " << p->id + 1 << "\n";
                        return;
                    }
                }
                cout << "AI lost its turn\n";
            }
            break;

        case 0:
            if (!start_pawn || !closest_swappable) {
                cout << "AI lost its turn\n";
                return;
            }
            if (move(card, start_pawn, 0, closest_swappable)) {
                cout << name << " used card " << card << " on pawn " << selected_pawn->id + 1 << "\n";
            } else {
                cout << "AI lost its turn\n";
            }
            break;

        default:
            break;
    }

}