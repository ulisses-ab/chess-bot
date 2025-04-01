#include <cstdint>

#ifndef PIECE
#define PIECE

typedef std::uint8_t int8;

enum Color {
    BLACK,
    WHITE
};

enum Type {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE,
};

enum FullPiece {
    BLACK_PAWN,
    WHITE_PAWN,
    BLACK_KNIGHT,
    WHITE_KNIGHT,
    BLACK_BISHOP,
    WHITE_BISHOP,
    BLACK_ROOK,
    WHITE_ROOK,
    BLACK_QUEEN,
    WHITE_QUEEN,
    BLACK_KING,
    WHITE_KING,
    BLACK_NONE,
    WHITE_NONE
};

union Piece {
    int8 raw;
    struct {
        bool color : 1;
        int8 type : 3;
    };
};

#endif