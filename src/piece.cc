#include "piece.h"



Piece::Piece(const string& color, char symbol): color(color){
    if (color == "white"){
        this->symbol = symbol; 
    } else {
        this->symbol = tolower(symbol); 
    }
}

const string& Piece::getColor() const{ 
    return color; 
} 

char Piece::getSymbol() const{ 
    return symbol; 
}

void Piece::notifyMoved() {
    // nothing to do
}

bool Piece::getHasMoved(){
    return true; 
}

void Piece::setHasMoved(bool flag){
    // cout << "Piece::setHasMoved()" << endl;
} 

string Piece::getUnicodeSymbol() const {
    if (color == "white") {
        if (tolower(symbol) == 'k')  return "♔";
        if (tolower(symbol) == 'q')  return "♕";
        if (tolower(symbol) == 'r')  return "♖";
        if (tolower(symbol) == 'b')  return "♗";
        if (tolower(symbol) == 'n')  return "♘";
        if (tolower(symbol) == 'p')  return "♙";
    } else {
        if (tolower(symbol) == 'k')  return "♚";
        if (tolower(symbol) == 'q')  return "♛";
        if (tolower(symbol) == 'r')  return "♜";
        if (tolower(symbol) == 'b')  return "♝";
        if (tolower(symbol) == 'n')  return "♞";
        if (tolower(symbol) == 'p')  return "♟";
    }
    
    return " "; // empty square
}

