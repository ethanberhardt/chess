#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn: public Piece{
    public: 
        Pawn(const string& color, bool hasMoved = false); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 
        void notifyMoved() override; 

    private: 
        bool hasMoved; 
}; 


#endif // PAWN_H