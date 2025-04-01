#include "./../headers/Board.hpp"
#include <iostream>
#include <typeinfo>

int main() {
    Board* b = new Board();

    std::vector<Move>* moves = b->possibleMoves();

    b->print();


    for(auto move : *moves) {
        b->pushMove(move);
        b->print();
        b->popMove(move);
    }

    return 0;
}