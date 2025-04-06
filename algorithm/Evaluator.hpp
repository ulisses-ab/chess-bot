#pragma once

#include "../game_impl/Game.hpp"
#include "../game_impl/CheckDetector.hpp"

class Evaluator {
public:
    Evaluator(const Game &game) : game(game), checkDetector(game) {}

    int evaluateMove(Move move);
    int evaluateTerminalState();
private:
    const Game &game;

    CheckDetector checkDetector;

    static const int midgameTable[7][64];
    static const int endgameTable[7][64];

    int evaluateNormal(Move move);
    int evaluateCapture(Move move);
    int evaluateShortCastling(Move move);
    int evaluateLongCastling(Move move);
    int evaluatePromotion(Move move);
    int evaluateEnPassant(Move move);

    int evaluateDisplacement(Square start, Square end);
    int pst(Piece piece, Square square);
    int pst(Square square);
    Square relativePosition(Square square, Color color);
};