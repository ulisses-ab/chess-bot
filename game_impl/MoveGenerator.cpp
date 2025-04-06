#include "MoveGenerator.hpp"
#include "../util.hpp"

std::vector<Move> *MoveGenerator::generatePossibleMoves() {
    moves = new std::vector<Move>();

    if(game.getHalfmoveCounter() == 50) {
        return moves;
    }

    Square kingPosition = -1;
    std::vector<Square> enemySquares = {};

    for(Square i(0); i.isSafe(); i += 1) {
        Piece piece = game.getPiece(i);

        if(piece.hasColor(game.enemy())) {
            enemySquares.push_back(i);
            continue;
        }

        switch(piece.type) {
            case PAWN: generatePawnMoves(i); break;
            case KNIGHT: generateKnightMoves(i); break;
            case BISHOP: generateBishopMoves(i); break;
            case ROOK: generateRookMoves(i); break;
            case QUEEN: generateQueenMoves(i); break;
            case KING: 
                kingPosition = i;
                generateKingMoves(i); 
                break;
        }
    }

    invalidDetector.filterMoves((*moves), enemySquares, kingPosition);
    
    return moves;
}

void MoveGenerator::generatePawnMoves(Square square) {
    Square front = square + game.direction();

    if(front.row == game.relativeRow(7)) {
        generatePawnPromotions(square, front);
        return;
    }

    if(square.row == game.relativeRow(4) && abs((front - game.getEnPassantTarget()).col) == 1) {
        push(Move::enPassant(square, game.getEnPassantTarget()));
    }

    if(square.row == game.relativeRow(1) && game.getPiece(front).isEmpty() && game.getPiece(front+game.direction()).isEmpty()) {
        push(Move::pawnDoubleMove(square, game.ally()));
    }

    tryNonCapturingMove(square, front, Move::RESET_HALFMOVE_COUNTER);
    if(front.col != 7) tryCapturingMove(square, front + 1, Move::RESET_HALFMOVE_COUNTER);
    if(front.col != 0) tryCapturingMove(square, front - 1, Move::RESET_HALFMOVE_COUNTER);
}

void MoveGenerator::generatePawnPromotions(Square square, Square front) {
    if(game.getPiece(front).isEmpty()) {
        push(Move::promotion(square, front, QUEEN));
        push(Move::promotion(square, front, KNIGHT));
    }

    Square diag1 = front + 1;
    Piece piece = game.getPiece(diag1);
    if(front.col != 7 && piece.hasColor(game.enemy())) {
        push(Move::promotion(square, diag1, QUEEN, piece.type));
        push(Move::promotion(square, diag1, KNIGHT, piece.type));
    }

    Square diag2 = front - 1;
    piece = game.getPiece(diag2);
    if(front.col == 0 && game.getPiece(diag2).hasColor(game.enemy())) {
        push(Move::promotion(square, diag2, QUEEN, piece.type));
        push(Move::promotion(square, diag2, KNIGHT, piece.type));
    }
}

void MoveGenerator::generateKnightMoves(Square square) {
    constexpr std::array<Offset, 8> offsets = {{{2, 1}, {2, -1}, {1, -2}, {1, 2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}}};
    tryOffsets(square, offsets);
}

void MoveGenerator::generateBishopMoves(Square square) {
    tryLoop(square, {1, 1});
    tryLoop(square, {1, -1});
    tryLoop(square, {-1, 1});
    tryLoop(square, {-1, -1});
}

void MoveGenerator::generateRookMoves(Square square) {
    Move::MoveAction action = Move::NO_ACTION;

    if(game.relativeRow(square) == 0) {
        switch(square.col) {
            case 0: action = Move::REMOVE_KINGSIDE_CASTLING_RIGHTS; break;
            case 7: action = Move::REMOVE_QUEENSIDE_CASTLING_RIGHTS; break;
        }
    }

    tryLoop(square, {1, 0}, action);
    tryLoop(square, {-1, 0}, action);
    tryLoop(square, {0, 1}, action);
    tryLoop(square, {0, -1}, action);
}

void MoveGenerator::generateQueenMoves(Square square) {
    tryLoop(square, {1, 1});
    tryLoop(square, {1, -1});
    tryLoop(square, {-1, 1});
    tryLoop(square, {-1, -1});
    tryLoop(square, {1, 0});
    tryLoop(square, {-1, 0});
    tryLoop(square, {0, 1});
    tryLoop(square, {0, -1});
}

void MoveGenerator::generateKingMoves(Square square) {
    if(game.relativeRow(square) == 0) {
        generateCastlings(square);
    }

    constexpr std::array<Offset, 8> offsets = {{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}};
    tryOffsets(square, offsets, Move::REMOVE_CASTLING_RIGHTS);
}

void MoveGenerator::generateCastlings(Square square) {
    CastlingRights rights = game.getCastlingRights(game.ally());

    if(rights.kingside) {
        if(game.getPiece(square - 1).isEmpty() && game.getPiece(square - 2).isEmpty()) {
            push(Move::shortCastling(game.ally()));
        }
    }

    if(rights.queenside) {
        if(game.getPiece(square + 1).isEmpty() && game.getPiece(square + 2).isEmpty() && game.getPiece(square + 3).isEmpty()) {
            push(Move::longCastling(game.ally()));
        }
    }
}

void MoveGenerator::tryRegularMove(Square start, Square end, Move::MoveAction action) {
    tryCapturingMove(start, end, action);
    tryNonCapturingMove(start, end, action);
}

void MoveGenerator::tryCapturingMove(Square start, Square end, Move::MoveAction action) {
    Piece p = game.getPiece(end);

    bool cond = p.hasColor(game.enemy());
    if(game.getPiece(end).hasColor(game.enemy())) {
        push(Move::capture(start, end, game.getPiece(end).type, action));
    }
}

void MoveGenerator::tryNonCapturingMove(Square start, Square end, Move::MoveAction action) {
    if(game.getPiece(end).isEmpty()) {
        push(Move::normal(start, end, action));
    }
}

template <size_t n>
void MoveGenerator::tryOffsets(Square square, const std::array<Offset, n> &offsets, Move::MoveAction action) {
    Utils::loop(square, offsets, [this, square, action](Square current) {
        tryRegularMove(square, current, action);
        return CONTINUE_LOOP;
    });
}

void MoveGenerator::tryLoop(Square start, Offset offset, Move::MoveAction action) {
    Square end = start;

    Utils::loop(start, offset, [this, start, action](Square current) {
        Piece piece = game.getPiece(current);

        if(piece.notEmpty()) {
            tryCapturingMove(start, current, action);
            return BREAK_LOOP;
        }

        push(Move::normal(start, current, action));
        return CONTINUE_LOOP;
    });
}

void MoveGenerator::push(Move m) {
    moves->push_back(m);
} 