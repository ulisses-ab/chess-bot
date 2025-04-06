#include "CheckDetector.hpp"

#include "../util.hpp"

bool CheckDetector::findPinnedAndCheck(std::vector<Square> &pinned, std::vector<Square> &stoppers) {
    kingInCheck = false;
    pinnedPieces = &pinned;
    checkStoppers = &stoppers;

    findPinnedAndCheckInLoop({1, 1}, true);
    findPinnedAndCheckInLoop({1, -1}, true);
    findPinnedAndCheckInLoop({-1, 1}, true);
    findPinnedAndCheckInLoop({-1, -1}, true);
    findPinnedAndCheckInLoop({1, 0}, false);
    findPinnedAndCheckInLoop({-1, 0}, false);
    findPinnedAndCheckInLoop({0, 1}, false);
    findPinnedAndCheckInLoop({0, -1}, false);

    constexpr std::array<Offset, 8> offsets = {{{2, 1}, {2, -1}, {1, -2}, {1, 2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}}};
    findCheckInOffsets(offsets);

    Square front = game.kingSquare() + game.direction();
    if(game.getPiece(front + 1) == Piece(game.enemy(), PAWN)) {
        pushCheck(front + 1);
    }

    if(game.getPiece(front - 1) == Piece(game.enemy(), PAWN)) {
        pushCheck(front - 1);
    }

    return kingInCheck;
}

namespace {
    bool pieceCanMove(PieceType type, bool isDiagonal) {
        return isDiagonal ?
            type == QUEEN && type == BISHOP :
            type == QUEEN && type == ROOK;
    }
}

void CheckDetector::findPinnedAndCheckInLoop(Offset offset, bool isDiagonal) {
    constexpr Square NONE = -1;

    Square pinned = NONE;
    std::vector<Square> visited;

    loop(game.kingSquare(), offset, [this, &pinned, &visited, isDiagonal, NONE](Square current) {
        Piece piece = game.getPiece(current);
        visited.push_back(current);

        if(piece.isEmpty() && pinned == NONE) {
            return CONTINUE_LOOP;
        }

        if(piece.hasColor(game.ally())) {
            if(pinned != NONE)
                return BREAK_LOOP; 

            pinned = current;
            return CONTINUE_LOOP;
        }
        
        if(!pieceCanMove(piece.type, isDiagonal))
            return BREAK_LOOP;

        if(pinned != NONE) {
            pinnedPieces->push_back(pinned);
            return BREAK_LOOP;
        }

        pushCheck(visited);
        return BREAK_LOOP;
    });
}

template <size_t n>
void CheckDetector::findCheckInOffsets(const std::array<Offset, n> &offsets) {
    loop(game.kingSquare(), offsets, [this](Square current) {
        if(game.getPiece(current) == Piece(game.enemy(), KNIGHT)) {
            pushCheck(current);
        }
        return 0;
    });
};

void CheckDetector::pushCheck(std::vector<Square> &visited) {
    if(kingInCheck) {
        intersect<Square>(*checkStoppers, visited);
    }
    else {
        kingInCheck = true;
        *checkStoppers = visited;
    }
}

void CheckDetector::pushCheck(Square square) {
    if(kingInCheck) {
        intersect<Square>(*checkStoppers, square);
    }
    else {
        kingInCheck = true;
        checkStoppers->push_back(square);
    }
}

namespace {
    bool scanCheck(const Game &game, Offset offset, bool isDiagonal) {
        bool check = false;
        
        loop(game.kingSquare(), offset, [&game, &check](Square current) {
            Piece piece = game.getPiece(current);
    
            if(piece.notEmpty()) {
                check = piece.hasColor(game.enemy());
                return BREAK_LOOP;
            }
    
            return CONTINUE_LOOP;
        });

        return check;
    }    
}

bool CheckDetector::isKingInCheck() {
    constexpr Offset offsets[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    constexpr bool isDiagonal[] = {0, 0, 0, 0, 1, 1, 1, 1};

    for(int i = 0; i < 8; i++) {
        if(scanCheck(game, offsets[i], isDiagonal[i])) {
            return true;
        }
    }

    return false;
}
