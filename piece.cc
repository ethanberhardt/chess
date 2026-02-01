#include "piece.h"
#include <cctype>


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
