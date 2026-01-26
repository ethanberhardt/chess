#ifndef KING_H
#define KING_H

#include "piece.h"

class King: public Piece{
    public: 
        King(const string& color); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 

    private: 
        //
}; 


#endif // KING_H