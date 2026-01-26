#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <iostream>

class Board {
    public: 
        Board();
        ~Board(); 
        void printBoard(); 

        int movePiece(int fromRow, int fromCol, int toRow, int toCol); 
        bool isPathClear(int fromRow, int fromCol, int toRow, int toCol); 

    private: 
        Piece* board[8][8]; // 2D Array that keeps pointers to each piece
        static int sign(int x);
};



#endif // BOARD_H