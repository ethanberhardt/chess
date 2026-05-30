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
        cout << row+1 << " "; // Just to keep tabs on indices 
        for (int col = 0; col < 8; col++){
            if (board[row][col] == nullptr){
                cout << "· "; 
            } else {
                cout << (board[row][col])->getUnicodeSymbol() << " "; 
            }
        }
        cout << endl; 
    } cout << "  a b c d e f g h" << endl; // Just to keep tabs on indices
}


MoveResult Board::movePiece(int fromRow, int fromCol, int toRow, int toCol){

    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return MoveResult::INVALID_ROW_COL; // invalid square -> invalid move
    
    Piece* currPiece = board[fromRow][fromCol]; 
    Piece* destination = board[toRow][toCol];
    MoveResult result; MoveRecord record; 
    
    // Validates that the piece exists on the square AND checks that the piece's move is valid for what it is
    result = validatePieceMove(currPiece, fromRow, fromCol, toRow, toCol);
    if (result != MoveResult::SUCCESS) return result; 

    // Validates if a pawn capture is a valid move [en passant AND regular pawn captures]
    result = validatePawnCapture(currPiece, destination, fromRow, fromCol, toRow, toCol);
    if (result != MoveResult::SUCCESS) return result; 

    // checks if the path is clear [if the move is more than 1 block away, except pawns; they check in front of them]
    result = isPathClear(fromRow, fromCol, toRow, toCol); 
    if (result != MoveResult::SUCCESS) return result; // path is blocked -> invalid move
    
    // can't move to a place where a piece of your own is already situated -> illegal friendly fire
    if (destination != nullptr && destination->getColor() == currPiece->getColor()) return MoveResult::ILLEGAL_FRIENDLY_FIRE; 

    // Makes sure that the move is valid so that the king isn't left in check or moves into check
    result = wouldLeaveKingInCheck(fromRow, fromCol, toRow, toCol, currPiece->getColor()); 
    if (result != MoveResult::SUCCESS) return result; 

    // Castling Handler -> needs to be left after wouldLeaveKingInCheck as this moves the rook that is going to be castled
    result = performCastling(currPiece, fromRow, fromCol, toCol, record); 
    if (result != MoveResult::SUCCESS) return result; 

    // Handling the capture of a piece
    // Creating MoveRecord for the Undo/Redo Functionality 

    Piece* ep = nullptr; 
    if (isEnPassantMove(currPiece, destination, fromRow, fromCol, toCol, ep)){
        record.type = MoveType::EN_PASSANT; 
        record.capturedRow = fromRow; 
        record.capturedCol = toCol; 
    }

    Piece* captured = captureHandler(currPiece, destination, fromRow, toCol); 

    // Only true for NORMAL moves right now
    record.fromRow = fromRow; record.fromCol = fromCol; 
    record.toRow = toRow; record.toCol = toCol; 

    record.movingPiece = currPiece; 

    record.capturedPiece = captured; 

    record.previousLastDoubleStepPawn = lastDoubleStepPawn; 

    record.movingPieceHadMoved = currPiece->getHasMoved(); 
    
    // Check last pawn that moved 2 squares. Keep this as stored state. If not, assign a nullptr.
    if (tolower(currPiece->getSymbol()) == 'p' && abs(fromRow - toRow) == 2){
        this->lastDoubleStepPawn = currPiece; 
    } else {
        this->lastDoubleStepPawn = nullptr; 
    }

    // Push to the history stack
    // cout << (int)record.type << endl;    
    // cout << record.capturedRow << endl; 
    // cout << record.capturedCol << endl;   
    moveHistory.push(record); 
    // cout << sizeof(moveHistory) << endl; 

    // Updates the board -> moves the piece!
    updateBoard(fromRow, fromCol, toRow, toCol);

    // cout << "Will I see this or not?" << endl;

    // Checks for pawn promotions, if applicable
    record.promotedPiece = pawnPromotion(currPiece->getColor(), toRow, toCol); 
    if (record.promotedPiece != nullptr){
        record.type = MoveType::PROMOTION; 
    }
    
    return MoveResult::SUCCESS; // move successful

}

