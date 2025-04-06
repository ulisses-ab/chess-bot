#pragma once

#include "Piece.hpp"
#include "Move.hpp"
#include <array>
#include <string>
#include <stack>

struct CastlingRights {
    bool kingside : 1;
    bool queenside : 1;

    CastlingRights() : kingside(true), queenside(true) {}
};

class Game {
public:
    Game();

    Game(std::string fen);
    std::string toFen() const;

    void print(bool details = false) const;

    void pushMove(Move move);
    void popMove();

    bool isStart() const {
        return pastMoves.empty() && pastCastlingRights.empty() && pastHalfmoveCounters.empty();
    }

    Piece getPiece(Square square) const {
        return board[square];
    }

    Piece getPiece(int row, int column) const {
        return getPiece(Square(row, column));   
    }
    
    Color getTurn() const {
        return turn;
    }

    Color ally() const {
        return getTurn();
    }

    Color enemy() const {
        return flip(getTurn());
    }

    Square getEnPassantTarget() const {
        return enPassantTarget;
    }

    CastlingRights getCastlingRights(Color color) const {
        return castlingRights[color];
    }

    int getHalfmoveCounter() const {
        return halfmoveCounter;
    }

    int getPhase() const {
        return std::min(std::max(phase, 0), 255);
    }

    Offset direction() const {
        return turn == WHITE ? Offset(1, 0) : Offset(-1, 0);
    }

    int relativeRow(int row) {
        return ally() == WHITE ? row : 7 - row;
    }

    int relativeRow(Square square) const {
        return ally() == WHITE ? square.row : 7 - square.row;
    }

    Square kingSquare() const {
        return kingSquare_[getTurn()];
    }
private:
    struct {
        Piece pieces[64];

        Piece &operator[](Square square) {
            return pieces[square.number];
        }

        const Piece &operator[](Square square) const {
            return pieces[square.number];
        }
    } board;

    Square kingSquare_[2];
    CastlingRights castlingRights[2];
    Square enPassantTarget;
    int halfmoveCounter;
    Color turn;
    int phase;

    std::stack<Move> pastMoves;
    std::stack<CastlingRights> pastCastlingRights;
    std::stack<int> pastHalfmoveCounters;

    void pushActions(Move move);

    void pushNormal(Move move);
    void pushCapture(Move move);
    void pushShortCastling(Move move);
    void pushLongCastling(Move move);
    void pushPromotion(Move move);
    void pushEnPassant(Move move);

    void popActions(Move move);

    void popNormal(Move move);
    void popCapture(Move move);
    void popShortCastling(Move move);
    void popLongCastling(Move move);
    void popPromotion(Move move);
    void popEnPassant(Move move);

    void movePiece(Square start, Square end);

    void removeCastlingRights() {
        pastCastlingRights.push(castlingRights[turn]);
        castlingRights[turn].kingside = false;
        castlingRights[turn].queenside = false;
    }

    void removeKingsideCastlingRights() {
        pastCastlingRights.push(castlingRights[turn]);
        castlingRights[turn].kingside = false;
    }

    void removeQueensideCastlingRights() {
        pastCastlingRights.push(castlingRights[turn]);
        castlingRights[turn].queenside = false;
    }

    void popCastlingRights() {
        castlingRights[turn] = pastCastlingRights.top();
        pastCastlingRights.pop();
    }

    void resetHalfmoveCounter() {
        pastHalfmoveCounters.push(halfmoveCounter);
        halfmoveCounter = 0;
    }

    void popHalfmoveCounter() {
        halfmoveCounter = pastHalfmoveCounters.top();
        pastHalfmoveCounters.pop();
    }

};