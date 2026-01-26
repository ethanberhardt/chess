#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook: public Piece{
    public: 
        Rook(const string& color); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 

    private: 
        //
}; 


#endif // ROOK_H