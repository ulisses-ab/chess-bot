#include "./../headers/Board.hpp"
#include <iostream>

Board::Board() {
    turn = WHITE;
    totalHalfmoves = 0;
    kingHasMoved = false;
    blackKRookHasMoved = false;
    blackQRookHasMoved = false;
    whiteKRookHasMoved = false;
    whiteQRookHasMoved = false;

    Piece startingBoard[] = {
        WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_KING, WHITE_QUEEN, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK,
        WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN,
        BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE,
        BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE,
        BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE,
        BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE, BLACK_NONE,
        BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN,
        BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_KING, BLACK_QUEEN, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK,
    };
 
    for(int i = 0; i < 64; i++) {
        board[i] = startingBoard[i];
    }
    
    pieceSquares[WHITE_PAWN] = {8, 9, 10, 11, 12, 13, 14, 15};
    pieceSquares[BLACK_PAWN] = {55, 54, 53, 52, 51, 50, 49, 48};

    pieceSquares[WHITE_KNIGHT] = {1, 6};
    pieceSquares[BLACK_KNIGHT] = {62, 57};
    
    pieceSquares[WHITE_BISHOP] = {2, 5};
    pieceSquares[BLACK_BISHOP] = {61, 58};

    pieceSquares[WHITE_ROOK] = {0, 7};
    pieceSquares[BLACK_ROOK] = {63, 56};

    pieceSquares[WHITE_QUEEN] = {4};
    pieceSquares[BLACK_QUEEN] = {60};

    pieceSquares[WHITE_KING] = {3};
    pieceSquares[BLACK_KING] = {59};
}

void Board::print() {
    char pieceChar[] = {'p', 'P', 'n', 'N', 'b', 'B', 'r', 'R', 'q', 'Q', 'k', 'K', '.', '.'};

    for(int i = 56; i >= 0; i -= 8) {
        for(int j = 7; j >= 0; j--) {
            std::cout << pieceChar[board[i+j].raw] << " ";
        }
        std::cout << '\n';
    }

    std::cout << '\n';
}

