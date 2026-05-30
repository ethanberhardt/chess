#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen: public Piece{
    public: 
        Queen(const string& color); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 

    private: 
        //
}; 


#endif // QUEEN_H