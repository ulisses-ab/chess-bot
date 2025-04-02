#include "Piece.hpp"
#include <array>
#include <vector>

#ifndef BOARD
#define BOARD

class Board {
public:
    std::array<uint8_t, 32> board;
    uint8_t flags;
    int8_t enPassantTarget;
    int8_t halfmoveClock;

    Board();
    Board(std::string fen);
    void print(bool details = false) const;
    int evaluate() const;
    std::vector<Board> *generatePossibleMoves() const;
    std::string toFen() const;

    bool getTurn() const {
        return flags & 0b00001;
    }

    bool getCastlingRight(bool color, bool side) const {
        return flags & (0b00010 << (color * 2 + side));
    }

    void removeCastlingRight(bool color, bool side) {
        flags &= ~(0b00010 << (color * 2 + side));
    }

    void removeCastlingRights(bool color) {
        flags &= ~(0b00110 << (color * 2));
    }

    int getPiece(int square) const {
        return (board[square/2] >> (4*(square%2))) & 0b1111;
    }

    int getPiece(int row, int column) const {
        return getPiece(8*row + column);
    }

    void setPiece(int square, int piece) {
        board[square/2] &= 0b11110000 >> (4*(square%2));
        board[square/2] |= piece << (4*(square%2));
    }
private:
    static const char pieceChar[];
};

#endif