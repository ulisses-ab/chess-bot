#include "Game.hpp"
#include <iostream>

using namespace std;

Game::Game() : board{{
    {WHITE, ROOK}, {WHITE, KNIGHT}, {WHITE, BISHOP}, {WHITE, KING}, 
    {WHITE, QUEEN}, {WHITE, BISHOP}, {WHITE, KNIGHT}, {WHITE, ROOK},
    {WHITE, PAWN}, {WHITE, PAWN}, {WHITE, PAWN}, {WHITE, PAWN},
    {WHITE, PAWN}, {WHITE, PAWN}, {WHITE, PAWN}, {WHITE, PAWN},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY}, {NO_COLOR, EMPTY},
    {BLACK, PAWN}, {BLACK, PAWN}, {BLACK, PAWN}, {BLACK, PAWN},
    {BLACK, PAWN}, {BLACK, PAWN}, {BLACK, PAWN}, {BLACK, PAWN},
    {BLACK, ROOK}, {BLACK, KNIGHT}, {BLACK, BISHOP}, {BLACK, KING},
    {BLACK, QUEEN}, {BLACK, BISHOP}, {BLACK, KNIGHT}, {BLACK, ROOK},
}}, turn{WHITE}, enPassantTarget{-1}, halfmoveCounter{0}, phase{300}, kingSquare_{59, 3} {};
 
Game::Game(string fen) : board{{{NO_COLOR, EMPTY}}}, turn{WHITE}, enPassantTarget{-1}, halfmoveCounter{0}, phase{300}, kingSquare_{59, 3} {
    Square square = 63;

    int i = 0;
    for(; true; i++, square -= 1) {
        char c = fen[i];

        if(c == ' ') {
            break;
        }

        if(c == '/') {
            square += 1;
            continue;
        }

        if(isdigit(c)) {
            square -= c - '1';
        }

        board[square] = Piece::fromChar(c);
        if(c == 'K') {
            kingSquare_[WHITE] = square;
        }

        if(c == 'k') {
            kingSquare_[BLACK] = square;
        }
    }

    i++;
    turn = (fen[i] == 'w') ? WHITE : BLACK;
    i++;

    for(; fen[i] != ' '; i++) {
        switch(fen[i]) {
            case 'K':
                castlingRights[WHITE].kingside = true;
                break;
            case 'Q':
                castlingRights[WHITE].queenside = true;
                break;
            case 'k':
                castlingRights[BLACK].kingside = true;
                break;
            case 'q':
                castlingRights[BLACK].queenside = true;
                break;
        }
    }

    i++;
    if(fen[i] == '-') {
        enPassantTarget = -1;
    }
    else {
        int col = ('h') - fen[i];
        i++;
        int row = fen[i] - '1';

        int square = 8*col + row;
        enPassantTarget = square;
    }
    i += 2;

    if(i == fen.size() - 1) {
        halfmoveCounter = fen[i] - '0'; 
    }
    else {
        halfmoveCounter = fen[i] - '0';
        halfmoveCounter *= 10;
        halfmoveCounter += fen[i+1] - '0';
    }
}

void Game::print(bool printFen) const {
    for(Square square(63); square.isSafe(); square -= 1) {
        cout << getPiece(square).getChar() << " ";

        if(square.number % 8 == 0)
            cout << "\n";
    }

    cout << "\n";

    if(printFen) {
        cout << toFen() << "\n";
    } 
}

string Game::toFen() const {
    string output = "";

    for(int row = 7; row >= 0; row--) {
        int emptyCounter = 0;
        string rowString = "";

        for(int column = 7; column >= 0; column--) {
            Piece piece = getPiece(row, column);

            if(piece.isEmpty()) {
                emptyCounter++;
                continue;
            }

            if(emptyCounter > 0) {
                rowString += to_string(emptyCounter);
                emptyCounter = 0;
            }

            rowString += piece.getChar();
        }

        if(emptyCounter > 0) {
            rowString += to_string(emptyCounter);
        }

        output += rowString + (row == 0 ? " " : "/");
    }

    output += turn ? 'w' : 'b';
    output += " ";
    output += castlingRights[WHITE].kingside ? "K" : "";
    output += castlingRights[WHITE].queenside ? "Q" : "";
    output += castlingRights[BLACK].kingside ? "k" : "";
    output += castlingRights[BLACK].queenside ? "q" : "";

    if(output[output.size() - 1] == ' ') {
        output += '-';
    }

    output += ' ';

    if(enPassantTarget == 1) {
        output += '-';
    }
    else {
        output += ('h' - enPassantTarget.col);
        output += to_string(enPassantTarget.row + 1);
    }
    
    output += ' ';

    output += to_string(halfmoveCounter);

    return output;
}

