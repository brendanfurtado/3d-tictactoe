//
// Student Name: Brendan Furtado
// Student ID: 260737867

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

//Solutions to iterate over in checkWinner & checkPlayerWinner
int winning_moves[48][3] =
        {{1,2,3}, {4,5,6},{7,8,9},{1,4,7},{2,5,8},{3,6,9},{3,5,7},{1,5,9},
         {10,11,12},{13,14,15},{16,17,18},{10,13,16},{11,14,17},{12,15,18},{10,14,18},{12,14,16},
         {19,20,21},{22,23,24},{25,26,27},{19,22,25},{20,23,26},{21,24,27},{19,23,27},{21,23,25}, //This line and above are individual board wins
         {1,11,21},{4,14,24},{7,17,27},{1,13,25},{2,14,26},{3,15,27},{1,14,27},{7,14,21}, {3,11,19}, {6,14,22}, {9,17,25}, {7,13,19}, {8,14,20}, {9,15,21}, {9,14,19}, //This line represents wins across the 3 boards
         {1,10,19},{2,11,20},{3,12,21},{4,13,22},{5,14,23},{6,15,24},{7,16,25},{8,17,26},{9,18,27} //This line represents wins in same cell across the 3 boards
        };

int minimax(char board[], int depth, bool maximizing); //Algorithm used to implement the computer ai
char checkPlayerWinner(char board[]); //Helper function for minimax algorithm , determines who which player (player or ai) wins

//Introduction and rules of the game
void greetAndInstruct(){

    cout << "Hello and welcome to the Tic-Tac-Toe challenge: Player against Computer.\n"
            "The board is numbered from 1 to 27 as per the following:\n" << endl;

    cout << " 1 | 2 | 3 " << "\t\t" << " 10 | 11 | 12 " << "\t\t" << " 19 | 20 | 21 " << endl;
    cout << "-----------" << "\t\t" << " ------------" << "\t\t" << " ------------" << endl;
    cout << " 4 | 5 | 6 " << "\t\t" << " 13 | 14 | 15 " << "\t\t" << " 22 | 23 | 24 " << endl;
    cout << "-----------" << "\t\t" << " ------------" << "\t\t" << " ------------" << endl;
    cout << " 7 | 8 | 9 " << "\t\t" << " 16 | 17 | 18 " << "\t\t" << " 25 | 26 | 27 " << endl;

    cout << "\nPlayer starts first. Simply input the number of the cell you want to occupy. Player’s move\n"
            "is marked with X. Computer’s move is marked with O\n"
            "Start? (y/n):\n" << endl;
}

//Display board function
void displayBoard(char board[]){

    //Array that will be used to populate the cell numbers, and any board moves done by the computer or player
    string array[27] = {"1","2","3","4","5","6","7","8","9",
                        "10","11","12","13","14","15","16","17",
                        "18","19","20","21","22","23","24","25","26","27"};

    //Populate the string array with changed elements in board
    for(int i = 0; i < 27; i++) {

        //If an 'x' or an 'o' is encountered, convert and add it to the array at the current index
        if (board[i] == 'x' || board[i] == 'o') {
            char boardCharacter = board[i];
            std::stringstream ss;
            std::string strChar;
            ss << boardCharacter;
            ss >> strChar;

            //Makes spacing look nice when a char replaces a double digit
            if(i >= 9) {
                array[i] = strChar + " ";
            }
            else{
                array[i] = strChar;
            }
        }
        //Skip the index replacement and keep traversing the array
    }

    //Print to standard out the board with array's current values
    cout << " " << array[0] << " |" << " " << array[1] << " |" << " " << array[2]  << "\t\t" << " " << array[9] << " |" << " " << array[10] << " |" << " " << array[11] <<
         "\t\t" << " " << array[18] << " |" << " " << array[19] << " |" << " " << array[20] << endl;
    cout << "-----------" << "\t\t" << " ------------" << "\t\t" << " ------------" << endl;
    cout << " " << array[3] << " |" << " " << array[4] << " |" << " " << array[5]  << "\t\t" << " " << array[12] << " |" << " " << array[13] << " |" << " " << array[14] <<
         "\t\t" << " " << array[21] << " |" << " " << array[22] << " |" << " " << array[23] << endl;
    cout << "-----------" << "\t\t" << " ------------" << "\t\t" << " ------------" << endl;
    cout << " " << array[6] << " |" << " " << array[7] << " |" << " " << array[8]  << "\t\t" << " " << array[15] << " |" << " " << array[16] << " |" << " " << array[17] <<
         "\t\t" << " " << array[24] << " |" << " " << array[25] << " |" << " " << array[26] << endl;

    cout << endl;


}//End of displayBoard()

