#include "./../headers/MoveGenerator.hpp"
#include <iostream>

std::vector<Board>mv;

int ptype;

bool MoveGenerator::turn;
Board MoveGenerator::board;
Board MoveGenerator::newBoard;
std::vector<Board> *MoveGenerator::moves;

std::vector<Board> *MoveGenerator::generatePossibleMoves(const Board &b) {
    moves = new std::vector<Board>();
    board = b;
    turn = board.getTurn();

    newBoard = board;
    newBoard.enPassantTarget = 64;
    newBoard.halfmoveClock++;
    newBoard.flags ^= 1;

    for(int i = 0; i < 64; i++) {
        int piece = board.getPiece(i);

        if(piece == NONE) 
            continue;

        if((piece >> 3) ^ turn) 
            continue;

        ptype = piece;

        switch(piece & 0b0111) {
            case PAWN:
                generatePawnMoves(i);
                break;
            
            case KNIGHT:
                generateKnightMoves(i);
                break;

            case BISHOP:
                generateBishopMoves(i);
                break;
            
            case ROOK:
                generateRookMoves(i);
                break;

            case QUEEN:
                generateQueenMoves(i);
                break;
            
            case KING:
                generateKingMoves(i);
                break;
        }
    }

    return moves;
}

void MoveGenerator::generatePawnMoves(int square) {
    int direction = turn ? 8 : -8;
    int front = square + direction;

    newBoard.halfmoveClock = 0;

    if(front / 8 == 0 || front / 8 == 7) {
        generatePawnPromotions(square, front);
        newBoard.halfmoveClock = board.halfmoveClock + 1;
        return;
    }

    if(square / 8 == 3 + turn && (front + 1 == board.enPassantTarget || front - 1 == board.enPassantTarget)) {
        pushEnPassant(square);
    }

    if(square / 8 == (turn ? 1 : 6) && board.getPiece(front + direction) == NONE) {
        pushDoubleMove(square, front + direction);
    }

    trySquare(square, front, false);
    if(front % 8 != 7) 
        trySquare(square, front + 1, true, false);
    if(front % 8 != 0) 
        trySquare(square, front - 1, true, false);

    newBoard.halfmoveClock = board.halfmoveClock + 1;
}

void MoveGenerator::generatePawnPromotions(int square, int front) {
    if(board.getPiece(front) == NONE) {
        pushPawnPromotion(square, front, KNIGHT);
        pushPawnPromotion(square, front, QUEEN);
    }

    int diag1 = front + 1;
    int diagPiece1 = board.getPiece(diag1);
    if(front % 8 != 7 && diagPiece1 != NONE && (diagPiece1 >> 3) ^ turn) {
        pushPawnPromotion(square, diag1, KNIGHT);
        pushPawnPromotion(square, diag1, QUEEN);
    }

    int diag2 = front - 1;
    int diagPiece2 = board.getPiece(diag2);
    if(front % 8 == 0 && diagPiece2 != NONE && (diagPiece2 >> 3) ^ turn) {
        pushPawnPromotion(square, diag2, KNIGHT);
        pushPawnPromotion(square, diag2, QUEEN);
    }
}

void MoveGenerator::generateKnightMoves(int square) {
    int displacements[] = {17, 15, 6, 10, -6, -10, -17, -15};
    int horizontalDisplacements[] = {1, -1, -2, 2, 2, -2, -1, 1};
    tryDisplacements(square, displacements, horizontalDisplacements, 8);
}

void MoveGenerator::generateBishopMoves(int square) {
    trySquareLoop(square, 7);
    trySquareLoop(square, 9);
    trySquareLoop(square, -7);
    trySquareLoop(square, -9);
}

void MoveGenerator::generateRookMoves(int square) {
    switch (square)
    {
    case 0:
        newBoard.removeCastlingRight(WHITE, 0);
        break;

    case 7:
        newBoard.removeCastlingRight(WHITE, 1);
        break;

    case 56:
        newBoard.removeCastlingRight(BLACK, 0);
        break;

    case 63:
        newBoard.removeCastlingRight(BLACK, 1);
        break;
    }

    trySquareLoop(square, 1);
    trySquareLoop(square, -1);
    trySquareLoop(square, 8);
    trySquareLoop(square, -8);

    newBoard.flags = board.flags ^ 1;
}

void MoveGenerator::generateQueenMoves(int square) {
    trySquareLoop(square, 1);
    trySquareLoop(square, -1);
    trySquareLoop(square, 8);
    trySquareLoop(square, -8);
    trySquareLoop(square, 7);
    trySquareLoop(square, 9);
    trySquareLoop(square, -7);
    trySquareLoop(square, -9);
}

