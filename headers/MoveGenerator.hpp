#include "Board.hpp"

#ifndef MOVE_GENERATOR
#define MOVE_GENERATOR

class MoveGenerator {
public:
    MoveGenerator(const Board &board);

    static std::vector<Board> *generatePossibleMoves(const Board &board);
    std::vector<Board> *generatePossibleMoves();
private:
    bool turn;
    Board board;
    Board newBoard;
    std::vector<Board> *moves;

    void generatePawnMoves(int square);
    void generateKnightMoves(int square);
    void generateBishopMoves(int square);
    void generateRookMoves(int square);
    void generateQueenMoves(int square);
    void generateKingMoves(int square);

    void generatePawnPromotions(int square, int front);

    void trySquare(int startSquare, int endSquare, bool capture = true, bool nonCapture = true);
    void tryDisplacements(int startSquare, int displacements[], int horizontalDisplacements[], int n);
    void trySquareLoop(int startSquare, int step);

    void pushMove(int start, int end);

    void pushPawnPromotion(int start, int end, int promotion);
    void pushDoubleMove(int start, int end);
    void pushEnPassant(int start);
    void pushCastling(int start, int end);
};

#endif