void Board::printDetails() {
    char pieceChar[] = {'p', 'P', 'n', 'N', 'b', 'B', 'r', 'R', 'q', 'Q', 'k', 'K', '.', '.'};

    print();

    for(int i = 0; i < 12; i++) {
        std::cout << pieceChar[i] << " ";
        for(auto piece : pieceSquares[i]) {
            std::cout << piece << " ";
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    std::stack<std::pair<int, int>> temp;

    while (!enPassantTargetStack.empty()) {
        std::cout << enPassantTargetStack.top().first << "/" << enPassantTargetStack.top().second << " ";
        temp.push(enPassantTargetStack.top());
        enPassantTargetStack.pop();
    }

    while (!temp.empty()) {
        enPassantTargetStack.push(temp.top());
        temp.pop();
    }

    std::cout << '\n';

    std::stack<int> temp2;

    while (!halfmoveResetStack.empty()) {
        std::cout << halfmoveResetStack.top() << " ";
        temp2.push(halfmoveResetStack.top());
        halfmoveResetStack.pop();
    }

    while (!temp2.empty()) {
        halfmoveResetStack.push(temp2.top());
        temp2.pop();
    }

    std::cout << '\n';

    std::cout << std::endl << totalHalfmoves << " " << turn << " " 
    << kingHasMoved << blackKRookHasMoved << blackQRookHasMoved << whiteKRookHasMoved << whiteQRookHasMoved
    << std::endl << std::endl;
}

void Board::pushMove(Move move) {
    Piece piece = board[move.start];
    board[move.start].type = NONE;
    int end = move.end;
    int capturedSquare = move.end;

    totalHalfmoves++;

    if(piece.type == PAWN) {
        halfmoveResetStack.push(totalHalfmoves);
        int moveOffset = move.end - move.start;
        int direction;
        
        if(move.flag) {
            direction = (turn ? 8 : -8);
            if(moveOffset % 8 != 0 && move.captured == NONE) {
                board[move.end - direction].type = NONE;
                capturedSquare = move.end - direction;

                popNumber(pieceSquares[WHITE_PAWN ^ turn], capturedSquare);
            }
            else {
                end = move.start + direction + (move.end & 0b11) - 1;
                capturedSquare = end;

                piece.raw = ((move.end & 0b11100) >> 1) | turn;

                pieceSquares[piece.raw].push_back(end);
                popNumber(pieceSquares[BLACK_PAWN | turn], move.start);
            }
        }
        else if(abs(moveOffset) == 16) {
            enPassantTargetStack.push({move.start + (turn ? 8 : -8), totalHalfmoves});
        }
    }  
    else if(move.flag) {
        if(piece.type == ROOK) {
            switch (move.start)
            {
            case 0:
                whiteKRookHasMoved = true;
                break;
            case 7:
                whiteQRookHasMoved = true;
                break;
            case 56:
                blackKRookHasMoved = true;
                break;
            case 63:
                blackQRookHasMoved = true;
                break;        
            }
        }
        else {    
            kingHasMoved = true;

            if(board[move.end].color == turn) {
                board[move.start].raw = BLACK_ROOK | turn;
    
                for(auto& square : pieceSquares[BLACK_ROOK | turn]) {
                    if(square == move.end) {
                        square = move.start;
                        break;
                    }
                }
            }
        } 
    }
    
    turn = !turn;
    board[end] = piece;

    for(auto& square : pieceSquares[piece.raw]) {
        if(square == move.start) {
            square = move.end;
            break;
        }
    }

    if(move.captured == NONE) 
        return;

    if(halfmoveResetStack.top() != totalHalfmoves) {
        halfmoveResetStack.push(totalHalfmoves);
    }


    popNumber(pieceSquares[(move.captured << 1) | turn], capturedSquare);
}

void Board::popNumber(std::vector<int>& v, int number) {
    int n = v.size();

    for(int i = 0; i < n; i++) {
        if(v[i] == number) {
            std::swap(v[i], v[n-1]);
            v.pop_back();
            break;
        }
    }
}

void Board::popMove(Move move) {
    totalHalfmoves--;

    Piece piece = board[move.end];
    Piece captured{0};
    captured.color = turn;
    captured.type = move.captured;

    if(move.flag) {
        if(move.start / 8 == (turn ? 1 : 6)) {
            piece.raw = WHITE_PAWN ^ turn;

            pieceSquares[WHITE_PAWN ^ turn].push_back(move.start);
            move.end = move.start + (turn ? -8 : 8) + (move.end & 0b11) - 1;

            popNumber(pieceSquares[board[move.end].raw], move.end);
        }
        else if (piece.type == PAWN) {
            int capturedPawnSquare = move.end + (turn ? 8 : -8);
            board[capturedPawnSquare].raw = BLACK_PAWN | turn;
            pieceSquares[BLACK_PAWN | turn].push_back(capturedPawnSquare);
        }
        else if (piece.type == KING) {
            int displacement = move.end - move.start;
            kingHasMoved = false;

            if(displacement == -3 || displacement == 4) {
                board[move.start] = piece;
                board[move.end].raw = WHITE_ROOK ^ turn;

                for(auto& square : pieceSquares[WHITE_ROOK ^ turn]) {
                    if(square == move.start) {
                        square = move.end;
                        break;
                    }
                }

                pieceSquares[WHITE_KING ^ turn][0] = move.start;

                return;
            }
        }
        else {
            switch (move.start)
            {
            case 0:
                whiteKRookHasMoved = false;
                break;
            case 7:
                whiteQRookHasMoved = false;
                break;
            case 63:
                blackQRookHasMoved = false;
                break;
            case 56:
                blackKRookHasMoved = false;
                break;
            }
        }
    }

    if(piece.type == PAWN) {
        halfmoveResetStack.pop();
        int moveOffset = move.end - move.start;
        if(abs(moveOffset) == 16) {
            enPassantTargetStack.pop();
        }
    }

    board[move.start] = piece;
    board[move.end] = captured;

    turn = !turn;

    for(auto& square : pieceSquares[piece.raw]) {
        if(square == move.end) {
            square = move.start;
            break;
        }
    }

    if(captured.type == NONE) {
        return;
    }

    if(!halfmoveResetStack.empty() && halfmoveResetStack.top() == totalHalfmoves + 1) {
        halfmoveResetStack.pop();
    }

    pieceSquares[captured.raw].push_back(move.end);
}

std::vector<Move>* Board::possibleMoves() {
    std::vector<Move>* moves = new std::vector<Move>();

    for(auto square : pieceSquares[WHITE_PAWN & turn])
        addPossiblePawnMoves(moves, square);

    for(auto square : pieceSquares[WHITE_KNIGHT & turn])
        addPossibleKnightMoves(moves, square);

    for(auto square : pieceSquares[WHITE_BISHOP & turn])
        addPossibleBishopMoves(moves, square);

    for(auto square : pieceSquares[WHITE_ROOK & turn])
        addPossibleRookMoves(moves, square);

    for(auto square : pieceSquares[WHITE_QUEEN & turn])
        addPossibleQueenMoves(moves, square);

    for(auto square : pieceSquares[WHITE_KING & turn])
        addPossibleKingMoves(moves, square);

    return moves;
}

void Board::addPossiblePawnMoves(std::vector<Move>* moves, int square) {
    int direction = (turn * 16) - 8;
    int front = square + direction;
    bool leftIsFree = front % 8 != 7;
    bool rightIsFree = front % 8 != 0;

    if(board[front].type == NONE) {
        moves->push_back(Move(square, front, NONE, 0));
    } 

    if(square / 8 == (turn ? 1 : 6) && board[front + direction].type == NONE) {
        moves->push_back(Move(square, front + direction, NONE, 0));
    }

    if(leftIsFree && board[front + 1].color != turn && board[front + 1].type != NONE) {
        moves->push_back(Move(square, front + 1, board[front + 1].type, 0));
    }

    if(rightIsFree && board[front - 1].color != turn && board[front - 1].type != NONE) {
        moves->push_back(Move(square, front - 1, board[front - 1].type, 0));
    }

    if(square / 8 == (turn ? 4 : 5) && enPassantTargetStack.top().second == totalHalfmoves) {
        if(leftIsFree && (front + 1) == enPassantTargetStack.top().first) {
            moves->push_back(Move(square, front + 1, 0, 1));
        }
        else if(rightIsFree && (front - 1) == enPassantTargetStack.top().first) {
            moves->push_back(Move(square, front - 1, 0, 1));
        }
    }
}

void Board::addPossibleKnightMoves(std::vector<Move>* moves, int square) {
    
}

void Board::addPossibleBishopMoves(std::vector<Move>* moves, int square) {

}

void Board::addPossibleRookMoves(std::vector<Move>* moves, int square) {

}

void Board::addPossibleQueenMoves(std::vector<Move>* moves, int square) {

}

void Board::addPossibleKingMoves(std::vector<Move>* moves, int square) {

}