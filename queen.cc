#include "queen.h"

using namespace std; 

Queen::Queen(const string& color): Piece(color, 'Q') {}

bool Queen::isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol){
    int deltaRow = fromRow - toRow; // change in rows [moving from left to right is +]
    int deltaCol = fromCol - toCol; // change in cols [moving from up to down is +]


    if (deltaRow == 0 && deltaCol == 0){ // make sure that we are actually moving
        return false; 
    } else if (abs(deltaRow) == abs(deltaCol)){ // queens can move diagonally so they need to move the same number of rows as cols [backwards counts]
        return true;
    } else if (deltaRow == 0 && deltaCol != 0){ // vertical move [up and down the board]
        return true; 
    } else if (deltaRow != 0 && deltaCol == 0){ // horizontal move [side to side across the board]
        return true; 
    } else {
        return false; 
    }
}
