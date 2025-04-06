#pragma once

#include "Game.hpp"
#include <vector>

class CheckDetector {
public:
    CheckDetector(const Game &game) : game(game) {}

    bool findPinnedAndCheck(std::vector<Square> &pinnedPieces, std::vector<Square> &checkStoppers);
    bool isKingInCheck();
private:
    const Game &game;
    bool kingInCheck;

    std::vector<Square> *pinnedPieces, *checkStoppers;

    void findPinnedAndCheckInLoop(Offset offset, bool diagonal);
    template <size_t n>
    void findCheckInOffsets(const std::array<Offset, n> &offsets);

    void pushCheck(std::vector<Square> &stoppers);
    void pushCheck(Square stopper);
};