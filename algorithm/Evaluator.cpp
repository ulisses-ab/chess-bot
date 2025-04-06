#include "Evaluator.hpp"

const int Evaluator::midgameTable[][64] = {
    // EMPTY PIECE
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0
    },

    // PAWN
    {
         0,   0,   0,   0,   0,   0,   0,   0,
        50,  50,  50,  50,  50,  50,  50,  50,
        10,  10,  20,  30,  30,  20,  10,  10,
         5,   5,  10,  25,  25,  10,   5,   5,
         0,   0,   0,  20,  20,   0,   0,   0,
         5,  -5, -10,   0,   0, -10,  -5,   5,
         5,  10,  10, -20, -20,  10,  10,   5,
         0,   0,   0,   0,   0,   0,   0,   0
    },

    // KNIGHT
    {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20,   0,   0,   0,   0, -20, -40,
        -30,   0,  10,  15,  15,  10,   0, -30,
        -30,   5,  15,  20,  20,  15,   5, -30,
        -30,   0,  15,  20,  20,  15,   0, -30,
        -30,   5,  10,  15,  15,  10,   5, -30,
        -40, -20,   0,   5,   5,   0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
    },

    // BISHOP
    {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10,   0,   0,   0,   0,   0,   0, -10,
        -10,   0,   5,  10,  10,   5,   0, -10,
        -10,   5,   5,  10,  10,   5,   5, -10,
        -10,   0,  10,  10,  10,  10,   0, -10,
        -10,  10,  10,  10,  10,  10,  10, -10,
        -10,   5,   0,   0,   0,   0,   5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
    },

    // ROOK
    {
          0,   0,   0,   0,   0,   0,   0,   0,
          5,  10,  10,  10,  10,  10,  10,   5,
         -5,   0,   0,   0,   0,   0,   0,  -5,
         -5,   0,   0,   0,   0,   0,   0,  -5,
         -5,   0,   0,   0,   0,   0,   0,  -5,
         -5,   0,   0,   0,   0,   0,   0,  -5,
         -5,   0,   0,   0,   0,   0,   0,  -5,
          0,   0,   0,   5,   5,   0,   0,   0
    },

    // QUEEN
    {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10,   0,   0,  0,  0,   0,   0, -10,
        -10,   0,   5,  5,  5,   5,   0, -10,
         -5,   0,   5,  5,  5,   5,   0,  -5,
          0,   0,   5,  5,  5,   5,   0,  -5,
        -10,   5,   5,  5,  5,   5,   0, -10,
        -10,   0,   5,  0,  0,   0,   0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
    },

    // KING
    {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
         20,  20,   0,   0,   0,   0,  20,  20,
         20,  30,  10,   0,   0,  10,  30,  20
    }
};

const int Evaluator::endgameTable[][64] = {
    // EMPTY PIECE
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0
    },

    // PAWN
    {
         0,   0,   0,   0,   0,   0,   0,   0,
        80,  80,  80,  80,  80,  80,  80,  80,
        50,  50,  50,  50,  50,  50,  50,  50,
        30,  30,  30,  30,  30,  30,  30,  30,
        20,  20,  20,  20,  20,  20,  20,  20,
        10,  10,  10,  10,  10,  10,  10,  10,
         5,   5,   5,   5,   5,   5,   5,   5,
         0,   0,   0,   0,   0,   0,   0,   0
    },
    // KNIGHT
    {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20,   0,   5,   5,   0, -20, -40,
        -30,   0,  10,  15,  15,  10,   0, -30,
        -30,   5,  15,  20,  20,  15,   5, -30,
        -30,   0,  15,  20,  20,  15,   0, -30,
        -30,   5,  10,  15,  15,  10,   5, -30,
        -40, -20,   0,   5,   5,   0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
    },
    // BISHOP
    {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10,   0,   0,   0,   0,   0,   0, -10,
        -10,   0,   5,  10,  10,   5,   0, -10,
        -10,   5,   5,  10,  10,   5,   5, -10,
        -10,   0,  10,  10,  10,  10,   0, -10,
        -10,  10,  10,  10,  10,  10,  10, -10,
        -10,   5,   0,   0,   0,   0,   5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
    },
    // ROOK
    {
         0,   0,   0,   0,   0,   0,   0,   0,
         5,  10,  10,  10,  10,  10,  10,   5,
        -5,   0,   0,   0,   0,   0,   0,  -5,
        -5,   0,   0,   0,   0,   0,   0,  -5,
        -5,   0,   0,   0,   0,   0,   0,  -5,
        -5,   0,   0,   0,   0,   0,   0,  -5,
        -5,   0,   0,   0,   0,   0,   0,  -5,
         0,   0,   0,   0,   0,   0,   0,   0
    },
    // QUEEN
    {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10,   0,   0,  0,  0,   0,   0, -10,
        -10,   0,   5,  5,  5,   5,   0, -10,
         -5,   0,   5,  5,  5,   5,   0,  -5,
          0,   0,   5,  5,  5,   5,   0,  -5,
        -10,   5,   5,  5,  5,   5,   0, -10,
        -10,   0,   5,  0,  0,   0,   0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
    },
    // KING
    {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10,   0,   0, -10, -20, -30,
        -30, -10,  20,  30,  30,  20, -10, -30,
        -30, -10,  30,  40,  40,  30, -10, -30,
        -30, -10,  30,  40,  40,  30, -10, -30,
        -30, -10,  20,  30,  30,  20, -10, -30,
        -30, -30,   0,   0,   0,   0, -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50
    }
};

