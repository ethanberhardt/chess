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
    } lastDoubleStepPawn = nullptr; 
}  

Board::~Board(){
    this->clearBoard(); 
}

void Board::boardInit(){

    this->clearBoard(); // just in case!

    // Black's pieces:
    board[7][0] = new Rook("black");
    board[7][1] = new Knight("black");
    board[7][2] = new Bishop("black");
    board[7][3] = new Queen("black");
    board[7][4] = new King("black"); 
    board[7][5] = new Bishop("black"); 
    board[7][6] = new Knight("black");
    board[7][7] = new Rook("black"); 

    

    // White's pieces:
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

void Board::clearBoard(){
    // Freeing Memory [Case 2]: Ending the game
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            if (board[row][col] != nullptr){
                delete board[row][col]; 
                board[row][col] = nullptr; 
            } 
        }
    } lastDoubleStepPawn = nullptr; 
}

void Board::placePiece(int row, int col, Piece* p){
    if (row < 0 || row > 7 || col < 0 || col > 7) return;
    if (board[row][col] != nullptr) delete board[row][col]; 
    board[row][col] = p; 
}

Piece* Board::getPiece(int row, int col){
    return board[row][col]; 
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

    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return 1; // invalid square -> invalid move

    Piece* currPiece = board[fromRow][fromCol]; 
    Piece* destination = board[toRow][toCol];

    // Validates that the piece exists on the square AND checks that the piece's move is valid for what it is
    if (!validatePieceMove(currPiece, fromRow, fromCol, toRow, toCol)) return 1; 

    // Validates if a pawn capture is a valid move
    if (!validatePawnCapture(currPiece, destination, fromRow, fromCol, toRow, toCol)) return 1; 

    // checks if the path is clear [if the move is more than 1 block away]
    if (!isPathClear(fromRow, fromCol, toRow, toCol)) return 2; // path is blocked -> [different type of] invalid move
    
    // can't move to a place where a piece of your own is already situated -> illegal friendly fire
    if (destination != nullptr && destination->getColor() == currPiece->getColor()) return 2; 

    // Makes sure that the move is valid so that the king isn't left in check or moves into check
    if (wouldLeaveKingInCheck(fromRow, fromCol, toRow, toCol, currPiece->getColor())) return 1; 

    // Castling Handler -> needs to be left after wouldLeaveKingInCheck as this moves the rook that is going to be castled
    if (performCastling(currPiece, fromRow, fromCol, toCol) == -1) return 1; 

    // Handling the capture of a piece
    captureHandler(currPiece, destination, fromRow, toCol); 
    
    // Check last pawn that moved 2 squares. Keep this as stored state. If not, assign a nullptr.
    if (tolower(currPiece->getSymbol()) == 'p' && abs(fromRow - toRow) == 2){
        this->lastDoubleStepPawn = currPiece; 
    } else {
        this->lastDoubleStepPawn = nullptr; 
    }

    // Updates the board -> moves the piece!
    updateBoard(fromRow, fromCol, toRow, toCol);

    // Checks for pawn promotions, if applicable
    pawnPromotion(currPiece->getColor(), toRow, toCol); 
    
    return 0; // move successful

}

// Defaults to a queen promotion; when introducing user input, will update this to the user's choice
void Board::pawnPromotion(const string& color, int row, int col){
    if (board[row][col] == nullptr || tolower(board[row][col]->getSymbol()) != 'p') return; 
    
    if ((color == "white" && row == 7) || (color == "black" && row == 0)){
        delete board[row][col]; 
        board[row][col] = new Queen(color); 
    } 
}

bool Board::isCheckMate(const string& color){
    return isKingInCheck(color) && !hasAnyLegalMove(color); 
}

bool Board::isStaleMate(const string& color){
    return !isKingInCheck(color) && !hasAnyLegalMove(color); 
}

