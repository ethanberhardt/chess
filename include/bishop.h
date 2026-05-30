#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop: public Piece{
    public: 
        Bishop(const string& color); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 

    private: 
    // 
}; 




#endif // BISHOP_H