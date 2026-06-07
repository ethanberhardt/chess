#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "moveResult.h"
#include "moveRecord.h"
#include <string>
#include <iostream>
#include <stack>
#include <vector>

class Board {

    public:
        // Construction and Cleanup
        Board();
        ~Board();

        // Board Display and Initialization
        void printBoard();
        void boardInit();
        void clearBoard();

        // Testing Utilities
        void placePiece(int row, int col, Piece* p);
        Piece* getPiece(int row, int col);
        void setWhiteToMove(bool whiteToMove);

        // Primary Game Interface
        MoveResult movePiece(int fromRow, int fromCol, int toRow, int toCol);
        bool undoLastMove();

        // Game State Queries
        bool isCheckMate(const string& color);
        bool isStaleMate(const string& color);
        bool isWhiteToMove() const;
        bool isFiftyMoveDraw() const;
        bool isThreeFoldRepetition() const;

    private:
        // Core Board Representation
        Piece* board[8][8];
        static int sign(int x);

        // Game State
        Piece* lastDoubleStepPawn;
        int lastDoubleStepRow;
        int lastDoubleStepCol;

        bool whiteToMove;
        int halfMoveClock;

        stack<MoveRecord> moveHistory;
        vector<string> positionHistory;

        // Move Validation
        MoveResult validateYourPiece(Piece* currPiece);
        MoveResult validatePieceMove(Piece* currPiece, int fromRow, int fromCol, int toRow, int toCol);
        MoveResult validatePawnCapture(Piece* currPiece, Piece* destination, int fromRow, int fromCol, int toRow, int toCol);
        MoveResult isPathClear(int fromRow, int fromCol, int toRow, int toCol);
        MoveResult wouldLeaveKingInCheck(int fromRow, int fromCol, int toRow, int toCol, const string& color);

        // Castling
        bool isCastlingAttempt(Piece* piece, int fromRow, int fromCol, int toCol);
        MoveResult isCastlingLegal(Piece* king, int fromRow, int fromCol, int toCol, int& rookFromCol, int& rookToCol);
        MoveResult performCastling(Piece* currPiece, int fromRow, int fromCol, int toCol, MoveRecord& record);

        // En Passant
        bool isEnPassantMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toCol, Piece*& ePawn);

        // Promotion
        Piece* pawnPromotion(const string& color, int row, int col);

        // Captures
        Piece* captureHandler(Piece* currPiece, Piece* destination, int fromRow, int toCol);

        // Move Recording and Undo Support
        void buildMoveRecord(int fromRow, int fromCol, int toRow, int toCol, int previousLastDoubleStepRow, int previouslastDoubleStepCol, Piece* currPiece, Piece* captured, MoveRecord& record);

        // Board Mutation Helpers
        void updateBoard(int fromRow, int fromCol, int toRow, int toCol);
        void applyMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, bool isCastle, int rookFromCol, int rookToCol, Piece* rook);
        void undoMove(Piece* moving, Piece* destination, int fromRow, int fromCol, int toRow, int toCol, bool isEnPassant, Piece* epPawn, bool isCastle, int rookFromCol, int rookToCol, Piece* rook);

        // Check, Checkmate, and Stalemate Logic
        bool isKingInCheck(const string& color);
        bool hasAnyLegalMove(const string& color);
        bool isSquareAttacked(int row, int col, const string& color);

        // Position Tracking and Draw Detection
        string generateFingerPrint() const;
};



#endif // BOARD_H