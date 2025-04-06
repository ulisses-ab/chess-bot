#include "Minimax.hpp"
#include <algorithm>

Move Minimax::findBestMove(int depth) {
    std::vector<Move> *moves = moveGenerator.generatePossibleMoves();

    if(moves->size() == 0) {
        return Move::normal(0, 0);
    }

    orderMoves(moves);
    
    int best = -10000000;
    Move bestMove;

    for(auto &move : *moves) {
        int eval = evaluator.evaluateMove(move);
        game.pushMove(move);
        
        int value = -negamax(depth-1, eval, -10000000, -best);
        if(value > best) {
            best = value;
            bestMove = move;
        }

        game.popMove();
    } 

    delete moves;
    return bestMove;
}
#include <iostream>
int Minimax::negamax(int depth, int precedingEval, int alpha, int beta) {
    std::vector<Move> *moves = moveGenerator.generatePossibleMoves();

    if(moves->size() == 0) {
        return evaluator.evaluateTerminalState();
    }

    if(depth == 1) {
        return highestEvaluation(moves) - precedingEval;
    }

    if(depth > 2) orderMoves(moves);

    int best = -10000000;

    for(auto &move : *moves) {
        int eval = evaluator.evaluateMove(move);
        game.pushMove(move);
    

        //if(depth == 3)        game.print(true);
        //if(depth == 3) std::cout << move.start.number << " " << move.end.number << " " << move.type << std::endl;
        best = std::max(best, -negamax(depth-1, eval - precedingEval, -beta, -alpha));
        alpha = std::max(best, alpha);
        //if(depth == 3 && game.getPiece(0).isEmpty() && game.getPiece(8).type != ROOK && game.getPiece(1).type != ROOK) exit(0);

        game.popMove();

        if(alpha > beta)
            break;
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