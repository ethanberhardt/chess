#include "piece.h"

enum class MoveType {
    NORMAL, 
    EN_PASSANT, 
    CASTLE, 
    PROMOTION
}; 

struct MoveRecord {

    int fromRow, fromCol; 
    int toRow, toCol; 

    Piece* movingPiece = nullptr; 

    Piece* capturedPiece = nullptr; // NULL if no capture
    int capturedRow = -1; // Data needed for en passant
    int capturedCol = -1; // Data needed for en passant

    Piece* previousLastDoubleStepPawn = nullptr; // NULL if no pawn moved 2 squares
    int previousLastDoubleStepRow = -1; 
    int previousLastDoubleStepCol = -1;

    MoveType type = MoveType::NORMAL; // NORMAL, EN_PASSANT, CASTLE, PROMOTION

    bool movingPieceHadMoved = false; // for king/rook/pawn

    Piece* rook = nullptr; // rook involved in castling
    bool rookHadMoved = false; // only relevant for castling
    int rookFromCol = -1; // for castling
    int rookToCol = -1; 
    
    Piece* promotedPiece = nullptr; // promoted piece 
    int previousHalfMoveClock = 0; // fifty-move rule state
}; 


