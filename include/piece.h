#ifndef PIECE_H
#define PIECE_H
 
#include <string>
#include <iostream>
using namespace std; 

class Piece {
    public: 
        Piece(const string& color, char symbol);
        virtual ~Piece() = default; 

        virtual bool isMoveShapeValid(int fromRow, int fromCol, int toRow, int toCol) = 0; 

        const string& getColor() const;
        char getSymbol() const;
        string getUnicodeSymbol() const; 

        virtual void notifyMoved();
        virtual bool getHasMoved(); 
        virtual void setHasMoved(bool); 

    private: 
        string color; 
        char symbol; 
}; 

#endif // PIECE_H