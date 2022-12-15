#include "board.h"
#include <algorithm>
#include <utility>

std::string output_template[16][16] = {
{"-NN","-RL","-RL","-RL","-RL","-NN","-NN","-NN","-NN","-RL","-RL","-RL","-RL","-RL","-NN","-NN"},
{"-NN","   ","-RF","   ","0RT","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-BL"},
{"-GL","   ","-RF","   ","   ","   ","   ","   ","   ","-BE","-BF","-BF","-BF","-BF","-BF","-BL"},
{"-GL","   ","-RF","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-BL"},
{"-GL","   ","-RF","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","0BT","-BL"},
{"-GL","   ","-RF","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-NN"},
{"-GL","   ","-RE","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-NN"},
{"-NN","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-NN"},
{"-NN","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-NN"},
{"-NN","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-YE","   ","-BL"},
{"-NN","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-YF","   ","-BL"},
{"-GL","0GT","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-YF","   ","-BL"},
{"-GL","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","-YF","   ","-BL"},
{"-GL","-GF","-GF","-GF","-GF","-GF","-GE","   ","   ","   ","   ","   ","   ","-YF","   ","-BL"},
{"-GL","   ","   ","   ","   ","   ","   ","   ","   ","   ","   ","0YT","   ","-YF","   ","-NN"},
{"-NN","-NN","-YL","-YL","-YL","-YL","-YL","-NN","-NN","-NN","-NN","-YL","-YL","-YL","-YL","-NN"}
};

void game_board::create_board() {
    int id = 0;
    game_location *head, *curr, *next, *safety;

    head = new game_location(game_location::SLIDE_START, game_color::RED, id++);
    curr = head;

    for (int i = 1; i < 60; i++) {
        game_color color = (game_color)(i / 15);
        switch (i % 15) {
            case 0:
            case 8:
            // make start of slide
            next = new game_location(game_location::SLIDE_START, color, id++, nullptr, curr, nullptr);
            break;
            case 1:
            // make location right before safety zone
            next = new game_location(game_location::SAFETY_ENTRANCE, color, id++, nullptr, curr, nullptr);
            // make safety zone
            safety = new game_location(game_location::SAFETY, color, id++, nullptr, next, nullptr);
            next->safety = safety;
            for (int j = 1; j < 5; j++) {
                game_location* next_safety = new game_location(game_location::SAFETY, color, id++, nullptr, safety, nullptr);
                safety->next = next_safety;
                safety = next_safety;
            }
            safety->next = new game_location(game_location::FINISH, color, id++, nullptr, safety, nullptr);
            break;
            case 2:
            case 9:
            case 10:
            case 11:
            // make slide locations
            next = new game_location(game_location::SLIDE, color, id++, nullptr, curr, nullptr);
            break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 13:
            case 14:
            // make normal locations
            next = new game_location(game_location::NONE, game_color::NONE, id++, nullptr, curr, nullptr);
            break;
            case 3:
            // make end of slide / connect starts
            next = new game_location(game_location::SLIDE_END, color, id++, nullptr, curr, nullptr);
            start[i / 15] = new game_location(game_location::START, color, id++);
            start[i / 15]->next = next;
            break;
            case 12:
            // make end of slide
            next = new game_location(game_location::SLIDE_END, color, id++, nullptr, curr, nullptr);
            break;
        }
        curr->next = next;
        curr = next;
    }
    curr->next = head;
    head->prev = curr;
}

game_board::game_board() {
    create_board();
}

/**
+---------------------------------------------------------------+
|087|000|001|008|009|011|012|013|014|015|016|017|018|019|020|021|
|086|   |002|   |010|   |   |   |   |   |   |   |   |   |   |022|
|085|   |003|   |   |   |   |   |   |029|028|027|026|025|024|023|
|084|   |004|   |   |   |   |   |   |   |   |   |   |   |   |030|
|083|   |005|   |   |   |   |   |   |   |   |   |   |   |032|031|
|082|   |006|   |   |   |   |   |   |   |   |   |   |   |   |033|
|081|   |007|   |   |   |   |   |   |   |   |   |   |   |   |034|
|080|   |   |   |   |   |   |   |   |   |   |   |   |   |   |035|
|079|   |   |   |   |   |   |   |   |   |   |   |   |   |   |036|
|078|   |   |   |   |   |   |   |   |   |   |   |   |051|   |037|
|077|   |   |   |   |   |   |   |   |   |   |   |   |050|   |038|
|075|076|   |   |   |   |   |   |   |   |   |   |   |049|   |039|
|074|   |   |   |   |   |   |   |   |   |   |   |   |048|   |040|
|067|068|069|070|071|072|073|   |   |   |   |   |   |047|   |041|
|066|   |   |   |   |   |   |   |   |   |   |054|   |046|   |042|
|065|064|063|062|061|060|059|058|057|056|055|053|052|045|044|043|
+---------------------------------------------------------------+

+---------------------------------------------------------------+
|-NN|-RL|-RL|-RL|-RL|-NN|-NN|-NN|-NN|-RL|-RL|-RL|-RL|-RL|-NN|-NN|
|-NN|   |-RF|   |0RT|   |   |   |   |   |   |   |   |   |   |-BL|
|-GL|   |-RF|   |   |   |   |   |   |-BE|-BF|-BF|-BF|-BF|-BF|-BL|
|-GL|   |-RF|   |   |   |   |   |   |   |   |   |   |   |   |-BL|
|-GL|   |-RF|   |   |   |   |   |   |   |   |   |   |   |0BT|-BL|
|-GL|   |-RF|   |   |   |   |   |   |   |   |   |   |   |   |-NN|
|-GL|   |-RE|   |   |   |   |   |   |   |   |   |   |   |   |-NN|
|-NN|   |   |   |   |   |   |   |   |   |   |   |   |   |   |-NN|
|-NN|   |   |   |   |   |   |   |   |   |   |   |   |   |   |-NN|
|-NN|   |   |   |   |   |   |   |   |   |   |   |   |-YE|   |-BL|
|-NN|   |   |   |   |   |   |   |   |   |   |   |   |-YF|   |-BL|
|-GL|0GT|   |   |   |   |   |   |   |   |   |   |   |-YF|   |-BL|
|-GL|   |   |   |   |   |   |   |   |   |   |   |   |-YF|   |-BL|
|-GL|-GF|-GF|-GF|-GF|-GF|-GE|   |   |   |   |   |   |-YF|   |-BL|
|-GL|   |   |   |   |   |   |   |   |   |   |0YT|   |-YF|   |-NN|
|-NN|-NN|-YL|-YL|-YL|-YL|-YL|-NN|-NN|-NN|-NN|-YL|-YL|-YL|-YL|-NN|
+---------------------------------------------------------------+

What each character means (ex: -NN = 012)
0 = (-) No pawn, (R,B,Y,G) pawn of that color
1 = (R,B,Y,G) location of that color
2 = (N) normal, (L) slide, (F) safety, (T) start, (E) end/home
*/