void Game::pushMove(Move move) {
    pastMoves.push(move);

    if(getPiece(move.start).type == KING) {
        kingSquare_[turn] = move.end;
    }

    switch (move.type) {
    case Move::NORMAL:
        pushNormal(move);
        break;
    case Move::CAPTURE:
        resetHalfmoveCounter();
        pushCapture(move);
        break;
    case Move::SHORT_CASTLING:
        removeCastlingRights();
        pushShortCastling(move);
        break;
    case Move::LONG_CASTLING:
        removeCastlingRights();
        pushLongCastling(move);
        break;
    case Move::PROMOTION: 
        pushPromotion(move);
        break;
    case Move::EN_PASSANT:
        pushEnPassant(move);
        break;
    }

    pushActions(move);

    turn = flip(turn);
}

void Game::pushActions(Move move) {
    enPassantTarget = -1;

    switch (move.action) {
    case Move::SET_EN_PASSANT_TARGET:
        enPassantTarget = move.start + direction();
    case Move::RESET_HALFMOVE_COUNTER:
        resetHalfmoveCounter();
        break;
    case Move::REMOVE_CASTLING_RIGHTS:
        removeCastlingRights();
        break;
    case Move::REMOVE_KINGSIDE_CASTLING_RIGHTS:
        removeKingsideCastlingRights();
        break;
    case Move::REMOVE_QUEENSIDE_CASTLING_RIGHTS:
        removeQueensideCastlingRights();
        break;
    }
}

void Game::pushNormal(Move move) {
    movePiece(move.start, move.end);
}

void Game::pushCapture(Move move) {
    movePiece(move.start, move.end);

    switch (move.captured) {
        case KNIGHT: phase -= 16; break;
        case BISHOP: phase -= 16; break;
        case ROOK: phase -= 32; break;
        case QUEEN: phase -= 48; break;
    }
}

void Game::pushShortCastling(Move move) {
    movePiece(move.start, move.end);
    movePiece(move.end - 1, move.start - 1);
}

void Game::pushLongCastling(Move move) {
    movePiece(move.start, move.end);
    movePiece(move.end + 2, move.start + 1);
}

void Game::pushPromotion(Move move) {
    movePiece(move.start, move.end);
    board[move.end] = Piece(turn, move.promoted);
}

void Game::pushEnPassant(Move move) {
    movePiece(move.start, move.end);
    board[move.end-direction()] = Piece::empty(); 
}

void Game::popMove() {
    Move move = pastMoves.top();
    pastMoves.pop();

    turn = flip(turn);

    if(getPiece(move.start).type == KING) {
        kingSquare_[turn] = move.start;
    }

    switch (move.type) {
    case Move::NORMAL:
        popNormal(move);
        break;
    case Move::CAPTURE:
        popHalfmoveCounter();
        popCapture(move);
        break;
    case Move::SHORT_CASTLING:
        popCastlingRights();
        popShortCastling(move);
        break;
    case Move::LONG_CASTLING:
        popCastlingRights();
        popLongCastling(move);
        break;
    case Move::PROMOTION: 
        popPromotion(move);
        break;
    case Move::EN_PASSANT:
        popEnPassant(move);
        break;
    }

    popActions(move);
}

void Game::popActions(Move move) {
    if(!pastMoves.empty() && pastMoves.top().action == Move::SET_EN_PASSANT_TARGET) {
        enPassantTarget = pastMoves.top().start + direction();
    }

    switch (move.action) {
    case Move::SET_EN_PASSANT_TARGET:
    case Move::RESET_HALFMOVE_COUNTER:
        resetHalfmoveCounter();
        break;
    case Move::REMOVE_CASTLING_RIGHTS:
    case Move::REMOVE_KINGSIDE_CASTLING_RIGHTS:
    case Move::REMOVE_QUEENSIDE_CASTLING_RIGHTS:
        popCastlingRights();
        break;
    }
}

void Game::popNormal(Move move) {
    movePiece(move.end, move.start);
}

void Game::popCapture(Move move) {
    halfmoveCounter = pastHalfmoveCounters.top();
    pastHalfmoveCounters.pop();

    movePiece(move.end, move.start);
    board[move.end] = Piece(flip(turn), move.captured);

    switch (move.captured) {
        case KNIGHT: phase += 16; break;
        case BISHOP: phase += 16; break;
        case ROOK: phase += 32; break;
        case QUEEN: phase += 48; break;
    }
}

void Game::popShortCastling(Move move) {
    movePiece(move.end, move.start);
    movePiece(move.start - 1, move.end - 1);
}

void Game::popLongCastling(Move move) {
    movePiece(move.end, move.start);
    movePiece(move.start + 1, move.end + 2);
}

void Game::popPromotion(Move move) {
    movePiece(move.end, move.start);
    board[move.end] = Piece(turn, move.captured);
    board[move.start] = Piece(turn, PAWN);
}

void Game::popEnPassant(Move move) {
    movePiece(move.end, move.start);
    board[move.end+(turn ? -8 : 8)] = Piece(flip(turn), PAWN); 
}

void Game::movePiece(Square start, Square end) {
    board[end] = board[start];
    board[start] = Piece::empty();
}
