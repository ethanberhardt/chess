#include "board.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "king.h"
#include "pawn.h"
#include <iostream>

using namespace std;

/*
    Chess Engine V1

    Features:
    - Complete move validation
    - Check, checkmate, and stalemate detection
    - Castling
    - En passant
    - Pawn promotion
    - Undo functionality
    - Threefold repetition detection
    - Fifty-move rule detection

    Designed as the backend engine for future GUI and web-based versions.
*/

/* 
    - Initializes an empty chess board and resets all game-state tracking.
    - All board squares are set to nullptr, no en passant target exists, 
      the fifty-move counter is reset, and White is set to move first.
    - Piece placement is performed separately by boardInit(). 
*/
Board::Board(){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            board[row][col] = nullptr; 
        }
    }

    lastDoubleStepPawn = nullptr; 
    lastDoubleStepRow = -1; 
    lastDoubleStepCol = -1; 

    halfMoveClock = 0; 
    whiteToMove = true; 
} 

Board::~Board(){
    this->clearBoard(); 
}

/* 
    - Displays the current board state using Unicode chess symbols.
    - Automatically adjusts board orientation based on whose turn it is,
      allowing each player to view the board from their own perspective.
    - Empty squares are represented by dots and rank/file labels are
      displayed to match the current orientation.
*/
void Board::printBoard(){

    int rowStart = whiteToMove ? 7 : 0; 
    int rowEnd = whiteToMove ? -1 : 8;
    int rowStep = whiteToMove ? -1 : 1; 

    int colStart = whiteToMove ? 0 : 7; 
    int colEnd = whiteToMove ? 8 : -1; 
    int colStep = whiteToMove ? 1 : -1; 


    for (int row = rowStart; row != rowEnd; row += rowStep){
        cout << row+1 << " "; // Just to keep tabs on indices 
        for (int col = colStart; col != colEnd; col += colStep){
            if (board[row][col] == nullptr){
                cout << "· "; 
            } else {
                cout << (board[row][col])->getUnicodeSymbol() << " "; 
            }
        }
        cout << endl; 
    } 
    
    // Just to keep tabs on indices
    if (whiteToMove) cout << "  a b c d e f g h" << endl;
    else cout << "  h g f e d c b a" << endl;
}

/* 
    - Initializes the board to the standard starting chess position.
    - Clears any existing board state before creating and placing all
      pieces in their default locations.
    - Records the initial board fingerprint to begin position tracking
      for threefold repetition detection.
*/
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

    positionHistory.push_back(generateFingerPrint()); 
}

/* 
    - Removes all pieces from the board and releases their dynamically
      allocated memory.
    - Iterates through every square, deleting existing pieces and
      resetting all board pointers to nullptr.
    - Resets en passant, turn, and fifty-move rule state, returning
      the board to a clean default configuration.
*/
void Board::clearBoard(){
    // Freeing Memory [Case 2]: Ending the game
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            if (board[row][col] != nullptr){
                delete board[row][col]; 
                board[row][col] = nullptr; 
            } 
        }
    } 
    
    // Freeing pieces stored in move history that are not on the board
    while (!moveHistory.empty()){
        MoveRecord record = moveHistory.top(); 
        if (record.capturedPiece != nullptr) delete record.capturedPiece; 
        if (record.type == MoveType::PROMOTION && record.movingPiece != nullptr) delete record.movingPiece;
        moveHistory.pop(); 
    }

    lastDoubleStepPawn = nullptr; 
    lastDoubleStepRow = -1; lastDoubleStepCol = -1; 

    whiteToMove = true;
    halfMoveClock = 0;

    positionHistory.clear(); 
}

/* 
    - Places a piece on the specified square without performing any
      move validation or game-state updates.
    - Primarily intended for unit tests and custom board setup scenarios.
    - Replaces and safely deletes any existing piece occupying the target
      square before inserting the new piece.
*/
void Board::placePiece(int row, int col, Piece* p){
    if (row < 0 || row > 7 || col < 0 || col > 7) return;
    if (board[row][col] != nullptr) delete board[row][col]; 
    board[row][col] = p; 
}

Piece* Board::getPiece(int row, int col){
    return board[row][col]; 
}

void Board::setWhiteToMove(bool whiteToMove){
    this->whiteToMove = whiteToMove; 
}