std::pair<int,int> get_position(int id) {
    int side = id / 22;
    int pos = id % 22;

    int row, col;

    // select position
    if (pos == 0) {
        row = 0;
        col = 1;
    } else if (1 <= pos && pos <= 7) {
        row = pos - 1;
        col = 2;
    } else if (pos == 8) {
        row = 0;
        col = 3;
    } else if (9 <= pos && pos <= 10) {
        row = pos - 9;
        col = 4;
    } else if (11 <= pos && pos <= 21) {
        row = 0;
        col = pos - 6;
    }
    
    // rotate based on side
    switch (side) {
        case 0:
        return {row, col};
        case 1:
        return {col, 15 - row};
        case 2:
        return {15 - row, 15 - col};
        case 3:
        return {15 - col, row};
    }

    // returns invalid position if given invalid id
    return {-1, -1};
}

void update_out(std::string out[16][16], const pawn& p) {
    int id = p.location->id;
    std::pair<int,int> pos = get_position(id);
    char new_c;
    if ((id % 22) == 10) {
        new_c = out[pos.first][pos.second][0] + 1;
    } else {
        switch (p.color) {
            case game_color::RED:
            new_c = 'R';
            break;
            case game_color::BLUE:
            new_c = 'B';
            break;
            case game_color::YELLOW:
            new_c = 'Y';
            break;
            case game_color::GREEN:
            new_c = 'G';
            break;
            case game_color::NONE:
            // should never be this case
            break;
        }
    }
    out[pos.first][pos.second][0] = new_c;
}

void game_board::print(game_info *info) {
    if (info == nullptr) {
        // print board layout
        cout << "+---------------------------------------------------------------+\n";
        cout << "|087|000|001|008|009|011|012|013|014|015|016|017|018|019|020|021|\n";
        cout << "|086|   |002|   |010|   |   |   |   |   |   |   |   |   |   |022|\n";
        cout << "|085|   |003|   |   |   |   |   |   |029|028|027|026|025|024|023|\n";
        cout << "|084|   |004|   |   |   |   |   |   |   |   |   |   |   |   |030|\n";
        cout << "|083|   |005|   |   |   |   |   |   |   |   |   |   |   |032|031|\n";
        cout << "|082|   |006|   |   |   |   |   |   |   |   |   |   |   |   |033|\n";
        cout << "|081|   |007|   |   |   |   |   |   |   |   |   |   |   |   |034|\n";
        cout << "|080|   |   |   |   |   |   |   |   |   |   |   |   |   |   |035|\n";
        cout << "|079|   |   |   |   |   |   |   |   |   |   |   |   |   |   |036|\n";
        cout << "|078|   |   |   |   |   |   |   |   |   |   |   |   |051|   |037|\n";
        cout << "|077|   |   |   |   |   |   |   |   |   |   |   |   |050|   |038|\n";
        cout << "|075|076|   |   |   |   |   |   |   |   |   |   |   |049|   |039|\n";
        cout << "|074|   |   |   |   |   |   |   |   |   |   |   |   |048|   |040|\n";
        cout << "|067|068|069|070|071|072|073|   |   |   |   |   |   |047|   |041|\n";
        cout << "|066|   |   |   |   |   |   |   |   |   |   |054|   |046|   |042|\n";
        cout << "|065|064|063|062|061|060|059|058|057|056|055|053|052|045|044|043|\n";
        cout << "+---------------------------------------------------------------+\n";
        return;
    }

    // print board with pawn info
    std::string output[16][16];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            output[i][j] = output_template[i][j];
        }
    }
    for (player *pl : info->players) {
        for (auto p : pl->pawns) {
            update_out(output, *p);
        }
    }
    cout << "+---------------------------------------------------------------+\n";
    for (int i = 0; i < 16; i++) {
        cout << '|';
        for (int j = 0; j < 16; j++) {
            cout << output[i][j] << '|';
        }
        cout << '\n';
    }
    cout << "+---------------------------------------------------------------+\n";
    cout.flush();
}