bool Board::undoLastMove(){
    if (moveHistory.empty()) return false; 

    // Handles only Normal Moves
    MoveRecord record = moveHistory.top(); 

    // cout << "From: (" << record.fromRow << "," << record.fromCol << ")" << endl; 
    // cout << "To:   (" << record.toRow << "," << record.toCol << ")" << endl; 

    // cout << "Moving Piece Symbol: " << record.movingPiece->getSymbol() << endl; 
    // cout << "Is Captured Piece a NullPtr: " << (record.capturedPiece == nullptr) << endl; 
    // cout << "Is DoubleStepPawn a NullPtr: " << (record.previousLastDoubleStepPawn == nullptr) << endl; 

    moveHistory.pop(); 

    // cout << "Removing record from history" << endl; 

    board[record.fromRow][record.fromCol] = record.movingPiece; 
    // cout << "Putting back piece to where it was" << endl; 

    board[record.toRow][record.toCol] = record.capturedPiece; 
    // cout << "Restoring captured piece to it's square [if it exists]" << endl; 

    this->lastDoubleStepPawn = record.previousLastDoubleStepPawn; 
    // cout << "Restoring state for double pawn" << endl; 

    switch(record.type) {

        case (MoveType::CASTLE): 
            board[record.fromRow][record.rookFromCol] = record.rook; 
            board[record.fromRow][record.rookToCol] = nullptr; 
            record.rook->setHasMoved(record.rookHadMoved); 
            break; 

        case (MoveType::EN_PASSANT): 
            board[record.toRow][record.toCol] = nullptr; // IMPORTANT AS THE PAWN CAPTURED WOULDN'T BE ON IT'S REGULAR SQUARE
            board[record.capturedRow][record.capturedCol] = record.capturedPiece;
            break; 
        
        case (MoveType::PROMOTION): 
            delete record.promotedPiece; 
            board[record.toRow][record.toCol] = record.capturedPiece; 
            board[record.fromRow][record.fromCol] = record.movingPiece;
    }
    // cout << "Moving piece ptr: " << record.movingPiece << endl;
    // cout << "Board[from] ptr:  " << board[record.fromRow][record.fromCol] << endl;
    // cout << "About to restore hasMoved..." << endl;
    record.movingPiece->setHasMoved(record.movingPieceHadMoved);
    // cout << "Finished restoring hasMoved..." << endl;
    // cout << "Restoring hasMoved State" << endl; 

    return true; 
}

