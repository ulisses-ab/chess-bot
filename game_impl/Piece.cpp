#include "Piece.hpp"

#include <stdexcept>
#include <cstdint>
#include <ctype.h>

Color flip(Color color) {
    switch (color) {
        case WHITE: return BLACK;
        case BLACK: return WHITE;
        default: throw new std::invalid_argument("Color cannot be NO_COLOR");
    }
}

Piece Piece::fromChar(char c) {
    PieceType type;
    Color color = isupper(c) ? WHITE : BLACK;

    switch (tolower(c)) {
        case 'p': type = PAWN; break;
        case 'n': type = KNIGHT; break;
        case 'b': type = BISHOP; break;
        case 'r': type = ROOK; break;
        case 'q': type = QUEEN; break;
        case 'k': type = KING; break;
        default: type = EMPTY; break;
    }

    Piece p(color, type);
    return p;
}

char Piece::getChar() const {
    char c = '.';

    switch (type) {
        case PAWN:   c ='p'; break;
        case KNIGHT: c ='n'; break;
        case BISHOP: c ='b'; break;
        case ROOK:   c ='r'; break;
        case QUEEN:  c ='q'; break;
        case KING:   c ='k'; break;
    }

    if(color == WHITE) {
        c = toupper(c);
    }

    return c;
}