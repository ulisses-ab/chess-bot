#include "../game_impl/Game.hpp"
#include "../game_impl/MoveGenerator.hpp"
#include "Evaluator.hpp"

#ifndef MINIMAX
#define MINIMAX

class Minimax {
public:
    Minimax(Game &game) : game(game), moveGenerator(game), evaluator(game) {}
    Move findBestMove(int depth);
private:
    Game &game;
    MoveGenerator moveGenerator;
    Evaluator evaluator;

    int negamax(int depth, int precedingEval, int alpha, int beta);
    void orderMoves(std::vector<Move> *moves);
    int highestEvaluation(std::vector<Move> *moves);
};

#endif