#include "board.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std; 

bool convertSquare(const string& sq, int& row, int& col);
bool validInput(const string& from, const string& to, int& fromRow, int& fromCol, int& toRow, int& toCol); 
bool trulyYourPiece(Board& b, int fromRow, int fromCol, int currentPlayer);
string errorTranslator(MoveResult result); 
void clearScreen(); 

int main(){
    cout << "\033[?1049h";
    Board game; 
    game.boardInit(); 

    string originalString = ""; 
    string from = ""; string to = ""; 

    int currentPlayer = 1; // NOTE: UI handles which player's turn it is
    string statusMessage = ""; 

    while (true){
        clearScreen(); 
        cout << "======================================================================================" << endl;
        game.printBoard(); 
        cout << "======================================================================================" << endl;
        if (!statusMessage.empty()) cout << "INVALID MOVE! " << statusMessage << "\n" << endl; 
        statusMessage = ""; 

        string user = (currentPlayer) ? "White to Move" : "Black to Move"; cout << user << endl;
        cout << "Commands: 'undo' OR 0 0 to quit" << endl; 
        cout << "Enter Move (e.g. e2 e4): "; 

        getline(cin, originalString); 

        if (originalString == "undo"){
            if (game.undoLastMove()) currentPlayer = !currentPlayer; 
            else statusMessage = "No moves to undo"; 
            continue; 
        }

        else {
            istringstream iss(originalString); 
            iss >> from >> to; 
        }

        int fromRow, fromCol, toRow, toCol; 

        if (from == "0" && to == "0") break; 

        if (!validInput(from, to, fromRow, fromCol, toRow, toCol)) {
            statusMessage = "Invalid Input!"; 
            continue; 
        }

        if (!trulyYourPiece(game, fromRow, fromCol, currentPlayer)){
            statusMessage = "That's not your piece!";
            continue; 
        }

        MoveResult result = game.movePiece(fromRow, fromCol, toRow, toCol); 
        if (result != MoveResult::SUCCESS) {
            statusMessage =  errorTranslator(result); 
            continue; 
        }

        if (game.isCheckMate("white")) {
            statusMessage = "GAME OVER. Black wins!"; 
            break; 
        }

        if (game.isCheckMate("black")) {
            statusMessage = "GAME OVER. White wins!"; 
            break; 
        }

        if (game.isStaleMate("white") || game.isStaleMate("black")) {
            statusMessage = "GAME OVER. It's a draw!"; 
            break; 
        }

        currentPlayer = !currentPlayer; 

        
    }

    

    cout << "\033[?1049l";
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

    return (p && p->getColor() == color); 
}

void clearScreen(){
    cout << "\033[3J\033[2J\033[H";
}