//Checks if the input from player is valid
bool checkIfLegal(int cellNbre, char board[]){

    int realIndex = cellNbre - 1;
    //Check if the move collides with a marker ('x' or 'o')
    if(board[realIndex] == 'x' || board[realIndex] == 'o'){
        return false;
    }

    //See if a number is in bounds
    return !(cellNbre <= 0 || cellNbre > 27);

}

//Function that traverses the winning conditions array above and see's if a player has won on the board
bool checkWinner(char board[]){

    for(int i = 0; i < 48; i++){

        if((board[winning_moves[i][0] - 1] == 'x' && board[winning_moves[i][1] - 1] == 'x' && board[winning_moves[i][2] - 1] == 'x') ||
           (board[winning_moves[i][0] - 1] == 'o' && board[winning_moves[i][1] - 1] == 'o' && board[winning_moves[i][2] - 1] == 'o')){

            return true;
        }

        //Else Keep iterating over the solutions
    }

    return false;
}


//Calls the minimax algorithm, this function determines the best score out of all remaining spots on the board
//The best score index will be chosen, and it's move is based on that.
void computerMove(char board[]){

    int bestScore = -100000;
    int move;

    //Iterate through all empty spots on the board
    for(int i = 0; i < 27; i++){

        //If there is an empty spot, see what the outcome of choosing it will be like
        if(board[i] == ' '){
            board[i] = 'o';
            int score = minimax(board, 0, false); //The maximizing boolean indicates if it choses board[i] = 'o', which moves can the player take to minimize score
            board[i] = ' '; //Replace board with empty spot once done with recursion call

            //Determine which spot gives the best score/potential outcome
            if(score > bestScore){
                bestScore = score;
                move = i;
            }
        }
    }
    //Once done, the best score will be taken
    board[move] = 'o';
}

//Implemented the minimax algorithm for the AI component
//The AI takes all available spots on the board and essentially choses the one that gives them the highest value. The highest value being advantagous (neutral = 0, or a win)
//Negative scores give an indication that the AI may lose, so it is able to block a single cell win. If there are multiple cells in which a player may win in the next turn,
// the AI chooses to go for the best winning possibility and doesn't try to block the player.
//Takes into account a couple moves in advanced with depth set to 2
int minimax(char board[], int depth, bool maximizing) {

    //Exit conditions, exit on a win, loss, tie or don't go beyond a certain depth
    char result = checkPlayerWinner(board);
    if (result == 'o') {
        return 10; //Computer wins returns positive integer
    }
    if (result == 'x') {
        return -10; //Computer loses returns positive integer
    }

    if(result == 't'){
        return 0; //Computer is indifferent
    }

    //This determines how deep the AI will "think" on a given move
    if(depth == 2){
        return 0;
    }

    //Clause that figures out how to maximize computer's score
    if (maximizing) {
        int bestScore = -100000;
        for (int i = 0; i < 27; i++) {
            if (board[i] == ' ') {
                board[i] = 'o';

                //Recursive call to "look ahead" of the original branching
                int score = minimax(board, depth + 1, false);
                board[i] = ' '; //Replace board with empty spot once done with recursion call
                bestScore = max(score, bestScore); //Update best score on a max call
            }
        }
        return bestScore;
    }
    //Clause that figures out how to minimize player's score given a move that the computer chose
    else{
        int bestScore = 100000;
        for (int j = 0; j < 27; j++) {
            if (board[j] == ' ') {
                board[j] = 'x';

                //Recursive call to "look ahead" of the original branching
                int score = minimax(board, depth + 1, true);
                board[j] = ' '; //Replace board with empty spot once done with recursion call
                bestScore = min(score, bestScore); //Update player score that minimizes it
            }
        }
        return bestScore;
    }
}

//Helper function, a function like checkWinner but instead returns the winning character of the player
char checkPlayerWinner(char board[]) {

    for (int i = 0; i < 48; i++) {

        //Player wins case
        if ((board[winning_moves[i][0] - 1] == 'x' && board[winning_moves[i][1] - 1] == 'x' &&
             board[winning_moves[i][2] - 1] == 'x')) {
            return 'x';

            //Computer wins case
        } else if ((board[winning_moves[i][0] - 1] == 'o' && board[winning_moves[i][1] - 1] == 'o' &&
                    board[winning_moves[i][2] - 1] == 'o')) {
            return 'o';

            //Tie case
        } else {
            int openCells = 0;
            //Check for tie
            for (int j = 0; j < 27; j++) {
                if (board[j] == ' ') {
                    openCells++;
                }
            }

            if (openCells == 0) {
                return 't';
            }
        }

        //Else Keep iterating over the solutions
    }

    return ' ';
}