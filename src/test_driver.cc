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

void runTest(Board& b, int fromRow, int fromCol, int toRow, int toCol, const string& description, MoveResult expected, int& counter);
void pawnTester(Board& b1); 
void knightTester(Board& b1); 
void bishopTester(Board& b1); 
void rookTester(Board& b1); 
void queenTester(Board& b1);
void kingTester(Board& b1);

void promotionTester(Board& b1);
void enPassantTester(Board& b1); 
void castleTester(Board& b1);

void pinTester(Board& b1); 
void checkmateTester(Board& b1);
void stalemateTester(Board& b1); 

int main(){
    Board b1;

    // Pawns
    // pawnTester(b1);
    
    // Knights
    // knightTester(b1); 

    // Bishops
    // bishopTester(b1); 

    // Rooks
    // rookTester(b1); 

    // Queens
    // queenTester(b1);

    // Kings
    // kingTester(b1); 
    
    //Promotions
    // promotionTester(b1);

    // En Passant
    // enPassantTester(b1); 

    // Castling
    castleTester(b1); 

    // Pins
    // pinTester(b1); 

    // Checkmates
    // checkmateTester(b1); 

    // Stalemates
    // stalemateTester(b1); 

    return 0;
}

void pinTester(Board& b1){
    int counter = 0; 

    b1.clearBoard(); 

    b1.placePiece(1, 3, new Queen("white")); b1.placePiece(0, 3, new King("white")); b1.placePiece(7, 3, new Rook("black")); 
    b1.printBoard(); cout << endl; 

    runTest(b1, 1, 3, 1, 7, "Pin Tester: ", MoveResult::LEAVES_KING_IN_CHECK, counter); 
    runTest(b1, 0, 3, 0, 4, "Make sure King can move: ", MoveResult::SUCCESS, counter); 
    runTest(b1, 1, 3, 1, 7, "Pin Tester: ", MoveResult::SUCCESS, counter); 
    
    cout << "Pin Test: "<< counter << "/3" << endl; 
}

void checkmateTester(Board& b1){
    int counter = 0; 

    b1.clearBoard(); 
    b1.placePiece(2, 0, new Rook("white")); b1.placePiece(1, 1, new Queen("white"));
    b1.placePiece(0, 7, new King("black")); b1.printBoard(); cout << endl;

    runTest(b1, 2, 0, 0, 0, "Checkmate Tester: ", MoveResult::SUCCESS, counter); 
    if (b1.isCheckMate("black")) counter++; 
    
    cout << "Checkmate Test: "<< counter / 2 << "/1" << endl; 
}

void stalemateTester(Board& b1){
    int counter = 0; 

    b1.clearBoard(); 
    b1.placePiece(2, 0, new King("white")); b1.placePiece(2, 7, new Queen("white"));
    b1.placePiece(0, 0, new King("black")); b1.printBoard(); cout << endl;

    runTest(b1, 2, 7, 2, 1, "Stalemate Tester: ", MoveResult::SUCCESS, counter); 
    if (b1.isStaleMate("black")) counter++; 
    
    cout << "Stalemate Test: "<< counter / 2 << "/1" << endl; 
}


void runTest(Board& b1, int fromRow, int fromCol, int toRow, int toCol, const string& description, MoveResult expected, int& counter){
    MoveResult value = b1.movePiece(fromRow, fromCol, toRow, toCol); 

    cout << description << " -> should return " << (int)expected << ": " << (int)value << endl; 
    counter += (value == expected) ? 1 : 0; 

    b1.printBoard(); cout << endl; 
}

