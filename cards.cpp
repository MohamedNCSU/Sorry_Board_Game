#include "cards.h"

cards::cards() {
    curr_card = 0;

    int set[45] = {1,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,7,7,7,7,8,8,8,8,10,10,10,10,11,11,11,11,12,12,12,12,0,0,0,0};

    for (int i = 0; i < 45; i++) {
        deck[i] = set[i];
    }
}

void cards::shuffle_deck() { // the function generates 100 random numbers between 0 and 45
    // it then swaps cards at neighboring indices in the index array. This results in 50 card swaps
    int index[100];
    int temp;

    random_nums(index);

    for (int i = 0; i < 100; i = i + 2) {
        temp = deck[index[i]];
        deck[index[i]] = deck[index[i + 1]];
        deck[index[i + 1]] = temp;
    }
};

int cards::draw_card() { // this function increments an index that points to the next card in the deck
    // it returns the card at the index. If the whole deck is used, it is reshuffled and the index is reset to 0
    int card = deck[curr_card];
    curr_card++;

    if (curr_card == 45) {
        curr_card = 0;
        shuffle_deck();
    }

    return card;
}