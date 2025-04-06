#include "InvalidMoveDetector.hpp"

#include <algorithm>
#include <utility>
#include "../util/util.hpp"

void InvalidMoveDetector::filterMoves(std::vector<Move> &moves, std::vector<Square> &enemySquares, Square kingP) {
    std::vector<Square> pinnedPieces, checkStoppers;
    bool kingInCheck;
    kingInCheck = checkDetector.findPinnedAndCheck(pinnedPieces, checkStoppers);

    std::vector<Square> unsafeSquares;
    unsafeRecorder.record(enemySquares, unsafeSquares);
    
    filterMovesWhichCauseACheck(moves, pinnedPieces, unsafeSquares);

    if(kingInCheck) {
        filterMovesWhichDontStopCheck(moves, checkStoppers);
    }
}

void InvalidMoveDetector::filterMovesWhichCauseACheck(std::vector<Move> &moves, std::vector<Square> &pinnedPieces, std::vector<Square> &unsafeSquares) {
    std::sort(unsafeSquares.begin(), unsafeSquares.end());
    std::sort(pinnedPieces.begin(), pinnedPieces.end());
   
    for(int i = 0; i < moves.size(); i++) {
        if(
            isIn(pinnedPieces, moves[i].start) ||
            (game.getPiece(moves[i].start).type == KING && isIn(unsafeSquares, moves[i].end))
        ) {
            pop<Move>(moves, i);
            i--;
        }
    }  
}

void InvalidMoveDetector::filterMovesWhichDontStopCheck(std::vector<Move> &moves, std::vector<Square> &checkStoppers) {
    for(int i = 0; i < moves.size(); i++) {
        if(
            game.getPiece(moves[i].start).type != KING &&
            !isIn(checkStoppers, moves[i].end) 

        ) {
            pop<Move>(moves, i);
            i--;
        }
    }
}