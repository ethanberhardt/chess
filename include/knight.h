#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight: public Piece{
    public: 
        Knight(const string& color); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 

    private: 
    // 
}; 


#endif // KNIGHT_H