// 1) PAWNS
void pawnTester(Board& b1){
     
    int counter = 0; 

    // TEST 1A: Pawn moves one square forward
    b1.clearBoard(); b1.placePiece(1, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 1, 0, 2, 0, "Test 1A [Pawn Moves One Square]", MoveResult::SUCCESS, counter);

    // TEST 1B: Pawn moves two squares forward
    b1.clearBoard(); b1.placePiece(1, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 1, 0, 3, 0, "Test 1B [Pawn Moves Two Squares]", MoveResult::SUCCESS, counter);

    // TEST 1C: Pawn cannot move two squares forward twice
    runTest(b1, 3, 0, 5, 0, "Test 1C [Pawn Moves Two Squares AGAIN!]", MoveResult::INVALID_PIECE_MOVE, counter);
 
    // TEST 1D: Pawn cannot move into occupied square
    b1.clearBoard(); b1.placePiece(1, 0, new Pawn("white")); b1.placePiece(2, 0, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 1, 0, 2, 0, "Test 1D [Pawn Blocked]", MoveResult::PATH_NOT_CLEAR, counter); 

    // TEST 1E: Pawn captures diagonally
    b1.clearBoard(); b1.placePiece(1, 0, new Pawn("white")); b1.placePiece(2, 1, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 1, 0, 2, 1, "Test 1E [Pawn Capture]", MoveResult::SUCCESS, counter);

    // TEST 1F: Pawn cannot move diagonally without enemy piece
    runTest(b1, 2, 1, 3, 2, "Test 1F [Illegal Pawn Capture]", MoveResult::INVALID_PAWN_CAPTURE, counter);
    
    cout << "Generic Pawn Tests: "<< counter << "/6" << endl; 
}

