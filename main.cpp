#include "./game_impl/Game.hpp"
#include "./game_impl/MoveGenerator.hpp"
#include <iostream>

void lop(std::vector<Move> *moves, Game &game) {
    int i = 1;
    for(auto move : *moves) {
        game.pushMove(move);
        std::cout << i << '\n';
        game.print(true);
        game.popMove();
        i++;
    }
}

int main() {
    Game game;
    MoveGenerator moveGen(game);

    for(int j = 0;; j++) {
        std::vector<Move> *moves = moveGen.generatePossibleMoves();
        std::cout << moves->size() << '\n';


        lop(moves, game);
    
        int choices[] = {13, 20, 28, 21, 30, 19, 33};
        int choice = choices[j];

        game.pushMove((*moves)[choice-1]);
        game.print(true);

        std::cout << "-----------------------------------------------------\n";
        delete moves;
    }

    while(!game.isStart()) {
        game.popMove();
    }


    game.print(true);
}

