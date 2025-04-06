#include "./game_impl/Game.hpp"
#include "./game_impl/MoveGenerator.hpp"
#include "./algorithm/Minimax.hpp"
#include <iostream>

int main() {
    Game game;
    Minimax mm(game);
    Move m;
    Evaluator e(game);

    int x = 1;
    game.print(true);
    while(true) {
        std::cout << e.evaluateGame() << std::endl;
        game.print(true);
        m = mm.findBestMove(7);
        game.pushMove(m);
    }
    





}