// 2. KNIGHTS
void knightTester(Board& b1){
    int counter = 0; 

    // TEST 2A: Basic Knight Moves
    b1.clearBoard(); b1.placePiece(3, 3, new Knight("white")); b1.printBoard(); cout << endl; 

    vector<pair<int, int>> knightMoves = {{5,4}, {5,2}, {1,4}, {1,2}, {4,5}, {4,1}, {2,5}, {2,1}}; 
    for (const auto& move: knightMoves){
        runTest(b1, 3, 3, move.first, move.second, "Test 2A [Check Valid Knight Moves]", MoveResult::SUCCESS, counter);
        b1.movePiece(move.first, move.second, 3, 3);
    }

    // TEST 2B: Out of Bounds Check
    b1.clearBoard(); b1.placePiece(0, 0, new Knight("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, -2, -1, "Test 2B [Out of Bounds Check]", MoveResult::INVALID_ROW_COL, counter);

    runTest(b1, 0, 0, 2, 1, "Test 2B [Out of Bounds Check]", MoveResult::SUCCESS, counter);

    // TEST 2C: Capturing Enemy Pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Knight("white")); b1.placePiece(2, 1, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 2, 1, "Test 2C [Capturing Enemy Pieces]", MoveResult::SUCCESS, counter);

    // TEST 2D: Illegal Friendly Fire
    b1.clearBoard(); b1.placePiece(0, 0, new Knight("white")); b1.placePiece(2, 1, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 2, 1, "Test 2D [Illegal Friendly Fire]", MoveResult::ILLEGAL_FRIENDLY_FIRE, counter);

    // TEST 2E: Jumping Over Pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Knight("white")); 
    b1.placePiece(1, 1, new Pawn("white")); b1.placePiece(1, 0, new Pawn("white")); b1.placePiece(0, 1, new Knight("white")); 
    b1.printBoard(); cout << endl; 

    runTest(b1, 0, 0, 2, 1, "Test 2E [Jumping Knight]", MoveResult::SUCCESS, counter);

    // TEST 2F: Illegal Knight Move
    b1.clearBoard(); b1.placePiece(3, 3, new Knight("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 3, 3, 4, 4, "Test 2F [Illegal Knight Move]", MoveResult::INVALID_PIECE_MOVE, counter);

    cout << "Generic Knight Tests: "<< counter << "/14" << endl; 
}

// 3. BISHOPS
void bishopTester(Board& b1){
    int counter = 0; 

    // TEST 3A: Basic Bishop Moves
    b1.clearBoard(); b1.placePiece(3, 3, new Bishop("white")); b1.printBoard(); cout << endl; 

    vector<pair<int, int>> bishopMoves = {{0,6}, {0,0}, {7,7}, {6,0}}; 
    for (const auto& move: bishopMoves){
        runTest(b1, 3, 3, move.first, move.second, "Test 3A [Check Valid Bishop Moves]", MoveResult::SUCCESS, counter);
        b1.movePiece(move.first, move.second, 3, 3);
    }

    // TEST 3B: Out of Bounds Check
    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, -1, -1, "Test 3B [Out of Bounds Check]", MoveResult::INVALID_ROW_COL, counter);

    runTest(b1, 0, 0, 1, 1, "Test 3B [Out of Bounds Check]", MoveResult::SUCCESS, counter);

    // TEST 3C: Capturing Enemy Pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.placePiece(7, 7, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 7, "Test 3C [Capturing Enemy Pieces]", MoveResult::SUCCESS, counter);

    // TEST 3D: Illegal Friendly Fire
    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.placePiece(6, 6, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 6, 6, "Test 3D [Illegal Friendly Fire]", MoveResult::ILLEGAL_FRIENDLY_FIRE, counter);

    // TEST 3E: Obstructing pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.placePiece(4, 4, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 7, "Test 3E [Obstructing Pieces (Same Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.placePiece(4, 4, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 7, "Test 3E [Obstructing Pieces (Opposing Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    // TEST 3F: Illegal Bishop Moves
    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 0, "Test 3F [Illegal Bishop Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Bishop("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 0, 7, "Test 3F [Illegal Bishop Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    cout << "Generic Bishop Tests: "<< counter << "/12" << endl; 
}

// 4. ROOKS
void rookTester(Board& b1){
    int counter = 0; 

    // TEST 4A: Basic Rook Moves
    b1.clearBoard(); b1.placePiece(3, 3, new Rook("white")); b1.printBoard(); cout << endl; 

    vector<pair<int, int>> rookMoves = {{3,0}, {3,7}, {7,3}, {0,3}}; 
    for (const auto& move: rookMoves){
        runTest(b1, 3, 3, move.first, move.second, "Test 4A [Check Valid Rook Moves]", MoveResult::SUCCESS, counter);
        b1.movePiece(move.first, move.second, 3, 3);
    }

    // TEST 4B: Out of Bounds Check
    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, -7, 0, "Test 4B [Out of Bounds Check]", MoveResult::INVALID_ROW_COL, counter);

    runTest(b1, 0, 0, 3, 0, "Test 4B [Out of Bounds Check]", MoveResult::SUCCESS, counter);

    // TEST 4C: Capturing Enemy Pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.placePiece(4, 0, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 4, 0, "Test 4C [Capturing Enemy Pieces]", MoveResult::SUCCESS, counter);

    // TEST 4D: Illegal Friendly Fire
    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.placePiece(4, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 4, 0, "Test 4D [Illegal Friendly Fire]", MoveResult::ILLEGAL_FRIENDLY_FIRE, counter);

     // TEST 4E: Obstructing pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.placePiece(5, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 0, "Test 4E [Obstructing Pieces (Same Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.placePiece(5, 0, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 0, "Test 4E [Obstructing Pieces (Opposing Colour)]", MoveResult::PATH_NOT_CLEAR, counter);


    // TEST 4F: Illegal Rook Moves
    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 6, 6, "Test 4F [Illegal Rook Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Rook("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 1, 2, "Test 4F [Illegal Rook Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    cout << "Generic Rook Tests: "<< counter << "/12" << endl; 
}

// 5. QUEENS
void queenTester(Board& b1){
    int counter = 0; 

    // TEST 5A: Basic Queen Moves
    b1.clearBoard(); b1.placePiece(3, 3, new Queen("white")); b1.printBoard(); cout << endl; 

    vector<pair<int, int>> queenMoves = {{0,6}, {0,0}, {7,7}, {6,0}, {3,0}, {3,7}, {7,3}, {0,3}}; 
    for (const auto& move: queenMoves){
        runTest(b1, 3, 3, move.first, move.second, "Test 5A [Check Valid Queen Moves]", MoveResult::SUCCESS, counter);
        b1.movePiece(move.first, move.second, 3, 3);
    }

    // TEST 5B: Out of Bounds Check
    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.printBoard(); cout << endl; 

    runTest(b1, 0, 0, -7, 0, "Test 5B [Out of Bounds Check]", MoveResult::INVALID_ROW_COL, counter);
    runTest(b1, 0, 0, -1, -1, "Test 5B [Out of Bounds Check]", MoveResult::INVALID_ROW_COL, counter);

    runTest(b1, 0, 0, 3, 0, "Test 5B [Out of Bounds Check]", MoveResult::SUCCESS, counter);
    b1.movePiece(3, 0, 0, 0); 
    runTest(b1, 0, 0, 1, 1, "Test 5B [Out of Bounds Check]", MoveResult::SUCCESS, counter);

    // TEST 5C: Capturing Enemy Pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(4, 0, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 4, 0, "Test 5C [Capturing Enemy Pieces]", MoveResult::SUCCESS, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(7, 7, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 7, "Test 5C [Capturing Enemy Pieces]", MoveResult::SUCCESS, counter);

    // TEST 5D: Illegal Friendly Fire
    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(4, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 4, 0, "Test 5D [Illegal Friendly Fire]", MoveResult::ILLEGAL_FRIENDLY_FIRE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(6, 6, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 6, 6, "Test 5D [Illegal Friendly Fire]", MoveResult::ILLEGAL_FRIENDLY_FIRE, counter);

    // TEST 5E: Obstructing pieces
    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(5, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 0, "Test 5E [Obstructing Pieces (Same Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(5, 0, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 0, "Test 5E [Obstructing Pieces (Opposing Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(4, 4, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 7, "Test 5E [Obstructing Pieces (Same Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.placePiece(4, 4, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 7, "Test 5E [Obstructing Pieces (Opposing Colour)]", MoveResult::PATH_NOT_CLEAR, counter);

    // TEST 5F: Illegal Queen Moves
    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 6, 5, "Test 5F [Illegal Queen Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 1, 2, "Test 5F [Illegal Queen Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 7, 1, "Test 5F [Illegal Queen Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new Queen("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 3, 7, "Test 5F [Illegal Queen Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    cout << "Generic Queen Tests: "<< counter << "/24" << endl; 
}

// 6. KINGS
void kingTester(Board& b1){
    int counter = 0; 

     // TEST 6A: Basic King Moves
    b1.clearBoard(); b1.placePiece(3, 3, new King("white")); b1.printBoard(); cout << endl; 

    vector<pair<int, int>> kingMoves = {{3,2}, {3,4}, {4,3}, {2,3}, {2,2}, {2,4}, {4,4}, {4,2}}; 
    for (const auto& move: kingMoves){
        runTest(b1, 3, 3, move.first, move.second, "Test 6A [Check Valid King Moves]", MoveResult::SUCCESS, counter);
        b1.movePiece(move.first, move.second, 3, 3);
    }

    // TEST 6B: Out of Bounds Check
    b1.clearBoard(); b1.placePiece(0, 0, new King("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, -1, -1, "Test 6B [Out of Bounds Check]", MoveResult::INVALID_ROW_COL, counter);

    runTest(b1, 0, 0, 1, 1, "Test 6B [Out of Bounds Check]", MoveResult::SUCCESS, counter);

    // TEST 6C: Capturing Enemy Pieces
    b1.clearBoard(); b1.placePiece(0, 0, new King("white")); b1.placePiece(1, 0, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 1, 0, "Test 6C [Capturing Enemy Pieces]", MoveResult::SUCCESS, counter);

    // TEST 6D: Illegal Friendly Fire
    b1.clearBoard(); b1.placePiece(0, 0, new King("white")); b1.placePiece(1, 0, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 1, 0, "Test 6D [Illegal Friendly Fire]", MoveResult::ILLEGAL_FRIENDLY_FIRE, counter);

    // TEST 6E: Illegal King Moves
    b1.clearBoard(); b1.placePiece(0, 0, new King("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 2, 0, "Test 6E [Illegal King Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    b1.clearBoard(); b1.placePiece(0, 0, new King("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 0, 0, 0, 2, "Test 6E [Illegal King Moves]", MoveResult::INVALID_PIECE_MOVE, counter);

    cout << "Generic King Tests: "<< counter << "/14" << endl; 
}

// 7. PROMOTIONS [ONLY QUEENS FOR NOW]
void promotionTester(Board& b1){
    int counter = 0; 

    // TEST 7A: Basic Pawn Promotion [No Capture]
    b1.clearBoard(); b1.placePiece(6, 3, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 6, 3, 7, 3, "Test 7A [Basic Pawn Promotion]", MoveResult::SUCCESS, counter); 

    Piece* promoted = b1.getPiece(7,3);
    if (promoted && tolower(promoted->getSymbol()) == 'q' && promoted->getColor() == "white") counter++; 

    b1.clearBoard(); b1.placePiece(1, 3, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 1, 3, 0, 3, "Test 7A [Basic Pawn Promotion]", MoveResult::SUCCESS, counter);

    promoted = b1.getPiece(0,3);
    if (promoted && tolower(promoted->getSymbol()) == 'q' && promoted->getColor() == "black") counter++; 

    // TEST 7B: No Promotion BEFORE Last Rank

    b1.clearBoard(); b1.placePiece(5, 3, new Pawn("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 5, 3, 6, 3, "Test 7B [No Promotion BEFORE Last Rank]", MoveResult::SUCCESS, counter);

    promoted = b1.getPiece(6,3);
    if (promoted && tolower(promoted->getSymbol()) == 'p' && promoted->getColor() == "white") counter++; 

    b1.clearBoard(); b1.placePiece(2, 3, new Pawn("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 2, 3, 1, 3, "Test 7B [No Promotion BEFORE Last Rank]", MoveResult::SUCCESS, counter);

    promoted = b1.getPiece(1,3);
    if (promoted && tolower(promoted->getSymbol()) == 'p' && promoted->getColor() == "black") counter++; 

    // TEST 7C: Promotion via Capture
    b1.clearBoard(); b1.placePiece(6, 3, new Pawn("white")); b1.placePiece(7, 4, new Knight("black")); b1.printBoard(); cout << endl; 
    runTest(b1, 6, 3, 7, 4, "Test 7C [Pawn Promotion via Capture]", MoveResult::SUCCESS, counter);

    promoted = b1.getPiece(7,4);
    if (promoted && tolower(promoted->getSymbol()) == 'q' && promoted->getColor() == "white") counter++; 

    b1.clearBoard(); b1.placePiece(1, 3, new Pawn("black")); b1.placePiece(0, 4, new Knight("white")); b1.printBoard(); cout << endl; 
    runTest(b1, 1, 3, 0, 4, "Test 7C [Pawn Promotion via Capture]", MoveResult::SUCCESS, counter);

    promoted = b1.getPiece(0,4);
    if (promoted && tolower(promoted->getSymbol()) == 'q' && promoted->getColor() == "black") counter++; 


    cout << "Promotion Pawn Tests: "<< counter / 2 << "/6" << endl; 
}

// 8) EN PASSANT
void enPassantTester(Board& b1){
    int counter = 0; 

    // TEST 8A: Valid En Passant
    b1.clearBoard(); b1.placePiece(4, 3, new Pawn("white")); b1.placePiece(6, 4, new Pawn("black")); b1.printBoard(); cout << endl; 
    b1.movePiece(6, 4, 4, 4); b1.printBoard(); 
    runTest(b1, 4, 3, 5, 4, "Test 8A [Valid En Passant]", MoveResult::SUCCESS, counter);

    // TEST 8B: Immediate En Passant Checker
    b1.clearBoard(); b1.placePiece(4, 3, new Pawn("white")); b1.placePiece(1, 0, new Pawn("white")); 
    b1.placePiece(6, 4, new Pawn("black")); b1.printBoard(); cout << endl; 
    b1.movePiece(6, 4, 4, 4); b1.printBoard(); 
    b1.movePiece(1, 0, 2, 0); b1.printBoard(); 
    runTest(b1, 4, 3, 5, 4, "Test 8B [Checks for Immediate En Passant]", MoveResult::INVALID_PAWN_CAPTURE, counter);

    // TEST 8C: No Double Step Checker
    b1.clearBoard(); b1.placePiece(5, 3, new Pawn("white")); b1.placePiece(6, 4, new Pawn("black")); b1.printBoard(); cout << endl; 
    b1.movePiece(6, 4, 5, 4); b1.printBoard(); 
    runTest(b1, 5, 3, 6, 4, "Test 8C [Double Step Required]", MoveResult::INVALID_PAWN_CAPTURE, counter);

    // TEST 8D: Adjacency Checker
    b1.clearBoard(); b1.placePiece(4, 3, new Pawn("white")); b1.placePiece(6, 5, new Pawn("black")); b1.printBoard(); cout << endl; 
    b1.movePiece(6, 5, 4, 5); b1.printBoard(); 
    runTest(b1, 4, 3, 5, 4, "Test 8D [Adjacency Required]", MoveResult::INVALID_PAWN_CAPTURE, counter);

    // TEST 8E: Color Validation
    b1.clearBoard(); b1.placePiece(3, 3, new Pawn("white")); b1.placePiece(1, 4, new Pawn("white")); b1.printBoard(); cout << endl; 
    b1.movePiece(1, 4, 3, 4); b1.printBoard(); 
    runTest(b1, 3, 3, 4, 4, "Test 8E [Color Validation]", MoveResult::INVALID_PAWN_CAPTURE, counter);

    cout << "En Passant Tests: "<< counter << "/5" << endl; 
}

// 9) CASTLING
void castleTester(Board& b1){
    int counter = 0; 

    // TEST 9A: Regular King-Side Castle
    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 7, new Rook("white")); 
    b1.printBoard(); cout << endl; 

    runTest(b1, 0, 4, 0, 6, "Test 9A [Regular King-Side Castling]", MoveResult::SUCCESS, counter);

    // TEST 9B: Regular Queen-Side Castle
    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 0, new Rook("white")); 
    b1.printBoard(); cout << endl; 

    runTest(b1, 0, 4, 0, 2, "Test 9B [Regular Queen-Side Castling]", MoveResult::SUCCESS, counter);

    // TEST 9C: Illegal Castling When In Check
    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 7, new Rook("white")); b1.placePiece(7, 4, new Queen("black"));
    b1.printBoard(); cout << endl; 

    runTest(b1, 0, 4, 0, 6, "Test 9C [Illegal Castling When In Check]", MoveResult::KING_IN_CHECK, counter);

    // TEST 9D: Illegal Castling Through Pieces
    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 7, new Rook("white")); b1.placePiece(0, 5, new Knight("white"));
    b1.printBoard(); cout << endl; 

    runTest(b1, 0, 4, 0, 6, "Test 9D [Illegal Castling Through Pieces]", MoveResult::PATH_NOT_CLEAR, counter);

    // TEST 9E: Illegal Castling Through Squares Under Attack
    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 7, new Rook("white")); b1.placePiece(6, 0, new Bishop("black"));
    b1.printBoard(); cout << endl; 

    runTest(b1, 0, 4, 0, 6, "Test 9E [Illegal Castling Through Squares Under Attack]", MoveResult::LEAVES_KING_IN_CHECK, counter);

    // TEST 9F: Illegal Castling After King AND/OR Rook Moving
    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 7, new Rook("white")); b1.printBoard(); cout << endl; 
    b1.movePiece(0, 4, 0, 3); b1.printBoard(); cout << endl; 

    runTest(b1, 0, 3, 0, 6, "Test 9F [Illegal Castling After King AND/OR Rook Moving]", MoveResult::ERR_CASTLING_KING_MOVED, counter);

    b1.clearBoard(); b1.placePiece(0, 4, new King("white")); b1.placePiece(0, 0, new Rook("white")); b1.printBoard(); cout << endl; 
    b1.movePiece(0, 0, 0, 1); b1.printBoard(); cout << endl;

    runTest(b1, 0, 4, 0, 2, "Test 9F [Illegal Castling After King AND/OR Rook Moving]", MoveResult::ERR_CASTLING_ROOK_MOVED, counter);

    cout << "Castling Test: "<< counter << "/7" << endl; 
}