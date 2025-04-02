#include "./../headers/Board.hpp"
#include "./../headers/MoveGenerator.hpp"
#include "./../headers/Evaluator.hpp"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

const char Board::pieceChar[] = {'.', 'p', 'n', 'b', 'r', 'q', 'k', ' ', ' ', 'P', 'N', 'B', 'R', 'Q', 'K'};

Board::Board() {
    flags = 0b0011111;
    enPassantTarget = 64;
    halfmoveClock = 0;

    int startingBoard[] = {
        WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_KING, WHITE_QUEEN, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK,
        WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN,
        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN,
        BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_KING, BLACK_QUEEN, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK,
    };

    for(int i = 0; i < 64; i++) {
        setPiece(i, startingBoard[i]);
    }
}
 
Board::Board(std::string fen) {
    for(int i = 0; i < 64; i++) {
        setPiece(i, NONE);
    }

    int square = 63;

    int i = 0;
    for(; true; i++, square--) {
        char c = fen[i];

        if(c == ' ') {
            break;
        }

        switch (c) {
        case '/':
            square++;
            continue;
        case 'P':
            setPiece(square, WHITE_PAWN);
            continue;
        case 'p':
            setPiece(square, BLACK_PAWN);
            continue;
        case 'N':
            setPiece(square, WHITE_KNIGHT);
            continue;
        case 'n':
            setPiece(square, BLACK_KNIGHT);
            continue;
        case 'B':
            setPiece(square, WHITE_BISHOP);
            continue;
        case 'b':
            setPiece(square, BLACK_BISHOP);
            continue;
        case 'R':
            setPiece(square, WHITE_ROOK);
            continue;
        case 'r':
            setPiece(square, BLACK_ROOK);
            continue;
        case 'Q':
            setPiece(square, WHITE_QUEEN);
            continue;
        case 'q':
            setPiece(square, BLACK_QUEEN);
            continue;
        case 'K':
            setPiece(square, WHITE_KING);
            continue;
        case 'k':
            setPiece(square, BLACK_KING);
            continue;
        default:
            square -= c - '1';
        }
    }

    i++;
    flags = fen[i] == 'w';
    i++;

    for(; fen[i] != ' '; i++) {
        switch(fen[i]) {
            case 'w':
                flags |= 1;
                break;
            case 'K':
                flags |= 0b01000;
                break;
            case 'Q':
                flags |= 0b10000;
                break;
            case 'k':
                flags |= 0b00010;
                break;
            case 'q':
                flags |= 0b00100;
                break;
        }
    }

    i++;
    if(fen[i] == '-') {
        enPassantTarget = 64;
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
        halfmoveClock = fen[i] - '0'; 
    }
    else {
        halfmoveClock = fen[i] - '0';
        halfmoveClock *= 10;
        halfmoveClock += fen[i+1] - '0';
    }
}

void Board::print(bool details) const {
    for(int row = 7; row >= 0; row--) {
        for(int column = 7; column >= 0; column--) {
            cout << pieceChar[getPiece(row, column)] << " ";
        }
        cout << "\n";
    }

    cout << "\n";

    if(details) {
        cout << getTurn() << " " <<
        getCastlingRight(BLACK, 1) << getCastlingRight(BLACK, 0) << " " << 
        getCastlingRight(WHITE, 1) << getCastlingRight(WHITE, 0) << " " <<
        int(enPassantTarget) << " " << int(halfmoveClock) << "\n" << toFen() << "\n";
    } 
}

vector<Board> *Board::generatePossibleMoves() const {
    return MoveGenerator::generatePossibleMoves(*this);
}

int Board::evaluate() const {
    return Evaluator::evaluate(*this);
}

std::string Board::toFen() const {
    std::string output = "";

    for(int row = 7; row >= 0; row--) {
        int noneCounter = 0;
        std::string rowString = "";

        for(int column = 7; column >= 0; column--) {
            int piece = getPiece(row, column);

            if(piece == NONE) {
                noneCounter++;
                continue;
            }

            if(noneCounter > 0) {
                rowString += std::to_string(noneCounter);
                noneCounter = 0;
            }

            rowString += pieceChar[piece];
        }

        if(noneCounter > 0) {
            rowString += std::to_string(noneCounter);
        }

        output += rowString + (row == 0 ? " " : "/");
    }

    output += (getTurn() ? 'w' : 'b');
    output += " ";
    output +=(getCastlingRight(WHITE, 0) ? "K" : "");
    output +=(getCastlingRight(WHITE, 1) ? "Q" : "");
    output +=(getCastlingRight(BLACK, 0) ? "k" : "");
    output +=(getCastlingRight(BLACK, 1) ? "q" : "");

    if(output[output.size() - 1] == ' ') {
        output += '-';
    }

    output += ' ';

    if(enPassantTarget == 64) {
        output += '-';
    }
    else {
        int row = enPassantTarget / 8;
        int col = enPassantTarget % 8;
        output += ('h' - col);
        output += std::to_string(row + 1);
    }
    
    output += ' ';

    output += std::to_string(halfmoveClock);

    return output;
}