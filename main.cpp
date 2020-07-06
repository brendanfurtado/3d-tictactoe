//
// Student Name: Brendan Furtado
// Student ID: 2607378671


#include <iostream>
#include <vector>

using namespace std;

char board[27] = {' ',' ',' ',' ',' ', ' ', ' ', ' ', ' ',
                  ' ',' ',' ',' ',' ', ' ', ' ', ' ', ' ',
                  ' ',' ',' ',' ',' ', ' ', ' ', ' ', ' '};


//Function declarations
void greetAndInstruct();
void displayBoard(char board[]);
bool checkIfLegal(int cellNbre, char board[]);
bool checkWinner(char board[]);


void computerMove(char board[]);
int minimax(char board[], int depth, bool maximizing); //Algorithm used to implement the computer ai
char checkPlayerWinner(char board[]); //Helper function for minimax algorithm , determines who which player (player or ai) wins


int main(){
    greetAndInstruct();
    string startInput;

    //Ask user if they want to play
    while(true) {
        cin >> startInput; //Ask if the user wants to play

        if (startInput.compare("n") == 0) {
            return 0;
        }
        else if (startInput.compare("y") != 0 && startInput.compare("n") != 0){
            cout << "Invalid input, type 'y' to play or 'n' to quit" << endl;
        }
            //User clicked y
        else break;
    }

    cout << "Initializing board..." << endl << endl;
    displayBoard(board);

//----------------------------------------------------------------------//
    //Main logic of game
    int cellsUsed = 0;
    //Player move first
    while (true) {

        //Check if there is a tie
        if(cellsUsed == 27){
            cout << "Tie!" << endl;
            break;
        }

        //Ask for player move
        cout << "Player's move: " << endl;
        int playerInput;
        cin >> playerInput;

        if (checkIfLegal(playerInput, board)) {
            int realIndex = playerInput - 1; //Recall things are indexed 0-26 which represents numbers 1-27
            board[realIndex] = 'x';
            cellsUsed += 1;
            displayBoard(board);

            //Check if player won
            if (checkWinner(board)) {
                cout << "Player has won!" << endl;
                break;
            }

        } else {    //Keep asking for input if the player inputs something wrong
            cout << "Invalid player move! Number is out of range or the space is occupied, try a different number"
                 << endl;
            cin.clear();
            cin.ignore(100, '\n');
            continue; //Continue to ask player for proper input
        }

        //Computer move
        cout << "Computer's move: " << endl;
        computerMove(board);
        cellsUsed += 1;
        displayBoard(board);

        //Check if Computer won
        if (checkWinner(board)) {
            cout << "Computer has won!" << endl;
            break;
        }
    }//End of while loop

}//End of main
