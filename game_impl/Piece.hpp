#pragma once

enum Color {
    BLACK,
    WHITE,
    NO_COLOR,
};

Color flip(Color color);

enum PieceType {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};

struct Piece {
public:
    Color color : 2;
    PieceType type : 3;

    constexpr Piece() : color(NO_COLOR), type(EMPTY) {};
    constexpr Piece(Color c, PieceType t) : color(c), type(t) {};

    char getChar() const;
    static Piece fromChar(char c);

    bool isEmpty() const { 
        return type == EMPTY; 
    }

    bool notEmpty() const { 
        return type != EMPTY; 
    }

    bool hasColor(Color c) const { 
        return color == c; 
    }

    static Piece empty() { 
        return Piece();
    }

    bool operator==(Piece p) {
        return p.color == color && p.type == type;
    }

    bool operator!=(Piece p) {
        return p.color != color || p.type != type;
    }
};