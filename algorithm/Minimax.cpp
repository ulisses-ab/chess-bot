#include "Minimax.hpp"
#include <algorithm>

Move Minimax::findBestMove(int depth) {
    std::vector<Move> *moves = moveGenerator.generatePossibleMoves();
    orderMoves(moves);
    
    int best = -10000000;
    Move bestMove;

    for(auto &move : *moves) {
        game.pushMove(move);
        
        int value = -negamax(depth-1, -10000000, -best);
        if(value > best) {
            best = value;
            bestMove = move;
        }

        game.popMove();
    } 

    delete moves;
    return bestMove;
}

int Minimax::negamax(int depth, int alpha, int beta) {
    std::vector<Move> *moves = moveGenerator.generatePossibleMoves();

    if(moves->size() == 0) {
        return evaluator.evaluateTerminalState();
    }

    if(moves->size() == 1) {
        return highestEvaluation(moves);
    }

    if(depth > 2) orderMoves(moves);

    int best = -10000000;

    for(auto &move : *moves) {
        game.pushMove(move);

        best = std::max(best, -negamax(depth-1, -beta, -alpha));
        alpha = std::max(best, alpha);

        if(alpha > beta)
            break;

        game.popMove();
    } 

    delete moves;
    return best;
}

void Minimax::orderMoves(std::vector<Move> *moves) {
    std::sort(moves->begin(), moves->end(), [this](Move a, Move b) {
        return evaluator.evaluateMove(a) > evaluator.evaluateMove(b);
    });
}

int Minimax::highestEvaluation(std::vector<Move> *moves) {
    int bestEval = -1000000;

    for(int i = 0; i < moves->size(); i++) {
        int eval = evaluator.evaluateMove(moves->at(i));

        bestEval = std::max(bestEval, eval);
    }

    return bestEval;
}