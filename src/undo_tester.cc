#include "board.h"
#include "pawn.h"
#include "knight.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"
#include "king.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// --- Helper Functions ---

/**
 * Validates that the current board state exactly matches a saved snapshot of piece pointers.
 */
bool areBoardsEqual(Board& b1, Piece* savedBoard[8][8]) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (b1.getPiece(r, c) != savedBoard[r][c]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Saves the current state of the board (piece pointers) into a 2D array.
 */
void saveBoard(Board& b, Piece* savedBoard[8][8]) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            savedBoard[r][c] = b.getPiece(r, c);
        }
    }
}

/**
 * Lightweight helper to validate piece metadata restoration (e.g., hasMoved state).
 */
bool checkPieceMetadata(Piece* p, bool expectedHasMoved) {
    if (!p) return true; // Non-existent pieces don't have metadata to check
    return p->getHasMoved() == expectedHasMoved;
}

/**
 * Standard test runner for a single move and undo operation.
 */
void runUndoTest(Board& b, int fromRow, int fromCol, int toRow, int toCol, const string& description, int& counter) {
    Piece* savedBoard[8][8];
    saveBoard(b, savedBoard);

    cout << "Testing: " << description << endl;

    MoveResult res = b.movePiece(fromRow, fromCol, toRow, toCol);
    if (res != MoveResult::SUCCESS) {
        cout << "FAILED: Move was not successful (Result: " << (int)res << ")" << endl;
        return;
    }

    if (!b.undoLastMove()) {
        cout << "FAILED: undoLastMove returned false" << endl;
        return;
    }

    if (areBoardsEqual(b, savedBoard)) {
        cout << "SUCCESS: State restored" << endl;
        counter++;
    } else {
        cout << "FAILED: State NOT restored" << endl;
        b.printBoard();
    }
    cout << "------------------------------------------" << endl;
}

// --- Original Test Cases ---

void basicUndoTest(Board& b, int& counter) {
    b.clearBoard();
    b.placePiece(1, 0, new Pawn("white"));
    runUndoTest(b, 1, 0, 2, 0, "Basic Pawn Move Undo", counter);
}

void captureUndoTest(Board& b, int& counter) {
    b.clearBoard();
    b.placePiece(1, 0, new Pawn("white"));
    b.placePiece(2, 1, new Pawn("black"));
    runUndoTest(b, 1, 0, 2, 1, "Pawn Capture Undo", counter);
}

void enPassantUndoTest(Board& b, int& counter) {
    b.clearBoard();
    b.placePiece(4, 3, new Pawn("white"));
    b.placePiece(6, 4, new Pawn("black"));
    b.movePiece(6, 4, 4, 4); // Black double step
    runUndoTest(b, 4, 3, 5, 4, "En Passant Undo", counter);
}

void castlingUndoTest(Board& b, int& counter) {
    // King-side
    b.clearBoard();
    b.placePiece(0, 4, new King("white"));
    b.placePiece(0, 7, new Rook("white"));
    runUndoTest(b, 0, 4, 0, 6, "King-side Castle Undo", counter);

    // Queen-side
    b.clearBoard();
    b.placePiece(0, 4, new King("white"));
    b.placePiece(0, 0, new Rook("white"));
    runUndoTest(b, 0, 4, 0, 2, "Queen-side Castle Undo", counter);
}

void promotionUndoTest(Board& b, int& counter) {
    b.clearBoard();
    b.placePiece(6, 0, new Pawn("white"));
    runUndoTest(b, 6, 0, 7, 0, "Pawn Promotion Undo", counter);
}

// --- Extended Test Cases ---

/**
 * 1. Multi-Undo Chain Test
 * Executes 3 legal moves in sequence, undos all of them, and verifies full restoration.
 */
void multiUndoChainTest(Board& b, int& counter) {
    b.clearBoard();
    Piece* wPawn = new Pawn("white");
    Piece* bPawn = new Pawn("black");
    b.placePiece(1, 4, wPawn); // e2
    b.placePiece(6, 3, bPawn); // d7

    Piece* savedBoard[8][8];
    saveBoard(b, savedBoard);

    cout << "Testing: Multi-Undo Chain (3 moves)" << endl;

    // Sequence: e2-e4, d7-d5, e4xd5
    b.movePiece(1, 4, 3, 4); 
    b.movePiece(6, 3, 4, 3);
    b.movePiece(3, 4, 4, 3);

    // Undo all moves in the chain
    b.undoLastMove();
    b.undoLastMove();
    b.undoLastMove();

    if (areBoardsEqual(b, savedBoard)) {
        cout << "SUCCESS: Full chain state restored" << endl;
        counter++;
    } else {
        cout << "FAILED: Chain state NOT restored" << endl;
        b.printBoard();
    }
    cout << "------------------------------------------" << endl;
}

/**
 * 2. Castle Rights Restoration Test
 * Validates that the 'hasMoved' state of the King and Rook is correctly reset after an undo,
 * ensuring castling rights are preserved.
 */
void castleRightsRestorationTest(Board& b, int& counter) {
    b.clearBoard();
    King* k = new King("white");
    Rook* r = new Rook("white");
    b.placePiece(0, 4, k);
    b.placePiece(0, 7, r);

    // King restoration check
    cout << "Testing: Castle Rights Restoration (King)" << endl;
    b.movePiece(0, 4, 0, 5);
    b.undoLastMove();
    if (checkPieceMetadata(k, false) && b.getPiece(0, 4) == k) {
        cout << "SUCCESS: King hasMoved restored" << endl;
        counter++;
    } else {
        cout << "FAILED: King hasMoved NOT restored" << endl;
    }

    // Rook restoration check
    cout << "Testing: Castle Rights Restoration (Rook)" << endl;
    b.movePiece(0, 7, 0, 6);
    b.undoLastMove();
    if (checkPieceMetadata(r, false) && b.getPiece(0, 7) == r) {
        cout << "SUCCESS: Rook hasMoved restored" << endl;
        counter++;
    } else {
        cout << "FAILED: Rook hasMoved NOT restored" << endl;
    }
    cout << "------------------------------------------" << endl;
}

/**
 * 3. Promotion Capture Undo Test
 * Verifies that undoing a promotion that was also a capture correctly restores
 * the original pawn, the captured piece, and removes the promoted piece.
 */
void promotionCaptureUndoTest(Board& b, int& counter) {
    b.clearBoard();
    Pawn* p = new Pawn("white");
    Knight* target = new Knight("black");
    b.placePiece(6, 0, p);
    b.placePiece(7, 1, target);

    runUndoTest(b, 6, 0, 7, 1, "Promotion Capture Undo", counter);
}

int main() {
    Board b;
    int counter = 0;
    int totalTests = 10; // 6 original + 4 new (chain, king rights, rook rights, prom-capture)

    // Run existing tests
    basicUndoTest(b, counter);
    captureUndoTest(b, counter);
    enPassantUndoTest(b, counter);
    castlingUndoTest(b, counter); 
    promotionUndoTest(b, counter);

    // Run newly added tests
    multiUndoChainTest(b, counter);
    castleRightsRestorationTest(b, counter);
    promotionCaptureUndoTest(b, counter);

    cout << "Undo Tests Passed: " << counter << "/" << totalTests << endl;

    return (counter == totalTests) ? 0 : 1;
}
