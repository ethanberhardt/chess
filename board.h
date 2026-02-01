#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <iostream>

class Board {
    public: 
        Board();
        ~Board(); 

        void printBoard(); 
        void boardInit();

        void clearBoard(); // for testing purposes
        void placePiece(int row, int col, Piece* p); // for testing purposes
        Piece* getPiece(int row, int col); // for testing purposes

        bool isCheckMate(const string& color);
        bool isStaleMate(const string& color);

        int movePiece(int fromRow, int fromCol, int toRow, int toCol); 
    
    private: 
        Piece* board[8][8]; // 2D Array that keeps pointers to each piece
        Piece* lastDoubleStepPawn;  
        static int sign(int x);

        // Helper Functions
        bool validatePieceMove(Piece* currPiece, int fromRow, int fromCol, int toRow, int toCol);
        bool validatePawnCapture(Piece* currPiece, Piece* destination, int fromRow, int fromCol, int toRow, int toCol);
        bool isSquareAttacked(int row, int col, const string& color); 
        bool isPathClear(int fromRow, int fromCol, int toRow, int toCol);

        bool isCastlingAttempt(Piece* piece, int fromCol, int toCol);
        bool isCastlingLegal(Piece* king, int fromRow, int fromCol, int toCol, int& rookFromCol, int& rookToCol);
        int performCastling(Piece* currPiece, int fromRow, int fromCol, int toCol);

        void captureHandler(Piece* currPiece, Piece* destination, int fromRow, int toCol);

        void updateBoard(int fromRow, int fromCol, int toRow, int toCol); 

        bool isKingInCheck(const string& color);
        bool isEnPassantMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toCol, Piece*& ePawn);
        void applyMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, 
                       bool isCastle, int rookFromCol, int rookToCol, Piece* rook);
        void undoMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, 
                      bool isCastle, int rookFromCol, int rookToCol, Piece* rook);
        bool wouldLeaveKingInCheck(int fromRow, int fromCol, int toRow, int toCol, const string& color);

        bool hasAnyLegalMove(const string& color);

        void pawnPromotion(const string& color, int row, int col); 
};



#endif // BOARD_H