const int pieceValue[] = {0, 100, 290, 320, 490, 900, 60000};

int Evaluator::evaluateMove(Move move) {
    switch (move.type) {
        case Move::NORMAL: return evaluateNormal(move);
        case Move::CAPTURE: return evaluateCapture(move);
        case Move::SHORT_CASTLING: return evaluateShortCastling(move);
        case Move::LONG_CASTLING: return evaluateLongCastling(move);
        case Move::PROMOTION: return evaluatePromotion(move);
        case Move::EN_PASSANT: return evaluateEnPassant(move);
    }

    return -1;
};

int Evaluator::evaluateNormal(Move move) {
    return evaluateDisplacement(move.start, move.end);
}

int Evaluator::evaluateCapture(Move move) {
    return (
        + pieceValue[move.captured]
        + pst(move.end)
        + evaluateDisplacement(move.start, move.end)
    );
}

int Evaluator::evaluateShortCastling(Move move) {
    return (
        + evaluateDisplacement(move.start, move.end) 
        + evaluateDisplacement(move.end-1, move.start-1)
    );
}

int Evaluator::evaluateLongCastling(Move move) {
    return (
        + evaluateDisplacement(move.start, move.end) 
        + evaluateDisplacement(move.end+2, move.start+1)
    );
}

int Evaluator::evaluatePromotion(Move move) {
    return (
        + pieceValue[move.promoted] 
        - pieceValue[PAWN] 
        + pst(Piece(game.getTurn(), move.promoted), move.end) 
        + pst(Piece(game.enemy(), move.captured), move.end)
        - pst(move.start)
    );
}

int Evaluator::evaluateEnPassant(Move move) {
    return (
        + evaluateDisplacement(move.start, move.end) 
        + pieceValue[PAWN] 
        + pst(move.end - game.direction())
    );
}

int Evaluator::evaluateDisplacement(Square start, Square end) {
    Piece piece = game.getPiece(start);
   
    return pst(piece, end) - pst(piece, start);
}

int Evaluator::pst(Piece piece, Square square) {
    int phase = game.getPhase();

    int midgameValue = midgameTable[piece.type][relativePosition(square, piece.color).number];
    int endgameValue = endgameTable[piece.type][relativePosition(square, piece.color).number];

    return (phase * midgameValue + (255 - phase) * endgameValue) / 255;
}

int Evaluator::pst(Square square) {
    Piece piece = game.getPiece(square);

    return pst(piece, square);
}

Square Evaluator::relativePosition(Square square, Color color) {
    return color == WHITE ? Square(63 - square.number) : square;
}

int Evaluator::evaluateTerminalState() {
    if(game.getHalfmoveCounter() == 50) {
        return 0;
    }

    return checkDetector.isKingInCheck() ? -60000 : 0; 
}