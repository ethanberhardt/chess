#include "board.h"

using namespace std; 

int main(){
    Board* game = new Board(); 

    cout << "Initial Board: " << endl; 
    game->printBoard(); 


    // TESTING ROOK [ONLY] MOVEMENTS
    // cout << "\nMove Rook from [0,0] to [7,0]: result = " << game->movePiece(0,0,7,0) << endl; 
    // game->printBoard(); 

    // cout << "\nMove Rook from [0,7] to [0,0]: result = " << game->movePiece(0,7,0,0) << endl; 
    // game->printBoard(); 

    // cout << "\nMove Rook from [7,7] to [7,0]: result = " << game->movePiece(7,7,7,0) << endl; 
    // game->printBoard(); 

    // // Moving Rook Incorrectly
    // cout << "\nMove Rook from [7,5] to [4,6]: result = " << game->movePiece(7,5,4,6) << endl; 
    // game->printBoard(); 

    // cout << "\nMove Rook from [0,0] to [7,0]: result = " << game->movePiece(0,0,7,0) << endl; 
    // game->printBoard();  

    // // TESTING BISHOP [& ROOK ONLY] MOVEMENTS
    // cout << "\nMove Bishop from [7,2] to [6,1]: result = " << game->movePiece(7,2,6,1) << endl; 
    // game->printBoard();

    // cout << "\nMove Bishop from [0,5] to [1,6]: result = " << game->movePiece(0,5,1,6) << endl; 
    // game->printBoard();

    // cout << "\nMove Bishop from [6,1] to [1,6]: result = " << game->movePiece(6,1,1,6) << endl; 
    // game->printBoard();

    // cout << "\nMove Rook from [0,7] to [0,6]: result = " << game->movePiece(0,7,0,6) << endl; 
    // game->printBoard();

    // cout << "\nMove Rook from [0,6] to [1,6]: result = " << game->movePiece(0,6,1,6) << endl; 
    // game->printBoard();

    // //TESTING KNIGHT ONLY MOVEMENTS
    // cout << "\nMove Knight from [4,4] to [6,5]: result = " << game->movePiece(4,4,6,5) << endl; 
    // game->printBoard();
    // game->movePiece(6,5,4,4); 
    // cout << "" << endl; game->printBoard();

    // cout << "\nMove Knight from [4,4] to [6,3]: result = " << game->movePiece(4,4,6,3) << endl; 
    // game->printBoard();
    // game->movePiece(6,3,4,4);
    // cout << "" << endl; game->printBoard(); 

    // cout << "\nMove Knight from [4,4] to [2,5]: result = " << game->movePiece(4,4,2,5) << endl; 
    // game->printBoard();
    // game->movePiece(2,5,4,4); 
    // cout << "" << endl; game->printBoard();

    // cout << "\nMove Knight from [4,4] to [2,3]: result = " << game->movePiece(4,4,2,3) << endl; 
    // game->printBoard();
    // game->movePiece(2,3,4,4);  
    // cout << "" << endl; game->printBoard();

    // cout << "\nMove Knight from [4,4] to [5,6]: result = " << game->movePiece(4,4,5,6) << endl; 
    // game->printBoard();
    // game->movePiece(5,6,4,4); 
    // cout << "" << endl; game->printBoard();

    // cout << "\nMove Knight from [4,4] to [3,6]: result = " << game->movePiece(4,4,3,6) << endl; 
    // game->printBoard();
    // game->movePiece(3,6,4,4); 
    // cout << "" << endl; game->printBoard();

    // cout << "\nMove Knight from [4,4] to [5,2]: result = " << game->movePiece(4,4,5,2) << endl; 
    // game->printBoard();
    // game->movePiece(5,2,4,4); 
    // cout << "" << endl; game->printBoard();

    // cout << "\nMove Knight from [4,4] to [3,2]: result = " << game->movePiece(4,4,3,2) << endl; 
    // game->printBoard();
    // game->movePiece(3,2,4,4);
    // cout << "" << endl; game->printBoard();

    delete game; 
    return 0; 
}