bool Board::hasAnyLegalMove(const string& color){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            Piece* currPiece = board[row][col]; 
            if (!currPiece) continue; 
            if (currPiece->getColor() != color) continue; 

            for (int destRow = 0; destRow < 8; destRow++){
                for (int destCol = 0; destCol < 8; destCol++){
                    Piece* destination = board[destRow][destCol];
                    if (row == destRow && col == destCol) continue; 

                    if (!currPiece->isMoveShapeValid(row, col, destRow, destCol)) continue;
                    if (!validatePawnCapture(currPiece, destination, row, col, destRow, destCol)) continue; 
                    if (destination != nullptr && destination->getColor() == currPiece->getColor()) continue; 
                    if (!isPathClear(row, col, destRow, destCol)) continue; 
                    if (wouldLeaveKingInCheck(row, col, destRow, destCol, currPiece->getColor())) continue; 
                    if (tolower(currPiece->getSymbol()) == 'k' && (abs(destCol - col) == 2)) continue; // this is reserved for only castling, not regular king moves
                    
                    cout << "(" << row << ", " << col << ") AND (" << destRow << ", " << destCol << ")" << endl; 
                    return true; 
                }
            }
        }
    } return false; 
}

bool Board::isEnPassantMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toCol, Piece*& ePawn){
    if (tolower(moving->getSymbol()) != 'p') return false;  // returns false if it isn't a pawn
    if (destination != nullptr) return false;               // returns false if it is a regular capture
    if (abs(fromCol - toCol) != 1) return false;            // returns false if the pawn doesn't change columns by 1

    ePawn = board[fromRow][toCol]; 
    return ePawn == lastDoubleStepPawn; 
}

void Board::applyMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, bool isCastle, int rookFromCol, int rookToCol, Piece* rook) {
    board[toRow][toCol] = moving;
    board[fromRow][fromCol] = nullptr;

    if (isEnPassant) board[fromRow][toCol] = nullptr;
    if (isCastle) {
        board[fromRow][rookToCol] = rook;  
        board[fromRow][rookFromCol] = nullptr; 
    }
}

void Board::undoMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, bool isCastle, int rookFromCol, int rookToCol, Piece* rook) {
    board[fromRow][fromCol] = moving;
    board[toRow][toCol] = destination;

    if (isEnPassant) board[fromRow][toCol] = epPawn;
    if (isCastle) {
        board[fromRow][rookFromCol] = rook; 
        board[fromRow][rookToCol] = nullptr; 
    }

}

bool Board::wouldLeaveKingInCheck(int fromRow, int fromCol, int toRow, int toCol, const string& color){
    Piece* moving = board[fromRow][fromCol];
    Piece* destination = board[toRow][toCol];

    Piece* epPawn = nullptr;
    bool isEnPassant = isEnPassantMove(moving, destination, fromRow, fromCol, toCol, epPawn);
 
    int rookFromCol = 0; int rookToCol = 0; 
    bool isCastle = isCastlingAttempt(moving, fromCol, toCol) && isCastlingLegal(moving, fromRow, fromCol, toCol, rookFromCol, rookToCol); 
    Piece* rook = isCastle ? board[fromRow][rookFromCol]: nullptr; 

    applyMove(moving, destination, fromRow, fromCol, toRow, toCol, isEnPassant, epPawn, isCastle, rookFromCol, rookToCol, rook);

    bool inCheck = isKingInCheck(color);

    undoMove(moving, destination, fromRow, fromCol, toRow, toCol, isEnPassant, epPawn, isCastle, rookFromCol, rookToCol, rook);

    return inCheck;
}

bool Board::isCastlingAttempt(Piece* piece, int fromCol, int toCol){
    return (tolower(piece->getSymbol()) == 'k' && abs(toCol - fromCol) == 2); 
}

bool Board::isCastlingLegal(Piece* king, int fromRow, int fromCol, int toCol, int& rookFromCol, int& rookToCol){
    Piece* rook = (fromCol < toCol) ? board[fromRow][7] : board[fromRow][0]; // Gets the rook that the king is trying to castle with
    
    if (!rook || tolower(rook->getSymbol()) != 'r') return false; // checks that there is a piece there and that it is a roook
    if (rook->getHasMoved() || king->getHasMoved()) return false; // checks if the king or rook have moved
    if (isKingInCheck(king->getColor())) {
        return false; // checks if the king is in check currently
    }

    int step = (fromCol < toCol) ? 1 : -1; 

    // for each col that the king is travelling across, check if the square is attacked. 
    for (int col = fromCol; col != toCol + step; col += step){
        if (isSquareAttacked(fromRow, col, king->getColor())) {
            return false; // If so, return false; 
        }
    } 

    rookFromCol = (step == 1) ? 7 : 0; 
    rookToCol   = (step == 1) ? toCol - 1 : toCol + 1;
    
    return true; 
}

