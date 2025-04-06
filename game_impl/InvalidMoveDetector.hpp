#pragma once

#include "Game.hpp"
#include "CheckDetector.hpp"
#include "UnsafeSquareRecorder.hpp"
#include <vector>

class InvalidMoveDetector {
public:
    InvalidMoveDetector(const Game &game) : game(game), checkDetector(game), unsafeRecorder(game) {};

    void filterMoves(std::vector<Move> &moves, std::vector<Square> &enemySquares, Square kingPosition);
private:
    const Game &game;

    CheckDetector checkDetector;
    UnsafeSquareRecorder unsafeRecorder;

    void filterMovesWhichCauseACheck(std::vector<Move> &moves, std::vector<Square> &pinnedPieces, std::vector<Square> &unsafeSquares);
    void filterMovesWhichDontStopCheck(std::vector<Move> &moves, std::vector<Square> &checkStoppers);
};