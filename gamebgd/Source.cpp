#include <iostream>
#include <chrono>
#include <Windows.h>
using namespace std;
struct board
{
    bool square, circle, empty;
    int pos;
    char data;
    board* next;
};

void drawBoard(board* fullBoard) // display the board in it's current state.
{
    board* current = fullBoard;
    while (current != NULL)
    {
        if (current->empty == true)
        {
            cout << "   " << current->pos;
        }
        else if (current->circle == true)
        {
            cout << "   " << 'O';
        }
        else
        {
            cout << "   " << 'X';
        }
        if (current->pos == 4 || current->pos == 7)
        {
            cout << "\n";
            cout << "-----------------------\n";
        }
        else if (current->pos == 1)
            cout << "\n";
        else
            cout << "   |";
        current = current->next;
    }

}

bool equal3(char x, char y, char z) // determine if 3 characters are equal whether it's X or O.
{
    if (x == y && x == z && x != ' ')
    {
        return true;
    }
    return false;
}

board* findNode(board* fullBoard, int new_data) // search a specific node in the linked list.
{
    board* temp = fullBoard;
    while (temp->pos != new_data)
    {
        temp = temp->next;
    }
    return temp;
}



int checkWinner(board* fullBoard) // check if somebody won or if it's a tie.
{
    board* temp = fullBoard;
    // Checking the rows for X or O victory.
    for (int i = 0; i <= 6; i = i + 3)
    {
        if (equal3(findNode(fullBoard, temp->pos - i)->data, findNode(fullBoard, temp->pos - i - 1)->data, findNode(fullBoard, temp->pos - i - 2)->data))
        {
            return findNode(fullBoard, temp->pos - i)->data == 'X' ? 2 : -2;
        }

    }
    // Checking the columns for X or O victory.
    for (int i = 0; i < 3; i++) {
        if (equal3(findNode(fullBoard, temp->pos - i)->data, findNode(fullBoard, temp->pos - i - 3)->data, findNode(fullBoard, temp->pos - i - 6)->data))
        {
            return findNode(fullBoard, temp->pos - i)->data == 'X' ? 2 : -2;

        }
    }
    // Checking the diagonals for X or O victory.
    if (equal3(findNode(fullBoard, temp->pos)->data, findNode(fullBoard, temp->pos - 4)->data, findNode(fullBoard, temp->pos - 8)->data))
    {
        return findNode(fullBoard, temp->pos)->data == 'X' ? 2 : -2;
    }
    if (equal3(findNode(fullBoard, temp->pos - 2)->data, findNode(fullBoard, temp->pos - 4)->data, findNode(fullBoard, temp->pos - 6)->data))
    {
        return findNode(fullBoard, temp->pos - 2)->data == 'X' ? 2 : -2;
    }
    // Checking for tie.
    bool tie = true;
    for (int i = 0; i < 9; i++)
    {
        if (temp->data == ' ')
        {
            tie = false;
        }
        temp = temp->next;
    }
    if (tie) return 0;
    return 1;
}
int minimax(board* fullBoard, int depth, bool isMaximizing, bool firstTime, int alpha, int beta) // algorithm for playing the best value for the AI.
{
    board* temp = fullBoard; // first check if someone has won.
    int result = checkWinner(fullBoard);
    if (result != 1)
    {
        return result;
    }
    if (isMaximizing) // if nobody won check if it's the maximizer's turn (X's turn)
    {
        int finalScore = -10;
        for (int i = 0; i < 9; i++)
        { // navigate through the board
            if (temp->data == ' ')
            {
                temp->data = 'X'; // play X in the specific location.
                int score = minimax(fullBoard, depth - 1, false, false, alpha, beta); // check recursively if this X play is the best play.
                temp->data = ' '; // undo the last play
                if (score > finalScore)
                {
                    finalScore = score; // store the value of the best play.
                    alpha = max(alpha, finalScore);
                    if (beta <= alpha) // if current branch is the best out of other branches (Performance Gain)
                    {
                        break; // ignore the other branches (Performance Gain)
                    }
                }
            }
            temp = temp->next;
        }
        return finalScore;
    }

    else // it's the minimizer's turn (O's turn)
    {
        int finalScore = 10;
        int finalPos;
        for (int i = 0;i < 9;i++)
        {
            if (temp->data == ' ')
            {
                temp->data = 'O'; // play O in the specific location.
                int score = minimax(fullBoard, depth - 1, true, false, alpha, beta); // check recursively if this O play is the best play.
                temp->data = ' '; // undo the last play
                if (score < finalScore)
                {
                    finalScore = score; // store the value of the best play.
                    finalPos = temp->pos; // store the best move
                    beta = min(beta, finalScore);
                    if (beta <= alpha) // if current branch is the best out of other branches. (Performance Gain)
                    {
                        break; // ignore the other branches
                    }
                }
                if (finalScore == -2)
                { //ignore other plays once there's a winning play. (Performance Gain)
                    break;
                }

            }

            temp = temp->next;
        }
        if (firstTime) // play the best move for O.
        {
            findNode(fullBoard, finalPos)->empty = false;
            findNode(fullBoard, finalPos)->circle = true;
        }
        return finalScore;
    }
}

