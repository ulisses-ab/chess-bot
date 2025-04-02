#include <cstdint>

#ifndef PIECE
#define PIECE

typedef std::uint8_t int8;

enum Color {
    BLACK,
    WHITE
};

enum Type {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};

enum Full {
    BLACK_PAWN      = 0b0001,
    BLACK_KNIGHT    = 0b0010,
    BLACK_BISHOP    = 0b0011,
    BLACK_ROOK      = 0b0100,
    BLACK_QUEEN     = 0b0101,
    BLACK_KING      = 0b0110,

    WHITE_PAWN      = 0b1001,
    WHITE_KNIGHT    = 0b1010,
    WHITE_BISHOP    = 0b1011,
    WHITE_ROOK      = 0b1100,
    WHITE_QUEEN     = 0b1101,
    WHITE_KING      = 0b1110,
};

#endif