// Defaults to a queen promotion; when introducing user input, will update this to the user's choice
Piece* Board::pawnPromotion(const string& color, int row, int col){
    if (board[row][col] == nullptr || tolower(board[row][col]->getSymbol()) != 'p') return nullptr; 
    
    if ((color == "white" && row == 7) || (color == "black" && row == 0)){
        board[row][col] = new Queen(color); 
        return board[row][col]; 
    }

    return nullptr; 
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
                    if (validatePawnCapture(currPiece, destination, row, col, destRow, destCol) != MoveResult::SUCCESS) continue; 
                    if (destination != nullptr && destination->getColor() == currPiece->getColor()) continue; 
                    if (isPathClear(row, col, destRow, destCol) != MoveResult::SUCCESS) continue; 
                    if (wouldLeaveKingInCheck(row, col, destRow, destCol, currPiece->getColor()) != MoveResult::SUCCESS) continue; 
                    if (tolower(currPiece->getSymbol()) == 'k' && (abs(destCol - col) == 2)) continue; // this is reserved for only castling, not regular king moves
                    
                    // cout << "(" << row << ", " << col << ") AND (" << destRow << ", " << destCol << ")" << endl; 
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

MoveResult Board::wouldLeaveKingInCheck(int fromRow, int fromCol, int toRow, int toCol, const string& color){
    Piece* moving = board[fromRow][fromCol];
    Piece* destination = board[toRow][toCol];

    Piece* epPawn = nullptr;
    bool isEnPassant = isEnPassantMove(moving, destination, fromRow, fromCol, toCol, epPawn);
 
    int rookFromCol = 0; int rookToCol = 0; 
    bool isCastle = (isCastlingAttempt(moving, fromRow, fromCol, toCol)) && (isCastlingLegal(moving, fromRow, fromCol, toCol, rookFromCol, rookToCol) == MoveResult::SUCCESS); 
    Piece* rook = isCastle ? board[fromRow][rookFromCol]: nullptr; 

    applyMove(moving, destination, fromRow, fromCol, toRow, toCol, isEnPassant, epPawn, isCastle, rookFromCol, rookToCol, rook);

    bool inCheck = isKingInCheck(color);

    undoMove(moving, destination, fromRow, fromCol, toRow, toCol, isEnPassant, epPawn, isCastle, rookFromCol, rookToCol, rook);

    if (inCheck) return MoveResult::LEAVES_KING_IN_CHECK; 

    return MoveResult::SUCCESS;
}

bool Board::isCastlingAttempt(Piece* piece, int fromRow, int fromCol, int toCol){
    if (tolower(piece->getSymbol()) != 'k' || abs(toCol - fromCol) != 2) return false; 
    if (piece->getColor() == "white") return (fromRow == 0 && fromCol == 4); 
    else return (fromRow == 7 && fromCol == 4); 
}

MoveResult Board::isCastlingLegal(Piece* king, int fromRow, int fromCol, int toCol, int& rookFromCol, int& rookToCol){
    Piece* rook = (fromCol < toCol) ? board[fromRow][7] : board[fromRow][0]; // Gets the rook that the king is trying to castle with
    
    if (!rook || tolower(rook->getSymbol()) != 'r') return MoveResult::ERR_CASTLING_ROOK_MOVED; // checks that there is a piece there and that it is a rook
    if (king->getHasMoved()) return MoveResult::ERR_CASTLING_KING_MOVED; // checks if the king or rook have moved
    if (rook->getHasMoved()) return MoveResult::ERR_CASTLING_ROOK_MOVED; // checks if the king or rook have moved
    if (isKingInCheck(king->getColor())) {
        return MoveResult::KING_IN_CHECK; // checks if the king is in check currently
    }

    int step = (fromCol < toCol) ? 1 : -1; 

    // for each col that the king is travelling across, check if the square is attacked. 
    for (int col = fromCol; col != toCol + step; col += step){
        if (isSquareAttacked(fromRow, col, king->getColor())) {
            return MoveResult::LEAVES_KING_IN_CHECK; // If so, return the appropriate error.
        }
    } 

    rookFromCol = (step == 1) ? 7 : 0; 
    rookToCol   = (step == 1) ? toCol - 1 : toCol + 1;
    
    return MoveResult::SUCCESS; 
}

MoveResult Board::performCastling(Piece* currPiece, int fromRow, int fromCol, int toCol, MoveRecord& record){
    int rookFromCol, rookToCol; 

    if (!isCastlingAttempt(currPiece, fromRow, fromCol, toCol)) return MoveResult::SUCCESS; // successful as the move itself is NOT a castling attempt

    MoveResult result = isCastlingLegal(currPiece, fromRow, fromCol, toCol, rookFromCol, rookToCol); 
    if (result != MoveResult::SUCCESS) return result;  

    record.type = MoveType::CASTLE; 
    record.rook = board[fromRow][rookFromCol]; 
    record.rookFromCol = rookFromCol; 
    record.rookToCol = rookToCol; 
    record.rookHadMoved = record.rook->getHasMoved(); 

    board[fromRow][rookToCol] = board[fromRow][rookFromCol]; 
    board[fromRow][rookFromCol] = nullptr; 
    board[fromRow][rookToCol]->notifyMoved();

    return MoveResult::SUCCESS; 
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
    // cout << board[fromRow][fromCol] << endl; 
    // cout << board[toRow][toCol] << endl; 

    board[toRow][toCol] = board[fromRow][fromCol]; 
    board[fromRow][fromCol] = nullptr; 
    board[toRow][toCol]->notifyMoved(); 
}

Piece* Board::captureHandler(Piece* currPiece, Piece* destination, int fromRow, int toCol){
    if (destination == nullptr && tolower(currPiece->getSymbol()) == 'p' && board[fromRow][toCol] == lastDoubleStepPawn) {
        Piece* captured = board[fromRow][toCol]; 
        board[fromRow][toCol] = nullptr; 
        return captured; 
    }
    if (destination != nullptr && destination->getColor() != currPiece->getColor()) return destination; // This changes now as we are keeping track of the history of the game

    return nullptr; 
}

MoveResult Board::validatePawnCapture(Piece* currPiece, Piece* destination, int fromRow, int fromCol, int toRow, int toCol){
    if (tolower(currPiece->getSymbol()) != 'p') return MoveResult::SUCCESS; 

    int deltaRow = fromRow - toRow; 
    int deltaCol = fromCol - toCol; 

    int forward = (currPiece->getColor() == "white") ? -1 : 1; 

    if (abs(deltaCol) == 1 && deltaRow == forward){
        if (destination == nullptr && lastDoubleStepPawn && board[fromRow][toCol] == lastDoubleStepPawn && lastDoubleStepPawn->getColor() != currPiece->getColor()) return MoveResult::SUCCESS; 
        else if (destination != nullptr && destination->getColor() != currPiece->getColor()) return MoveResult::SUCCESS; 
        else return MoveResult::INVALID_PAWN_CAPTURE; // a pawn cannot move diagonally if there is nothing there OR there is another pawn of the same colour
    }

    return MoveResult::SUCCESS; 
}

MoveResult Board::validatePieceMove(Piece* currPiece, int fromRow, int fromCol, int toRow, int toCol){
    if (currPiece == nullptr) { 
        return MoveResult::NON_EXISTENT_PIECE; // no piece to move -> invalid move
    } 

    if (!currPiece->isMoveShapeValid(fromRow, fromCol, toRow, toCol)) {
        if (currPiece->getSymbol() == 'K' && ((fromRow - toRow != 0) || (fromCol - toCol != 0))) return MoveResult::ERR_CASTLING_KING_MOVED; 
        return MoveResult::INVALID_PIECE_MOVE; // invalid move for particular piece -> uses polymorphism here!
    } return MoveResult::SUCCESS; 
}

// checks UP TO the toSquare; doesn't check if there is a piece on the toSquare [that would be the next step] -> unless it is a pawn :)
MoveResult Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol){
    // isPathClear DOES NOT apply to Knights as they jump over pieces and kings as they only move one square at a time
    char symbol = tolower(board[fromRow][fromCol]->getSymbol()); 
    if (symbol == 'n'){
        return MoveResult::SUCCESS; 
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
        if (board[currentRow][currentCol] != nullptr){ return MoveResult::PATH_NOT_CLEAR; }
        currentRow += stepRow; 
        currentCol += stepCol; 
    }

    // Makes sure that if a pawn is blocked it cannot capture the piece that's there or move forward
    if (symbol == 'p' && stepCol == 0 && board[currentRow][currentCol] != nullptr){
        return MoveResult::PATH_NOT_CLEAR; 
    }

    return MoveResult::SUCCESS; 
}

bool Board::isSquareAttacked(int row, int col, const string& color){
    string attacker = (color == "white") ? "black" : "white"; 

    for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            Piece* p = this->board[r][c]; 

            if (!p) continue; 
            if (p->getColor() != attacker) continue; 
            if (!p->isMoveShapeValid(r, c, row, col)) continue; 
            if (isPathClear(r, c, row, col) != MoveResult::SUCCESS) continue;
            if (p->getSymbol() == 'p' && c == col) continue; 
            return true; 
        }
    } return false; 
}

int moveRecord(int fromRow, int fromCol, int toRow, int toCol, Piece* moved, Piece* capture, 
                int capturedRow, int capturedCol, Piece* previousHasMoved, enum MoveType); 