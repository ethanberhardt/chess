#include "board.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "king.h"
#include "pawn.h"
#include <iostream>

using namespace std; 

int Board::sign(int x){
    if (x > 0) return 1; 
    else if (x < 0) return -1; 
    return 0; 
}

Board::Board(){

    // initialize all pointers on the board to null
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            board[row][col] = nullptr; 
        }
    }

    // After implementing all pieces, will make this a little bit more efficient

    // TESTING KNIGHT
    // board[4][4] = new Knight("white"); 

    // board[4][3] = new Rook("black");
    // board[4][5] = new Rook("black");
    // board[3][4] = new Rook("black");
    // board[5][4] = new Rook("black");

    // board[6][5] = new Bishop("black"); 
    

    // White's pieces:
    board[7][0] = new Rook("black");
    board[7][1] = new Knight("black");
    board[7][2] = new Bishop("black");
    board[7][3] = new Queen("black");
    board[7][4] = new King("black"); 
    board[7][5] = new Bishop("black"); 
    board[7][6] = new Knight("black");
    board[7][7] = new Rook("black"); 

    

    // Black's pieces:
    board[0][0] = new Rook("white"); 
    board[0][1] = new Knight("white");
    board[0][2] = new Bishop("white");
    board[0][3] = new Queen("white"); 
    board[0][4] = new King("white"); 
    board[0][5] = new Bishop("white"); 
    board[0][6] = new Knight("white");
    board[0][7] = new Rook("white"); 

    // All pawns
    for (int i = 0; i < 8; i++){
        board[6][i] = new Pawn("black");
        board[1][i] = new Pawn("white");
    }
}

Board::~Board(){
    // Freeing Memory [Case 2]: Ending the game
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            if (board[row][col] != nullptr){
                delete board[row][col]; 
                board[row][col] = nullptr; 
            } 
        }
    }
}

void Board::printBoard(){
    for (int row = 7; row >= 0; row--){
        cout << row << " "; // Just to keep tabs on indices 
        for (int col = 0; col < 8; col++){
            if (board[row][col] == nullptr){
                cout << ". "; 
            } else {
                cout << (board[row][col])->getSymbol() << " "; 
            }
        }
        cout << endl; 
    } cout << "  0 1 2 3 4 5 6 7" << endl; // Just to keep tabs on indices
}

// returns 0: successful | returns 1: invalid move | returns 2: path blocked OR 'to' box is occupied by same colour piece
int Board::movePiece(int fromRow, int fromCol, int toRow, int toCol){

    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7){
        return 1; // invalid square -> invalid move
    }

    Piece* currPiece = board[fromRow][fromCol]; 
    Piece* destination = board[toRow][toCol];

    if (currPiece == nullptr) { 
        cout << "No piece to move!" << endl; 
        return 1; // no piece to move -> invalid move
    } 

    if (!currPiece->isMoveShapeValid(fromRow, fromCol, toRow, toCol)) {
        cout << "Wrong Shape!" << endl; 
        return 1; // invalid move for particular piece -> uses polymorphism here!
    }

    if (tolower(currPiece->getSymbol()) == 'p'){
        int deltaRow = fromRow - toRow; 
        int deltaCol = fromCol - toCol; 

        int forward = (currPiece->getColor() == "white") ? -1 : 1; 

        if (abs(deltaCol) == 1 && deltaRow == forward){
            if (destination == nullptr || destination->getColor() == currPiece->getColor()){
                return 1; // a pawn cannot move diagonally if there is nothing there OR there is another pawn of the same colour
            }
        }
    }

    if (!isPathClear(fromRow, fromCol, toRow, toCol)){
        return 2; // path is blocked -> [different type of] invalid move
    }

    if (destination != nullptr && destination->getColor() == currPiece->getColor()){
        return 2; // can't move to a place where a piece of your own is already situated -> invalid move
    }

    // Freeing Memory [Case 1]: Capturing a piece
    if (destination != nullptr && destination->getColor() != currPiece->getColor()){
        delete destination; 
    }

    board[toRow][toCol] = board[fromRow][fromCol]; 
    board[fromRow][fromCol] = nullptr; 

    board[toRow][toCol]->notifyMoved(); // for now, pawns 'hasMoved' will be updated to true [kings and rooks will later implement a version of this]

    return 0; // move successful

}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol){
    // isPathClear DOES NOT apply to Knights as they jump over pieces
    if (tolower(board[fromRow][fromCol]->getSymbol()) == 'n'){
        return true; 
    }

    int deltaRow = toRow - fromRow; // change in rows [moving from left to right is +]
    int deltaCol = toCol - fromCol; // change in cols [moving from up to down is +]

    // How the piece moves -> depends on the type of piece and where it's going
    // rook: stepRow OR stepCol will be 1 and the other will be 0
    // bishop: stepRow AND stepCol will be 1 [since bishops can move diagonally]
    int stepRow = sign(deltaRow); 
    int stepCol = sign(deltaCol); 

    // Creating currentRow and currentCol to check if this square is clear
    // this is the next [potential] square the piece can take (if it can) 
    int currentRow = fromRow + stepRow; 
    int currentCol = fromCol + stepCol; 

    // checks each square until the final destination to make sure that there are no 
    // pieces in the way that can prohibit the piece from moving to that square
    while (currentRow != toRow || currentCol != toCol){
        if (board[currentRow][currentCol] != nullptr){ return false; }
        currentRow += stepRow; 
        currentCol += stepCol; 
    }

    return true; 
}