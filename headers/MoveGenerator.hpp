#include "Board.hpp"

#ifndef MOVE_GENERATOR
#define MOVE_GENERATOR

class MoveGenerator {
public:
    static std::vector<Board> *generatePossibleMoves(const Board &board);
private:
    static bool turn;
    static Board board;
    static Board newBoard;
    static std::vector<Board> *moves;

    static void generatePawnMoves(int square);
    static void generateKnightMoves(int square);
    static void generateBishopMoves(int square);
    static void generateRookMoves(int square);
    static void generateQueenMoves(int square);
    static void generateKingMoves(int square);

    static void generatePawnPromotions(int square, int front);

    static void trySquare(int startSquare, int endSquare, bool capture = true, bool nonCapture = true);
    static void tryDisplacements(int startSquare, int displacements[], int horizontalDisplacements[], int n);
    static void trySquareLoop(int startSquare, int step);

    static void pushMove(int start, int end);

    static void pushPawnPromotion(int start, int end, int promotion);
    static void pushDoubleMove(int start, int end);
    static void pushEnPassant(int start);
    static void pushCastling(int start, int end);
};

#endif