int medium(board* fullBoard) // algorithm for playing random value.
{
    int finalvar;
    int var = rand() % 9 + 1;

    board* node = findNode(fullBoard, var);

    if (node->empty == true)
    {
        node->empty = false;
        node->circle = true;
        finalvar = var;
    }
    else
    {
        var = medium(fullBoard);
    }
    return finalvar;

}

int funt(board* fullBoard, int depth, bool isMaximizing, bool firstTime, int alpha, int beta)
{ // algorithm for playing the worst value for the AI.
    board* temp = fullBoard;
    int result = checkWinner(fullBoard);
    if (result != 1)
    {
        return result;
    }
    if (isMaximizing)
    {
        int finalScore = 10;
        for (int i = 0; i < 9; i++)
        {
            if (temp->data == ' ')
            {
                temp->data = 'X';
                int score = funt(fullBoard, depth - 1, false, false, alpha, beta);
                temp->data = ' ';
                if (score < finalScore)
                {
                    finalScore = score; // store the value of the worst play.
                    alpha = min(alpha, finalScore);
                    if (alpha <= beta) // if current branch is the best out of other branches (Performance Gain)
                    {
                        break; // ignore the other branches (Performance Gain)
                    }
                }
            }
            temp = temp->next;
        }
        return finalScore;
    }

    else
    {
        int finalPos;
        int finalScore = -10;
        for (int i = 0;i < 9;i++)
        {
            if (temp->data == ' ')
            {
                temp->data = 'O';
                int score = funt(fullBoard, depth - 1, true, false, alpha, beta);
                temp->data = ' ';
                if (score > finalScore)
                {
                    finalScore = score; // store the value of the worst play.
                    finalPos = temp->pos; // store the best move
                    beta = max(beta, finalScore);
                    if (alpha <= beta) // if current branch is the best out of other branches (Performance Gain)
                    {
                        break; // ignore the other branches (Performance Gain)
                    }
                }
                if (finalScore == 2) { //ignore other plays once there's the worst play (Performance Gain)
                    break;
                }
            }
            temp = temp->next;
        }
        if (firstTime) // play the worst move for O.
        {
            findNode(fullBoard, finalPos)->empty = false;
            findNode(fullBoard, finalPos)->circle = true;
        }
        return finalScore;
    }
}
void startGame(int first, board* fullBoard);
board* getBoard(board* fullBoard);

int main()
{
    int k = 1; while (k == 1) { //while loop for restarting the game.

        int first;
        board* fullBoard = NULL;
        cout << "Welcome to Tic-tac-toe DOS Game.\n\n";
        first = 1; // used to decide X is starting.
        cout << "X will start first!\n\n";
        system("pause");
        system("cls");
        clock_t start, end;
        start = clock();
        startGame(first, fullBoard);
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by program is : " << fixed
            << time_taken;
        cout << " sec " << endl;
        cout << "\nGame over! Press 1 to play again or press any key to exit.\n";
        cin >> k;
        system("cls");
    }
}

