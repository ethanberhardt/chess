#include "pawn.h"

using namespace std; 

Pawn::Pawn(const string& color, bool hasMoved): Piece(color, 'P'), hasMoved(hasMoved) {}

bool Pawn::isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol){

    int deltaRow = fromRow - toRow; // change in rows [moving from left to right is +]
    int deltaCol = fromCol - toCol; // change in cols [moving from up to down is +]

    // White pawns move different from black pawns
    int direction = (this->getColor() == "white") ? -1 : 1; 

    if (deltaCol == 0){
        if (deltaRow == direction) return true; // single step 
        if (deltaRow == direction * 2) return true; // double step 
    }

    if (deltaRow == direction && abs(deltaCol) == 1) return true; // diagonal [step] capture

    return false; 
}

void Pawn::notifyMoved() {
    this->hasMoved = true; 
}
