#include "piece.h"

enum class MoveType {
    NORMAL, 
    EN_PASSANT, 
    CASTLE, 
    PROMOTION
}; 

struct MoveRecord {

    int fromRow; 
    int fromCol; 
    int toRow; 
    int toCol; 

    Piece* movingPiece; 
    Piece* capturedPiece; // NULL if no capture

    MoveType type; // NORMAL, EN_PASSANT, CASTLE, PROMOTION

    bool movingPieceHadMoved; 
    bool rookHadMoved; 

    int rookFromCol;
    int rookToCol; 
    Piece* rook; 

}; 


