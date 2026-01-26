#include "knight.h"

using namespace std; 

Knight::Knight(const string& color): Piece(color, 'N') {}

bool Knight::isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol){
    int deltaRow = fromRow - toRow; // change in rows [moving from left to right is +]
    int deltaCol = fromCol - toCol; // change in cols [moving from up to down is +]

    if (deltaRow == 0 && deltaCol == 0){
        return false; 
    } else if (abs(deltaRow) == 1 && abs(deltaCol) == 2){ // knights move 1 [up/down] and 2 [right/left] 
        return true; 
    } else if (abs(deltaCol) == 1 && abs(deltaRow) == 2){ // OR they move 1 [right/left] and 2 [up/down]
        return true; 
    } else {
        return false; 
    }

}
