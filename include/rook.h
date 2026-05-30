#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook: public Piece{
    public: 
        Rook(const string& color, bool hasMoved = false); 
        bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) override; 
        bool getHasMoved() override; 
        void notifyMoved() override; 
        void setHasMoved(bool flag) override; 
    private: 
        bool hasMoved; 
}; 


#endif // ROOK_H