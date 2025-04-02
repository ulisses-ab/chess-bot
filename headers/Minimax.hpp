#include "Board.hpp"

#ifndef MINIMAX
#define MINIMAX

class Minimax {
public:
    static Board findBestMove(const Board &board, int depth);
private:
    static int negamax(const Board &board, int depth, int player, int alpha, int beta);
    static void orderMoves(std::vector<Board> *moves);
};

#endif