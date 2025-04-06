#pragma once

#include "Game.hpp"
#include "Move.hpp"
#include "InvalidMoveDetector.hpp"
#include <vector>
#include <unordered_set>

class MoveGenerator {
public:
    MoveGenerator(const Game &game) : game(game), invalidDetector(game) {};
    std::vector<Move> *generatePossibleMoves();
private:
    const Game &game;
    std::vector<Move> *moves;
    
    InvalidMoveDetector invalidDetector;

    void push(Move m);

    void generatePawnMoves(Square square);
    void generateKnightMoves(Square square);
    void generateBishopMoves(Square square);
    void generateRookMoves(Square square);
    void generateQueenMoves(Square square);
    void generateKingMoves(Square square);
    void generatePawnPromotions(Square square, Square front);
    void generateCastlings(Square square);

    void tryCapturingMove(Square startSquare, Square endSquare, Move::MoveAction action = Move::NO_ACTION);
    void tryNonCapturingMove(Square startSquare, Square endSquare, Move::MoveAction action = Move::NO_ACTION);
    void tryRegularMove(Square startSquare, Square endSquare, Move::MoveAction action = Move::NO_ACTION);
    template <size_t n>
    void tryOffsets(Square startSquare, const std::array<Offset, n> &offsets, Move::MoveAction action = Move::NO_ACTION);
    void tryLoop(Square startSquare, Offset offset, Move::MoveAction action = Move::NO_ACTION);
};