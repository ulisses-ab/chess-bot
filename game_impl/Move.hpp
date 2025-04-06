#pragma once

#include <cstdint>
#include "Piece.hpp"
#include "Square.hpp"

struct Move {
    enum MoveType {
        NORMAL,
        CAPTURE,
        SHORT_CASTLING,
        LONG_CASTLING,
        PROMOTION,
        EN_PASSANT,
    };

    enum MoveAction {
        NO_ACTION, 
        RESET_HALFMOVE_COUNTER,
        SET_EN_PASSANT_TARGET,
        REMOVE_CASTLING_RIGHTS,
        REMOVE_KINGSIDE_CASTLING_RIGHTS,
        REMOVE_QUEENSIDE_CASTLING_RIGHTS,
    };

    Square start;
    Square end;
    PieceType captured : 3;
    MoveType type : 3;
    MoveAction action : 3;
    PieceType promoted : 3;

    Move() : start(0), end(0), captured(EMPTY), type(NORMAL), action(NO_ACTION), promoted(EMPTY) {};

    static Move normal(Square start, Square end, MoveAction action = NO_ACTION) {
        return Move(start, end, NORMAL, action);
    }

    static Move capture(Square start, Square end, PieceType captured, MoveAction action = NO_ACTION) {
        return Move(start, end, CAPTURE, action, captured);
    }

    static Move pawnDoubleMove(Square start, Color color) {
        return Move(start, color == WHITE ? start + 16 : start - 16, NORMAL, SET_EN_PASSANT_TARGET);
    }

    static Move promotion(Square start, Square end, PieceType promoted, PieceType captured = EMPTY) {
        return Move(start, end, PROMOTION, NO_ACTION, captured, promoted);
    }

    static Move enPassant(Square start, Square end) {
        return Move(start, end, EN_PASSANT);
    }

    static Move shortCastling(Color color) {
        Square kingSquare = color == WHITE ? 3 : 59;
        Square rookSquare = color == WHITE ? 0 : 56;
        return Move(kingSquare, rookSquare + 1, SHORT_CASTLING, REMOVE_CASTLING_RIGHTS);
    }

    static Move longCastling(Color color) {
        Square kingSquare = color == WHITE ? 3 : 59;
        Square rookSquare = color == WHITE ? 7 : 63;
        return Move(kingSquare, rookSquare - 2, LONG_CASTLING, REMOVE_CASTLING_RIGHTS);
    }

private:
    Move(Square start, Square end, MoveType type = NORMAL, MoveAction action = NO_ACTION, PieceType captured = EMPTY, PieceType promoted = EMPTY) 
    : start(start), end(end), type(type), action(action), captured(captured), promoted(promoted) {};
};