int Board::performCastling(Piece* currPiece, int fromRow, int fromCol, int toCol){
    int rookFromCol, rookToCol; 

    if (!isCastlingAttempt(currPiece, fromCol, toCol)) return 1; 
    if (!isCastlingLegal(currPiece, fromRow, fromCol, toCol, rookFromCol, rookToCol)) return -1; 

    board[fromRow][rookToCol] = board[fromRow][rookFromCol]; 
    board[fromRow][rookFromCol] = nullptr; 
    board[fromRow][rookToCol]->notifyMoved();

    return 0; 
}

bool Board::isKingInCheck(const string& color){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            Piece* p = board[row][col]; 
            if (!p) continue; 
            if (tolower(p->getSymbol()) == 'k' && p->getColor() == color){
                // cout << "(" << row << ", " << col << ")" << endl;
                return isSquareAttacked(row, col, color); 
            }
        }
    } return false; 
}

void Board::updateBoard(int fromRow, int fromCol, int toRow, int toCol){
    board[toRow][toCol] = board[fromRow][fromCol]; 
    board[fromRow][fromCol] = nullptr; 
    board[toRow][toCol]->notifyMoved(); 
}

void Board::captureHandler(Piece* currPiece, Piece* destination, int fromRow, int toCol){
    if (destination == nullptr && tolower(currPiece->getSymbol()) == 'p' && board[fromRow][toCol] == lastDoubleStepPawn) {
        delete board[fromRow][toCol]; 
        board[fromRow][toCol] = nullptr; 
    }
    if (destination != nullptr && destination->getColor() != currPiece->getColor()) delete destination; // Freeing Memory [Case 1]
}

bool Board::validatePawnCapture(Piece* currPiece, Piece* destination, int fromRow, int fromCol, int toRow, int toCol){
    if (tolower(currPiece->getSymbol()) != 'p') return true; 

    int deltaRow = fromRow - toRow; 
    int deltaCol = fromCol - toCol; 

    int forward = (currPiece->getColor() == "white") ? -1 : 1; 

    if (abs(deltaCol) == 1 && deltaRow == forward){
        if (destination == nullptr && lastDoubleStepPawn && board[fromRow][toCol] == lastDoubleStepPawn && lastDoubleStepPawn->getColor() != currPiece->getColor()) return true; 
        else if (destination != nullptr && destination->getColor() != currPiece->getColor()) return true; 
        else return false; // a pawn cannot move diagonally if there is nothing there OR there is another pawn of the same colour
    }

    return true; 
}

bool Board::validatePieceMove(Piece* currPiece, int fromRow, int fromCol, int toRow, int toCol){
    if (currPiece == nullptr) { 
        return false; // no piece to move -> invalid move
    } 

    if (!currPiece->isMoveShapeValid(fromRow, fromCol, toRow, toCol)) {
        return false; // invalid move for particular piece -> uses polymorphism here!
    } return true; 
}

// checks UP TO the toSquare; doesn't check if there is a piece on the toSquare [that would be the next step] -> unless it is a pawn :)
bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol){
    // isPathClear DOES NOT apply to Knights as they jump over pieces and kings as they only move one square at a time
    char symbol = tolower(board[fromRow][fromCol]->getSymbol()); 
    if (symbol == 'n'){
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
    while (!(currentRow == toRow && currentCol == toCol)){
        if (board[currentRow][currentCol] != nullptr){ return false; }
        currentRow += stepRow; 
        currentCol += stepCol; 
    }

    // Makes sure that if a pawn is blocked it cannot capture the piece that's there or move forward
    if (symbol == 'p' && stepCol == 0 && board[currentRow][currentCol] != nullptr){
        return false; 
    }

    return true; 
}

bool Board::isSquareAttacked(int row, int col, const string& color){
    string attacker = (color == "white") ? "black" : "white"; 

    for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            Piece* p = this->board[r][c]; 

            if (!p) continue; 
            if (p->getColor() != attacker) continue; 
            if (!p->isMoveShapeValid(r, c, row, col)) continue; 
            if (!isPathClear(r, c, row, col)) continue;
            if (p->getSymbol() == 'p' && c == col) continue; 
            return true; 
        }
    } return false; 
}