#include "board.h"
#include <iostream>
using namespace std;

bool tryMove(Board& board, int fromRow, int fromCol, int toRow, int toCol, const string& description) {
    int result = board.movePiece(fromRow, fromCol, toRow, toCol);
    if (result == 0) {
        cout << "SUCCESS: " << description << endl;
    } else if (result == 1) {
        cout << "FAILED (invalid move): " << description << endl;
    } else if (result == 2) {
        cout << "FAILED (path blocked): " << description << endl;
    }
    board.printBoard();
    cout << "--------------------" << endl;
    return result == 0;
}

int main() {
    Board board;

    cout << "Initial Board:" << endl;
    board.printBoard();
    cout << "====================" << endl;

    // --- Pawns ---
    tryMove(board, 6, 0, 5, 0, "Black pawn moves 1 forward");
    tryMove(board, 6, 1, 4, 1, "Black pawn moves 2 forward from starting position");
    tryMove(board, 6, 1, 4, 0, "Illegal diagonal move to empty space"); // should fail
    tryMove(board, 1, 0, 3, 0, "White pawn moves 2 forward from starting position");

    // --- Rooks ---
    tryMove(board, 7, 0, 5, 0, "White rook vertical move (should fail, path blocked)");
    tryMove(board, 7, 0, 5, 3, "Illegal rook diagonal move"); // should fail

    // --- Knights ---
    tryMove(board, 7, 1, 5, 2, "Black knight L-shape move over pieces");
    tryMove(board, 7, 1, 6, 3, "Illegal knight move"); // should fail

    // --- Bishops ---
    tryMove(board, 7, 2, 5, 4, "Black bishop diagonal move (path blocked, should fail)");
    tryMove(board, 0, 2, 2, 4, "White bishop diagonal move (if path clear)"); // adjust if needed

    // --- Queen ---
    tryMove(board, 7, 3, 5, 5, "White queen diagonal move");
    tryMove(board, 7, 3, 7, 5, "Illegal queen horizontal move (path blocked)");

    // --- King ---
    tryMove(board, 7, 4, 6, 4, "White king moves 1 square forward");
    tryMove(board, 7, 4, 5, 4, "Illegal king move >1 square");

    // --- Captures ---
    tryMove(board, 5, 0, 4, 0, "Pawn moves forward to empty square");
    tryMove(board, 3, 0, 4, 1, "Pawn captures opponent diagonally"); // make sure an opponent is there for capture

    cout << "Test sequence complete!" << endl;
    return 0;
}
