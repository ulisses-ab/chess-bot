#include "Piece.hpp"
#include "Move.hpp"
#include <vector>
#include <stack>

#ifndef BOARD
#define BOARD

class Board {
    Piece board[64];
    std::vector<int> pieceSquares[12];
    std::stack<std::pair<int, int>> enPassantTargetStack;
    std::stack<int> halfmoveResetStack;
    int totalHalfmoves;
    bool turn;
    bool kingHasMoved;
    bool blackKRookHasMoved, blackQRookHasMoved, whiteKRookHasMoved, whiteQRookHasMoved;
public:
    Board();
    void print();
    void printDetails();
    std::vector<Move>* possibleMoves();
    void pushMove(Move move);
    void popMove(Move move);
private:
    void addPossiblePawnMoves(std::vector<Move>* moves, int square);
    void addPossibleKnightMoves(std::vector<Move>* moves, int square);
    void addPossibleBishopMoves(std::vector<Move>* moves, int square);
    void addPossibleRookMoves(std::vector<Move>* moves, int square);
    void addPossibleQueenMoves(std::vector<Move>* moves, int square);
    void addPossibleKingMoves(std::vector<Move>* moves, int square);
    void popNumber(std::vector<int>& v, int number);
};

#endif