#ifndef KING_H
#define KING_H

#include "piece.h"

class King: public Piece{
    public: 
        King(const string& color, bool hasMoved = false); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 
        void notifyMoved() override;
        bool getHasMoved();
    private: 
        bool hasMoved; 
}; 


#endif // KING_H