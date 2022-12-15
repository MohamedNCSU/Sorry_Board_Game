#include "game.h"
#include "player.h"
#include "cards.h"
#include <iostream>

using namespace std;

sorry_game::sorry_game(){
    info.board = new game_board();
    int input;

    cout << "Specify Player Type: Press 1 for user player, press 2 for AI player, press 3 for none\n";
    cin >> input;

    if(input == 1){
        info.players.push_back(new console_player("Red Player", RED, &info));
    } else if(input == 2){
        info.players.push_back(new ai_player("Red AI", RED, &info));
    }

    cout << "Specify Player Type: Press 1 for user player, press 2 for AI player, press 3 for none\n";
    cin >> input;

    if(input == 1){
        info.players.push_back(new console_player("Blue Player", BLUE, &info));
    } else if(input == 2){
        info.players.push_back(new ai_player("Blue AI", BLUE, &info));
    }

    cout << "Specify Player Type: Press 1 for user player, press 2 for AI player, press 3 for none\n";
    cin >> input;

    if(input == 1){
        info.players.push_back(new console_player("Yellow Player", YELLOW, &info));
    } else if(input == 2){
        info.players.push_back(new ai_player("Yellow AI", YELLOW, &info));
    }


    cout << "Specify Player Type: Press 1 for user player, press 2 for AI player, press 3 for none\n";
    cin >> input;

    if(input == 1){
        info.players.push_back(new console_player("Green Player", GREEN, &info));
    } else if(input == 2){
        info.players.push_back(new ai_player("Green AI", GREEN, &info));
    }

    deck.shuffle_deck();
}

sorry_game::sorry_game(int num_console_players) {
    info.board = new game_board();
    if (num_console_players > 4) num_console_players = 4;
    if (num_console_players < 0) num_console_players = 0;

    if (num_console_players >= 1) info.players.push_back(new console_player("Red Player", RED, &info));
    else info.players.push_back(new ai_player("Red AI", RED, &info));

    if (num_console_players >= 2) info.players.push_back(new console_player("Blue Player", BLUE, &info));
    else info.players.push_back(new ai_player("Blue AI", BLUE, &info));

    if (num_console_players >= 3) info.players.push_back(new console_player("Yellow Player", YELLOW, &info));
    else info.players.push_back(new ai_player("Yellow AI", YELLOW, &info));

    if (num_console_players == 4) info.players.push_back(new console_player("Green Player", GREEN, &info));
    else info.players.push_back(new ai_player("Green AI", GREEN, &info));

    deck.shuffle_deck();
}

player* sorry_game::play(){
    int i = 0;
    int num_players = info.players.size();

    while(true){
        player* curr_player = info.players[i];

        cout << "\n";
        cout << "-------------------------------------------------------------------------------\n";
        cout << "It is now " << curr_player->name << "'s turn\n";
        cout << "-------------------------------------------------------------------------------\n";
        cout << "\n";
        info.board->print();
        cout << "\n";
        info.board->print(&info);
        cout << "-------------------------------------------------------------------------------\n";

        int card = deck.draw_card();
        cout << curr_player->name << " drew a(n) '";
        if (card == 0) cout << "Sorry";
        else cout << card;
        cout << "' card!\n";

        curr_player->pick_action(card);

        if (card != 2) i = (i + 1) % num_players;

        if (curr_player->pawns.size() == 0) {
            // win condition, break out of loop
            cout << "Game Finished. " << curr_player->name << " Wins!\n";
            return curr_player;
        }

        // wait until user hits enter
    }
}