#ifndef _cards_h_
#define _cards_h_

#include <random>
#include <ctime>

class cards {
private:
    int deck[45];
    int curr_card;

    int* random_nums(int (&vals)[100]) {

        std::srand(std::time(nullptr));

        for (int n = 0; n < 100; ++n) {
            vals[n] = rand() % 45;
        }

        return vals;
    }
public:
    cards();

    void shuffle_deck();

    int draw_card();

    
};

#endif