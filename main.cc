#include "board.h"
#include <iostream>
#include <string>

using namespace std; 

bool convertSquare(const string& sq, int& row, int& col);
bool validInput(const string& from, const string& to, int& fromRow, int& fromCol, int& toRow, int& toCol); 
bool trulyYourPiece(Board& b, int fromRow, int fromCol, int currentPlayer);
string errorTranslator(MoveResult result); 

int main(){
    Board game; 
    game.boardInit(); 

    cout << "======================================================================================" << endl;
    game.printBoard(); 

    string from = ""; string to = "";
    int currentPlayer = 1; 

    while (true){

        cout << "======================================================================================" << endl;
        string user = (currentPlayer) ? "White to Move" : "Black to Move"; cout << user << endl;
        
        cout << "Enter Move (e.g. e2 e4): "; 
        cin >> from >> to; cout << endl; 

        int fromRow, fromCol, toRow, toCol; 

        if (from == "0" && to == "0") break; 

        if (!validInput(from, to, fromRow, fromCol, toRow, toCol)) {
            game.printBoard(); 
            cout << "\nInvalid Input!" << endl; 
            continue; 
        }

        if (!trulyYourPiece(game, fromRow, fromCol, currentPlayer)){
            game.printBoard(); 
            cout << "\nThat's not your piece!" << endl; 
            continue; 
        }

        MoveResult result = game.movePiece(fromRow, fromCol, toRow, toCol); 
        if (result != MoveResult::SUCCESS) {
            game.printBoard(); 
            cout << "\nINVALID MOVE! " << errorTranslator(result) << endl; 
            continue; 
        }

        game.printBoard(); 

        if (game.isCheckMate("white")) {
            cout << "Game Over. Black wins!"; 
            break; 
        }

        if (game.isCheckMate("black")) {
            cout << "Game Over. White wins!"; 
            break; 
        }

        if (game.isStaleMate("white") || game.isStaleMate("black")) {
            cout << "Game Over. It's a draw!"; 
            break; 
        }

        currentPlayer = !currentPlayer; 
    }

    cout << "======================================================================================" << endl;

    return 0; 
}

string errorTranslator(MoveResult result){
    switch (result){
        case MoveResult::INVALID_ROW_COL: 
            return "Invalid Row and/or Column."; 
        case MoveResult::NON_EXISTENT_PIECE: 
            return "No piece exists on this square.";
        case MoveResult::INVALID_PIECE_MOVE: 
            return "Piece on this square cannot move to the square you want it to."; 
        case MoveResult::INVALID_PAWN_CAPTURE:
            return "Pawn cannot capture that particular square."; 
        case MoveResult::PATH_NOT_CLEAR:
            return "Path is not clear to move.";
        case MoveResult::ILLEGAL_FRIENDLY_FIRE: 
            return "Cannot capture your own piece.";
        case MoveResult::KING_IN_CHECK:
            return "King is in check."; 
        case MoveResult::LEAVES_KING_IN_CHECK: 
            return "That move would leave your King in check."; 
        case MoveResult::ERR_CASTLING_KING_MOVED: 
            return "Castling Illegal as the King has moved.";
        case MoveResult::ERR_CASTLING_ROOK_MOVED:
            return "Castling Illegal as the Rook has moved.";
        default: 
            return "SHOULD NOT BE PRINTED"; 
    }
}

bool convertSquare(const string& sq, int& row, int& col){
    char file_char = tolower(sq[0]); 
    int rank_int = sq[1] - '1';

    if (file_char < 'a' || file_char > 'h') return false; 
    if (rank_int < 0 || rank_int > 7) return false; 

    col = file_char - 'a'; 
    row = rank_int; 

    // cout << "(" << row << ", " << col << ")" << endl; 

    return true; 
}

bool validInput(const string& from, const string& to, int& fromRow, int& fromCol, int& toRow, int& toCol){
    if (!(from.length() == 2 && to.length() == 2)) return false; 
    
    if (!convertSquare(from, fromRow, fromCol) || !convertSquare(to, toRow, toCol)) return false; 

    return true; 
}

bool trulyYourPiece(Board& b, int fromRow, int fromCol, int currentPlayer){
    Piece* p = b.getPiece(fromRow, fromCol);
    string color = (currentPlayer) ? "white" : "black";

    if (p && p->getColor() == color) return true; 
    else return false; 
}