/* 
    - Executes a move after validating that it is legal under all chess rules.
    - Handles special moves including castling, en passant, and promotion,
      while also recording state required for undo functionality.
    - Updates board state, move history, draw-tracking information, turn
      ownership, and position history before completing the move.
*/
MoveResult Board::movePiece(int fromRow, int fromCol, int toRow, int toCol){

    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return MoveResult::INVALID_ROW_COL; // invalid square -> invalid move
    
    Piece* currPiece = board[fromRow][fromCol]; 
    Piece* destination = board[toRow][toCol];
    MoveResult result; MoveRecord record; 
    
    // Validates if the piece selected is owned by the player who selected it
    result = validateYourPiece(currPiece);
    if (result != MoveResult::SUCCESS) return result; 

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


    // Checks for en passant [for the MoveRecord], if applicable
    Piece* ep = nullptr; 
    if (isEnPassantMove(currPiece, destination, fromRow, fromCol, toCol, ep)){
        record.type = MoveType::EN_PASSANT; 
        record.capturedRow = fromRow; 
        record.capturedCol = toCol; 
    }

    // Handling the capture of a piece && Creating MoveRecord for Undo Functionality 
    Piece* captured = captureHandler(currPiece, destination, fromRow, toCol); 
    buildMoveRecord(fromRow, fromCol, toRow, toCol, lastDoubleStepRow, lastDoubleStepCol, currPiece, captured, record); 

    // Check last pawn that moved 2 squares. Keep this as stored state. If not, assign a nullptr
    // -> Different from making en passant happen; just updating state
    if (tolower(currPiece->getSymbol()) == 'p' && abs(fromRow - toRow) == 2){
        this->lastDoubleStepPawn = currPiece; 
        lastDoubleStepRow = toRow; 
        lastDoubleStepCol = toCol; 
    } else {
        this->lastDoubleStepPawn = nullptr; 
        lastDoubleStepRow = -1; 
        lastDoubleStepCol = -1;
    }

    // Updates the board -> moves the piece!
    updateBoard(fromRow, fromCol, toRow, toCol);

    // Checks for pawn promotions, if applicable
    record.promotedPiece = pawnPromotion(currPiece->getColor(), toRow, toCol); 
    if (record.promotedPiece != nullptr){
        record.type = MoveType::PROMOTION; 
    }

    moveHistory.push(record);  

    if (tolower(currPiece->getSymbol()) == 'p' || captured != nullptr) halfMoveClock = 0; 
    else halfMoveClock++; 

    whiteToMove = !whiteToMove;
    
    positionHistory.push_back(generateFingerPrint()); 
    
    return MoveResult::SUCCESS; // move successful

}

/* 
    - Reverts the most recently executed move and restores the board to
      its previous state.
    - Uses the stored MoveRecord to undo normal moves, castling,
      en passant captures, and pawn promotions.
    - Restores game state including piece movement flags, en passant
      tracking, turn ownership, and position history.
*/
bool Board::undoLastMove(){
    if (moveHistory.empty()) return false; 

    // Handles only Normal Moves
    MoveRecord record = moveHistory.top();
    moveHistory.pop(); 

    board[record.fromRow][record.fromCol] = record.movingPiece; 
    board[record.toRow][record.toCol] = record.capturedPiece; 
    this->lastDoubleStepPawn = record.previousLastDoubleStepPawn; 
    this->lastDoubleStepCol = record.previousLastDoubleStepCol; 
    this->lastDoubleStepRow = record.previousLastDoubleStepRow; 

    switch(record.type) {

        case (MoveType::NORMAL): 
            break; 

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
            break; 

        default: 
            cout << "ERROR!!! Weird Move Type in Undo Move Functionality" << endl; 
            return false; 
        }

    record.movingPiece->setHasMoved(record.movingPieceHadMoved); 
    whiteToMove = !whiteToMove; 

    positionHistory.pop_back(); 

    return true; 
}

bool Board::isCheckMate(const string& color){
    return isKingInCheck(color) && !hasAnyLegalMove(color); 
}

bool Board::isStaleMate(const string& color){
    return !isKingInCheck(color) && !hasAnyLegalMove(color); 
}

bool Board::isWhiteToMove() const{
    return whiteToMove; 
}

bool Board::isFiftyMoveDraw() const{
    return halfMoveClock >= 100; 
}

bool Board::isThreeFoldRepetition() const{
    if (positionHistory.size() < 3) return false; 

    string current = positionHistory.back(); 
    int counter = 0; 

    for (size_t i = 0; i < positionHistory.size() - 1; i++){
        if (positionHistory[i] == current) counter++; 
        if (counter >= 3) return true; 
    } 
    
    return false; 
} 

