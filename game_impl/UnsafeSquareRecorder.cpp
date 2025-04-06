#include "UnsafeSquareRecorder.hpp"

#include "../util.hpp"

void UnsafeSquareRecorder::record(std::vector<Square> &enemySquares, std::vector<Square> &unsafeSquares) {
    unsafe = &unsafeSquares;
}

void UnsafeSquareRecorder::recordSquare(Square square) {
    Piece piece = game.getPiece(square);

    switch(piece.type) {
        case PAWN: recordPawnAttacks(square); break; 
        case KNIGHT: recordKnightAttacks(square); break;
        case BISHOP: recordBishopAttacks(square); break;
        case ROOK: recordRookAttacks(square); break;
        case QUEEN: recordQueenAttacks(square); break;
        case KING: recordKingAttacks(square); break;
    }
}

void UnsafeSquareRecorder::recordPawnAttacks(Square square) {
    if(square.col != 0) {
        pushUnsafe(square - game.direction() - 1);
    }

    if(square.col != 7) {
        pushUnsafe(square - game.direction() + 1);
    }
}

void UnsafeSquareRecorder::recordKnightAttacks(Square square) {
    constexpr std::array<Offset, 8> offsets = {{{2, 1}, {2, -1}, {1, -2}, {1, 2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}}};
    recordOffsetAttacks(square, offsets);
}

void UnsafeSquareRecorder::recordBishopAttacks(Square square) {
    recordLoopAttacks(square, {1, 1});
    recordLoopAttacks(square, {1, -1});
    recordLoopAttacks(square, {-1, 1});
    recordLoopAttacks(square, {-1, -1});
}

void UnsafeSquareRecorder::recordRookAttacks(Square square) {
    recordLoopAttacks(square, {1, 0});
    recordLoopAttacks(square, {-1, 0});
    recordLoopAttacks(square, {0, 1});
    recordLoopAttacks(square, {0, -1});
}

void UnsafeSquareRecorder::recordQueenAttacks(Square square) {
    recordLoopAttacks(square, {1, 1});
    recordLoopAttacks(square, {1, -1});
    recordLoopAttacks(square, {-1, 1});
    recordLoopAttacks(square, {-1, -1});
    recordLoopAttacks(square, {1, 0});
    recordLoopAttacks(square, {-1, 0});
    recordLoopAttacks(square, {0, 1});
    recordLoopAttacks(square, {0, -1});
}

void UnsafeSquareRecorder::recordKingAttacks(Square square) {
    constexpr std::array<Offset, 8> offsets = {{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}};
    recordOffsetAttacks(square, offsets);
}

template <size_t n>
void UnsafeSquareRecorder::recordOffsetAttacks(Square square, const std::array<Offset, n> &offsets) {
    Utils::loop(square, offsets, [this](Square current){
        pushUnsafe(current);
        return CONTINUE_LOOP;
    });
}

void UnsafeSquareRecorder::recordLoopAttacks(Square square, Offset offset) {
    Square end = square;

    while(end.offsetIsSafe(offset)) {
        end += offset;

        if(!game.getPiece(end).isEmpty()) {
            pushUnsafe(end);
            break;
        }
        
        pushUnsafe(end);
    }
}

void UnsafeSquareRecorder::pushUnsafe(Square square) {
    if(game.getPiece(square).isEmpty()) {
        unsafe->push_back(square);
    }
}
