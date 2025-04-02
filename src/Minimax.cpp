#include "./../headers/Minimax.hpp"
#include <algorithm>

Board Minimax::findBestMove(const Board &board, int depth) {
    std::vector<Board> *moves = board.generatePossibleMoves();
    orderMoves(moves);

    int player = board.getTurn() ? 1 : -1;
    
    int best = -10000000;
    Board bestMove;

    for(auto &move : *moves) {
        int value = -negamax(move, depth-1, -player, -10000000, -best);
        if(value > best) {
            best = value;
            bestMove = move;
        }
    } 

    delete moves;
    return bestMove;
}

int Minimax::negamax(const Board &board, int depth, int player, int alpha, int beta) {
    if(depth == 0)
        return board.evaluate() * player;

    std::vector<Board> *moves = board.generatePossibleMoves();
    if(depth > 2) orderMoves(moves);

    int best = -10000000;

    for(auto &move : *moves) {
        best = std::max(best, -negamax(move, depth-1, -player, -beta, -alpha));
        alpha = std::max(best, alpha);

        if(alpha > beta)
            break;
    } 

    delete moves;
    return best;
}

void Minimax::orderMoves(std::vector<Board> *moves) {
    auto f = moves->at(0).getTurn() ?
        [](const Board& a, const Board& b) {return a.evaluate() < b.evaluate();} :
        [](const Board& a, const Board& b) {return a.evaluate() > b.evaluate();};

    std::sort(moves->begin(), moves->end(), f);
}