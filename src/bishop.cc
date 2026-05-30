#include "bishop.h"

using namespace std; 

Bishop::Bishop(const string& color): Piece(color, 'B') {}

bool Bishop::isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol){
    int deltaRow = fromRow - toRow; // change in rows [moving from left to right is +]
    int deltaCol = fromCol - toCol; // change in cols [moving from up to down is +]

    if (deltaRow == 0 && deltaCol == 0){
        return false; 
    } else if (abs(deltaRow) == abs(deltaCol)){ // bishops move diagonally so they need to move the same number of rows as cols [backwards counts]
        return true; 
    } else {
        return false; 
    }
}
