#include "board.h"
#include <iostream>
#include <string>

using namespace std; 

int main(){
    Board* game = new Board(); 
    game->boardInit(); 

    cout << "Initial Board: " << endl; 
    game->printBoard(); 

    string input = ""; 

    while (true){
        int a, b, c, d; 
        
        cout << "Please type your move [fromRow fromCol toRow toCol]: " << endl; 
        cin >> a >> b >> c >> d; 
        cout << "\n========================================================" << endl; 

        if (a == 0 && b == 0 && c == 0 && d == 0) break; 

        int result = game->movePiece(a, b, c, d); 

        if (result != 0) cout << "ERROR: " << result << endl; 
        game->printBoard(); cout << endl; 
    }

    delete game; 
    return 0; 
}