/* 
    - Returns the sign of an integer value.
    - Produces 1 for positive values, -1 for negative values,
      and 0 when the value is zero.
    - Used to determine movement direction when traversing the board.
*/
int Board::sign(int x){
    if (x > 0) return 1; 
    else if (x < 0) return -1; 
    return 0; 
}

/* 
    - Verifies that the selected piece exists and belongs to the player
      whose turn it currently is.
    - Prevents players from moving opponent pieces or selecting empty
      squares as move origins.
    - Serves as the first validation step in the move execution process.
*/
MoveResult Board::validateYourPiece(Piece* currPiece){
    if (currPiece == nullptr) return MoveResult::NON_EXISTENT_PIECE; // no piece to move -> invalid move
    string color = (whiteToMove) ? "white" : "black";

    if (currPiece->getColor() == color) return MoveResult::SUCCESS; 
    else return MoveResult::WRONG_TURN; 
}

/* 
    - Verifies that the requested move follows the movement rules of the
      selected piece.
    - Delegates move-shape validation to the piece hierarchy through
      polymorphism, allowing each piece type to define its own movement.
    - Provides specialized error reporting for castling attempts that
      fail due to king movement restrictions.
*/
MoveResult Board::validatePieceMove(Piece* currPiece, int fromRow, int fromCol, int toRow, int toCol){

    if (!currPiece->isMoveShapeValid(fromRow, fromCol, toRow, toCol)) {
        if (currPiece->getSymbol() == 'K' && ((fromRow - toRow != 0) || (fromCol - toCol != 0))) return MoveResult::ERR_CASTLING_KING_MOVED; 
        return MoveResult::INVALID_PIECE_MOVE; // invalid move for particular piece -> uses polymorphism here!
    } return MoveResult::SUCCESS; 
}

/* 
    - Validates pawn capture attempts, including both standard captures
      and en passant captures.
    - Ensures diagonal pawn movement only occurs when capturing an
      opposing piece or performing a legal en passant capture.
    - Prevents illegal diagonal pawn moves into empty squares or
      occupied squares containing friendly pieces.
*/
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

