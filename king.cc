#include "king.h"

using namespace std; 

King::King(const string& color): Piece(color, 'K') {}

bool King::isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol){
    int deltaRow = fromRow - toRow; // change in rows [moving from left to right is +]
    int deltaCol = fromCol - toCol; // change in cols [moving from up to down is +]


    if (deltaRow == 0 && deltaCol == 0){ // make sure that we are actually moving
        return false; 
    } else if (abs(deltaRow) <= 1 && abs(deltaCol) <= 1){ // kings can move only move one square anywhere around them
        return true;
    } else {
        return false; 
    }
}
