#include "rook.h"

using namespace std; 

Rook::Rook(const string& color): Piece(color, 'R') {}

bool Rook::isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol){
    int deltaRow = fromRow - toRow; // change in rows [moving from left to right is +]
    int deltaCol = fromCol - toCol; // change in cols [moving from up to down is +]

    if (deltaRow == 0 && deltaCol != 0){ // vertical move [up and down the board]
        return true; 
    } else if (deltaRow != 0 && deltaCol == 0){ // horizontal move [side to side across the board]
        return true; 
    } else {
        return false; 
    }
}