/* 
    - Verifies that all intermediate squares between the starting and
      destination squares are unobstructed.
    - Applies to sliding pieces such as rooks, bishops, queens, and
      pawns, while knights are excluded because they can jump over pieces.
    - Does not evaluate occupancy of the destination square except when
      validating forward pawn movement.
*/
MoveResult Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol){
    // checks UP TO the toSquare; doesn't check if there is a piece on the toSquare [that would be the next step] -> unless it is a pawn :)
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

/* 
    - Determines whether a move would leave the moving side's king in check.
    - Temporarily applies the move to the board, evaluates the resulting
      position, and then restores the original board state.
    - Handles special moves such as castling and en passant to ensure
      check validation accurately reflects the position after the move.
*/
MoveResult Board::wouldLeaveKingInCheck(int fromRow, int fromCol, int toRow, int toCol, const string& color){
    Piece* moving = board[fromRow][fromCol];
    Piece* destination = board[toRow][toCol];

    Piece* epPawn = nullptr;
    bool isEnPassant = isEnPassantMove(moving, destination, fromRow, fromCol, toCol, epPawn);
 
    int rookFromCol = -1; int rookToCol = -1; 
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

/* 
    - Validates whether a castling attempt satisfies all legal castling
      requirements.
    - Verifies that both the king and rook are eligible to castle and
      that the king is not currently in check.
    - Ensures the king does not move through or end on an attacked square
      and determines the rook movement required to complete the castle.
*/
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

/* 
    - Executes the rook movement associated with a legal castling move.
    - Validates the castling attempt and records the information required
      to undo the castle if necessary.
    - Updates the rook's position and movement state while leaving the
      king's movement to the normal move execution process.
*/
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

/* 
    - Determines whether a move qualifies as a legal en passant capture.
    - Verifies that a pawn is moving diagonally into an empty square and
      that the adjacent pawn is eligible to be captured en passant.
    - Returns the capturable pawn through ePawn when an en passant
      capture is detected.
*/
bool Board::isEnPassantMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toCol, Piece*& ePawn){
    if (tolower(moving->getSymbol()) != 'p') return false;  // returns false if it isn't a pawn
    if (destination != nullptr) return false;               // returns false if it is a regular capture
    if (abs(fromCol - toCol) != 1) return false;            // returns false if the pawn doesn't change columns by 1

    ePawn = board[fromRow][toCol]; 
    return ePawn == lastDoubleStepPawn; 
}

/* 
    - Handles pawn promotion when a pawn reaches the final rank.
    - Replaces the promoted pawn with a queen and returns a pointer
      to the newly created piece for undo tracking.
    - Currently supports automatic queen promotion only.
*/
Piece* Board::pawnPromotion(const string& color, int row, int col){
    if (board[row][col] == nullptr || tolower(board[row][col]->getSymbol()) != 'p') return nullptr; 
    
    if ((color == "white" && row == 7) || (color == "black" && row == 0)){
        board[row][col] = new Queen(color); 
        return board[row][col]; 
    }

    return nullptr; 
}

/* 
    - Identifies and processes captures associated with a move.
    - Handles both standard captures and the special case of en passant,
      returning the captured piece when applicable.
    - Removes en passant victims from the board while preserving captured
      pieces for move history and undo functionality.
*/
Piece* Board::captureHandler(Piece* currPiece, Piece* destination, int fromRow, int toCol){
    if (destination == nullptr && tolower(currPiece->getSymbol()) == 'p' && board[fromRow][toCol] == lastDoubleStepPawn) {
        Piece* captured = board[fromRow][toCol]; 
        board[fromRow][toCol] = nullptr; 
        return captured; 
    }
    if (destination != nullptr && destination->getColor() != currPiece->getColor()) return destination; // This changes now as we are keeping track of the history of the game

    return nullptr; 
}

/* 
    - Populates a MoveRecord with the information required to undo a move.
    - Stores the move coordinates, participating pieces, and relevant
      board state that may be modified during move execution.
    - Captures the moving piece's previous state before any board
      mutations occur.
*/
void Board::buildMoveRecord(int fromRow, int fromCol, int toRow, int toCol, int previousLastDoubleStepRow, int previousLastDoubleStepCol, Piece* currPiece, Piece* captured, MoveRecord& record){
    record.fromRow = fromRow; record.fromCol = fromCol; 
    record.toRow = toRow; record.toCol = toCol; 

    record.movingPiece = currPiece; 
    record.capturedPiece = captured; 

    record.previousLastDoubleStepPawn = lastDoubleStepPawn; 
    record.previousLastDoubleStepRow = previousLastDoubleStepRow; 
    record.previousLastDoubleStepCol = previousLastDoubleStepCol;

    record.movingPieceHadMoved = currPiece->getHasMoved(); 
}

/* 
    - Moves a piece from one square to another and updates the board layout.
    - Transfers ownership of the piece pointer to the destination square
      and clears the originating square.
    - Marks the piece as having moved, which is required for castling,
      en passant, and other state-dependent chess rules.
*/
void Board::updateBoard(int fromRow, int fromCol, int toRow, int toCol){
    board[toRow][toCol] = board[fromRow][fromCol]; 
    board[fromRow][fromCol] = nullptr; 
    board[toRow][toCol]->notifyMoved(); 
}

/* 
    - Temporarily applies a move to the board for move validation and
      check detection purposes.
    - Supports special move simulation including en passant captures and
      castling without modifying permanent game state.
    - Used in conjunction with undoMove() to evaluate hypothetical board
      positions before committing a move.
*/
void Board::applyMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, bool isCastle, int rookFromCol, int rookToCol, Piece* rook) {
    board[toRow][toCol] = moving;
    board[fromRow][fromCol] = nullptr;

    if (isEnPassant) board[fromRow][toCol] = nullptr;
    if (isCastle) {
        board[fromRow][rookToCol] = rook;  
        board[fromRow][rookFromCol] = nullptr; 
    }
}

/* 
    - Restores the board to its original state after a simulated move.
    - Reverses temporary board modifications including en passant captures
      and castling-related rook movement.
    - Used in conjunction with applyMove() to safely evaluate hypothetical
      positions without affecting the actual game state.
*/
void Board::undoMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, bool isCastle, int rookFromCol, int rookToCol, Piece* rook) {
    board[fromRow][fromCol] = moving;
    board[toRow][toCol] = destination;

    if (isEnPassant) board[fromRow][toCol] = epPawn;
    if (isCastle) {
        board[fromRow][rookFromCol] = rook; 
        board[fromRow][rookToCol] = nullptr; 
    }
}