void startGame(int first, board* fullBoard)
{
    int diff;
    int choice;
    bool isPlaying;
    cout << "Please choose the difficulty!\n";
    cout << "1 for easy, 2 for medium and 3 for hard.\n\n";
    cin >> diff;
    while (diff < 1 || diff > 3)
    {
        cin.clear(); cin.ignore(100, '\n'); // if a letter entered it will be cleared and ignored so it will not cause an infinite loop.
        cout << "Please enter a correct number! \n";
        Sleep(1000);
        cin >> diff;
    }
    cout << "\nGoodluck!\n\n";
    Sleep(1000);
    system("cls");
    for (int i = 1; i <= 9; i++) {
        fullBoard = getBoard(fullBoard);
    }

    bool has_winner = false;
    while (!has_winner)
    {
        board* current = fullBoard;

        drawBoard(fullBoard);

        if (first == 1)
        {
            isPlaying = true;
            while (isPlaying == true)
            {
                cout << "Player 1, please put the number corresponding to the area you want to fill: ";
                cin >> choice;
                while (choice < 1 || choice > 9) {
                    cin.clear(); cin.ignore(100, '\n');
                    cout << "Please enter a correct number! \n";
                    Sleep(1000);
                    cout << "Please enter a number between 1 and 9: ";
                    cin >> choice;
                }
                current = fullBoard;
                while (current != NULL && current->pos != choice)
                    current = current->next;

                if (current->empty == true)
                {
                    current->empty = false;
                    current->square = true; //square is X.
                    isPlaying = false; //it will exit the loop.
                    first = 2; //now it will be the AI's turn

                }
                else
                    cout << "The field that you chose is already used...\n";
            }

        }
        else
        {
            if (diff == 3)
            {
                minimax(fullBoard, 100, false, true, -10, 10); //call the function that plays the best value for O.
                first = 1;
            }
            if (diff == 1)
            {
                funt(fullBoard, 100, false, true, 10, -10); //call the function that plays the worst value for O.
                first = 1;
            }
            if (diff == 2)
            { //call the function that plays a random value for O.
                medium(fullBoard);
                first = 1;
            }
        }


        current = fullBoard;  //change the data in the nodes from 1 to 9 according to the given plays.
        while (current != NULL)
        {
            if (current->empty == true)
            {
                current->data = ' ';
            }
            else if (current->circle == true)
            {
                current->data = 'O';
            }
            else
            {
                current->data = 'X';
            }
            current = current->next;
        }


        has_winner = checkWinner(fullBoard) != 1; //check if someone won.
        system("cls");

    }

    int result = checkWinner(fullBoard);
    if (result == 0) //if nobody won.
    {
        cout << "\nThe game ended in a tie!\n" << endl;
        drawBoard(fullBoard);
    } //display the board to see the end result.
    else
    {
        cout << ((result == 2) ? 'X' : 'O') << " is the winner!!\n\n" << endl;
        drawBoard(fullBoard);

    }
}

board* getBoard(board* fullBoard) //linked list function.
{
    board* newBoard = new board;
    newBoard->empty = true;
    newBoard->circle = false;
    newBoard->square = false;
    newBoard->next = fullBoard;
    if (newBoard->next != NULL)
        newBoard->pos = newBoard->next->pos + 1;
    else
        newBoard->pos = 1;
    return newBoard;
}

// for time execution changes depending on some parameters:

// 1. level of the of the game
// as it differs in execution time depending on the difficulty chosen by the user.
// for example:
// a) hard level's execution time is 2.87 seconds
// b) easy level's execution time is 2.7 seconds
// c) medium level's execution time is 2.48 seconds
// and the parameter that controls the execution time depending on the levels of the game in our code is "diff"

// 2. skill of the user
// if the user wins or loses quickly, the execution time will be much shorter than a user who ties with computer and takes much time between turns.
// for example:
// a) if a user plays on hard difficulty until it's a tie, the execution time will be 11.72 seconds.
// b) if a user plays on hard difficulty and loses as fast as possible, the execution time will be 2.87 seconds.
// and the parameter that controls the execution time in our code is "finalScore" in the minimax function that calculates the algorithm of the computer playing

