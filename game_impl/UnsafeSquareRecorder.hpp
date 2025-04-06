#pragma once

#include "Game.hpp"
#include <vector>

class UnsafeSquareRecorder {
public:
    UnsafeSquareRecorder(const Game &game) : game(game) {}

    void record(std::vector<Square> &enemySquares, std::vector<Square> &unsafeSquares);
private:
    const Game &game;

    std::vector<Square> *unsafe;

    void recordSquare(Square square);
    void recordPawnAttacks(Square square);
    void recordKnightAttacks(Square square);
    void recordBishopAttacks(Square square);
    void recordRookAttacks(Square square);
    void recordQueenAttacks(Square square);
    void recordKingAttacks(Square square);
    template <size_t n>
    void recordOffsetAttacks(Square square, const std::array<Offset, n> &offsets);
    void recordLoopAttacks(Square square, Offset offset);

    void pushUnsafe(Square square);
};