/* 
    - Determines whether the specified player's king is currently under attack.
    - Locates the king on the board and checks whether its square is
      threatened by any opposing piece.
    - Used in move validation, castling checks, checkmate detection,
      and stalemate detection.
*/
bool Board::isKingInCheck(const string& color){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            Piece* p = board[row][col]; 
            if (!p) continue; 
            if (tolower(p->getSymbol()) == 'k' && p->getColor() == color){
                return isSquareAttacked(row, col, color); 
            }
        }
    } return false; 
}

/* 
    - Determines whether the specified player has at least one legal move
      available in the current position.
    - Iterates through every possible move for every piece of the given
      color and validates each candidate according to the engine's rules.
    - Used to distinguish between checkmate, stalemate, and normal game
      states.
*/
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
                    
                    return true; 
                }
            }
        }
    } return false; 
}

/* 
    - Determines whether a square is currently threatened by any opposing
      piece.
    - Evaluates all potential attacking pieces and verifies that they can
      legally reach the target square according to movement and path rules.
    - Forms the foundation of check detection, castling validation, and
      legal move analysis throughout the engine.
*/
bool Board::isSquareAttacked(int row, int col, const string& color){
    string attacker = (color == "white") ? "black" : "white"; 

    for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            Piece* p = this->board[r][c]; 

            if (!p) continue; 
            if (p->getColor() != attacker) continue; 
            if (!p->isMoveShapeValid(r, c, row, col)) continue; 
            if (isPathClear(r, c, row, col) != MoveResult::SUCCESS) continue;
            if (p->getSymbol() == 'p' && c == col) continue;  // Pawns attack diagonally and do not threaten squares directly ahead.
            return true; 
        }
    } return false; 
}

/*
    Generates a position fingerprint for threefold repetition detection.

    The fingerprint contains:
    - Side to move
    - Piece placement
    - Castling rights
    - En passant target square

    Two positions that produce identical fingerprints are considered
    equivalent for the purposes of repetition tracking.
*/
string Board::generateFingerPrint() const {
    string fingerprint = ""; 

    // Turn Determiner: 
    fingerprint += (whiteToMove) ? "w|" : "b|"; 

    // Piece Determiner: 
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++){
            Piece* p = board[row][col];
            if (p) fingerprint += p->getSymbol(); 
            else fingerprint += "."; 
        }
    } fingerprint += "|"; 

    // Castling Determiner [WQ WK BQ BK]
    Piece* whiteKing = board[0][4]; 
    Piece* blackKing = board[7][4]; 

    Piece* whiteQueenSide = board[0][0]; 
    Piece* whiteKingSide = board[0][7];

    Piece* blackQueenSide = board[7][0]; 
    Piece* blackKingSide = board[7][7]; 

    // White
    if (!whiteKing || tolower(whiteKing->getSymbol()) != 'k' || whiteKing->getHasMoved()) fingerprint += "00"; 
    else {
        if (!whiteQueenSide || tolower(whiteQueenSide->getSymbol()) != 'r' || whiteQueenSide->getHasMoved()) fingerprint += "0"; 
        else fingerprint += "1"; 

        if (!whiteKingSide || tolower(whiteKingSide->getSymbol()) != 'r' || whiteKingSide->getHasMoved()) fingerprint += "0"; 
        else fingerprint += "1"; 
    }
    
    // Black
    if (!blackKing || tolower(blackKing->getSymbol()) != 'k' || blackKing->getHasMoved()) fingerprint += "00"; 
    else {
        if (!blackQueenSide || tolower(blackQueenSide->getSymbol()) != 'r' || blackQueenSide->getHasMoved()) fingerprint += "0"; 
        else fingerprint += "1"; 

        if (!blackKingSide || tolower(blackKingSide->getSymbol()) != 'r' || blackKingSide->getHasMoved()) fingerprint += "0"; 
        else fingerprint += "1"; 
    }

    fingerprint += "|"; 

    // En Passant Determiner
    if (!lastDoubleStepPawn) fingerprint += "-|"; 
    else if (lastDoubleStepPawn->getColor() == "white") fingerprint += to_string(lastDoubleStepRow + 1) + to_string(lastDoubleStepCol) + "|"; 
    else fingerprint += to_string(lastDoubleStepRow - 1) + to_string(lastDoubleStepCol) + "|"; 
    
    return fingerprint; 
}