void MoveGenerator::generateKingMoves(int square) {
    if(board.getCastlingRight(turn, 0)) {
        int kingSquare = turn ? 3 : 59;

        if(!board.getPiece(kingSquare - 1) && !board.getPiece(kingSquare - 2)) {
            pushCastling(square, kingSquare - 3);
        }
    }

    if(board.getCastlingRight(turn, 1)) {
        int kingSquare = turn ? 3 : 59;

        if(!board.getPiece(kingSquare + 1) && !board.getPiece(kingSquare + 2) && !board.getPiece(kingSquare + 3)) {
            pushCastling(square, kingSquare + 4);
        }
    }

    newBoard.removeCastlingRights(turn);

    int displacements[] = {1, 9, 8, 7, -1, -9, -8, -7};
    int horizontalDisplacements[] = {1, 1, 0, -1, -1, -1, 0, 1};
    tryDisplacements(square, displacements, horizontalDisplacements, 8);

    newBoard.flags = board.flags ^ 1;
}

void MoveGenerator::trySquare(int startSquare, int endSquare, bool capture, bool nonCapture) {
    int piece = board.getPiece(endSquare);

    if(piece == NONE) {
        if(nonCapture)
            pushMove(startSquare, endSquare);
    }
    else if((((piece & 0b1000) >> 3) ^ turn) && capture) {
        pushMove(startSquare, endSquare);
    }
}

void MoveGenerator::tryDisplacements(int startSquare, int displacements[], int horizontalDisplacements[], int n) {
    for(int i = 0; i < n; i++) {
        int displacement = displacements[i];
        int endSquare = startSquare + displacement; 

        int horizontalMove = horizontalDisplacements[i];
        int leftLimit = 8 + horizontalMove;
        int rightLimit = horizontalMove - 1;

        if(endSquare >= 64 || endSquare < 0 || endSquare % 8 >= leftLimit || endSquare % 8 <= rightLimit) {
            continue;
        }

        trySquare(startSquare, endSquare);
    }
}

void MoveGenerator::trySquareLoop(int startSquare, int step) {
    int horizontalLimit = (32 + step) % 8;

    for(
        int endSquare = startSquare + step; 
        endSquare > 0 && endSquare < 64 && endSquare % 8 != 0 && endSquare % 8 != 7; 
        endSquare += step
    ) {
        int piece = board.getPiece(endSquare);

        if(piece != NONE) {
            trySquare(startSquare, endSquare);
            break;
        }
        
        pushMove(startSquare, endSquare);
    }
}

bool notSafe(int x) {
    return x < 0 || x >= 64;
} 

void MoveGenerator::pushMove(int start, int end) {
    if(notSafe(start) || notSafe(end)) {
        std::cout << "ALERTM ";
        board.print();
        std::cout << start << " " << end << " " << ptype << " ";
    }

    moves->push_back(newBoard);

    if(board.getPiece(end) != NONE) {
        moves->back().halfmoveClock = 0;
    }

    moves->back().setPiece(start, NONE);
    moves->back().setPiece(end, board.getPiece(start));
}

void MoveGenerator::pushCastling(int start, int end) {
    if(notSafe(start) || notSafe(end)) {
        std::cout << "ALERTC ";
        board.print();
        std::cout << start << " " << end << " ";
    }

    moves->push_back(newBoard);

    moves->back().removeCastlingRights(turn);
    moves->back().setPiece(end, KING | (turn << 3));
    moves->back().setPiece(start, ROOK | (turn << 3));
}

void MoveGenerator::pushDoubleMove(int start, int end) {
    if(notSafe(start) || notSafe(end)) {
        std::cout << "ALERTD ";
        board.print();
        std::cout << start << " " << end << " ";
    }

    moves->push_back(newBoard);

    moves->back().setPiece(end, PAWN | (turn << 3));
    moves->back().setPiece(start, NONE);
    moves->back().enPassantTarget = start + (turn ? 8 : -8);
}

void MoveGenerator::pushEnPassant(int start) {
    if(notSafe(start) || notSafe(board.enPassantTarget)) {
        std::cout << "ALERTE ";
        board.print();
        std::cout << start << " " << board.enPassantTarget << " ";
    }

    moves->push_back(newBoard);

    moves->back().setPiece(board.enPassantTarget, PAWN | (turn << 3));
    moves->back().setPiece(start, NONE);
    moves->back().setPiece(board.enPassantTarget + (turn ? -8 : 8), NONE);
}

void MoveGenerator::pushPawnPromotion(int start, int end, int promotion) {
    if(notSafe(start) || notSafe(end)) {
        std::cout << "ALERTP ";
        board.print();
        std::cout << start << " " << end << " ";
    }

    moves->push_back(newBoard);

    moves->back().setPiece(end, promotion | (turn << 3));
    moves->back().setPiece(start, NONE);
}