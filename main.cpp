#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cctype>
#include <string>
#include <sstream>

using namespace std;

void readDictionary(vector<string> &dictionary) {
    ifstream myFile;
    myFile.open("dictionary-3to5-letters.txt");

    assert(myFile.fail() == false);
    dictionary.clear();

    // Keep repeating while input from the file yields a word
    string newWord;                         // Store a single input word
    while (myFile >> newWord) {           // While there is another word to be read
        // Add this new word to the end of the vector
        dictionary.push_back(newWord);
    }

    // Close the dictionary file
    myFile.close();
}


class Board {
public:

    bool checkWinCondition() { // checks the win statement and decides the user won or not
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != completedBoard[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    string getRowString(int row) { // gets all of the characters from the selected row of the vector
        vector<char> cRow = board[row];
        return string(cRow.begin(), cRow.end());

    }

    string getColString(int col) { // gets all of the characters from the selected column of the vector
        string sCol;
        for (int i = 0; i < size; i++) {
            sCol += board[i][col];
        }
        return sCol;
    }

    void putRowString(int row, string sRow) { // puts all of the characters in a string in a particular row
        for (int i = 0; i < size && i < sRow.length(); i++) {
            board[row][i] = sRow[i];
        }
    }

    void putColString(int col, string sCol) {  // puts all of the characters in a string in a particular column
        for (int i = 0; i < size && i < sCol.length(); i++) {
            board[i][col] = sCol[i];
        }
    }

    void rotateRow(int row, int numRotate) { // extracts the string from a row, rotates the string, places the string in the row
        string sRow = getRowString(row);
        string afterRotateRow = rotate(numRotate, sRow);
        putRowString(row, afterRotateRow);
    }

    void rotateCol(int col, int numRotate) { // extracts the string from a column, rotates the string, places the string in the column
        string sCol = getColString(col);
        string afterRotateCol = rotate(numRotate, sCol);
        putColString(col, afterRotateCol);
    }

    void scrambleEverything(int numScrambles) { // uses rotateRow and rotateCol to rotate everything
        for (int i = 0; i < numScrambles; i++) {
            int randomIndex = (int) (rand() % size);
            rotateRow(randomIndex, 1);
            randomIndex = (int) (rand() % size);

            rotateCol(randomIndex, 1);

        }
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                initScrambledBoard[i][j] = board[i][j];
            }
        }
    }

    string rotate(int numRotates, string word) { //rotates the word, for eg: word=king and numRotates= 2
        string result;
        for (int i = 0; i < size; i++) {
            result += ' ';
            if (i >= word.length()) {
                word += ' ';
            }
        }
        //result= "    "
        for (int i = 0; i < size; i++) {
            int rotatedIndex = (i + numRotates + size) % size;// updated value for rotated index= 2 (remainder)
            result[rotatedIndex] = word[i];// result="ngki"
        }
        return result;
    }

    void draw();

    explicit Board(int boardSize) { // constructor that creates the of the given boardSize
        this->board.resize(boardSize, vector<char>(boardSize));
        this->completedBoard.resize(boardSize, vector<char>(boardSize));
        this->initScrambledBoard.resize(boardSize, vector<char>(boardSize));
        size = boardSize;
    }

    int *fromPosToRowCol(int pos) { // calculates the row and column position from a position if the board was a 1d vector
        int row = pos / size;
        int col = pos % size;
        return new int[2]{row, col};
    }

    void insertWordAt(int pos, string word) { // inserts word into the 2d vector by taking the row and column
        int currentPos = pos;
        for (int i = 0;
             i < word.length();
             i++, currentPos++) {
            int *rowCol = fromPosToRowCol(currentPos);
            int row = rowCol[0];
            int col = rowCol[1];
            board[row][col] = word[i];
            completedBoard[row][col] = word[i];
        }
    }

    string pickWord(vector<string> dictionary) { // picks random words from the dictionary
        int randomIndex = (int) (rand() % dictionary.size());
        string randWord = dictionary[randomIndex];
        return randWord;
    }

    void printSolutionWords() { // displays the words before they are scrambled for the user to see and refer to
        cout << "These are the words that you should try to spell out using the board, in order:" << endl;
        for (int i = 0; i < currentWords.size(); i++) {
            if (i != 0) {
                cout << ", ";
            }
            cout << currentWords[i];
        }
        cout << "." << endl;
    }

    void fitEverything(vector<string> dictionary) { // fits all the randomly picked words and see if the words can fit inside within the guidelines
        string randWord = pickWord(dictionary);

        int space = size * size;
        for (int i = 0; true;) {

            insertWordAt(i, randWord);

            i += (int) randWord.length();
            currentWords.push_back(randWord);
            if (i + 1 < space) {
                i++;
                insertWordAt(i, " ");

            }

            if (space - i <= 2) {
                break;
            }
            randWord = pickWord(dictionary);
            if (space - i <= 4 && randWord.length() > (space - i)) {
                while (randWord.length() > (space - i)) {
                    randWord = pickWord(dictionary);
                }
            }
        }
    }

    void displayScrambledWords() { // displays the words that are scrambled for the user to see and refer to so that they can solve it
        cout << "Current board words: " << endl;
        for (int i = 0; i < size * size; i++) {
            int *rowcol = fromPosToRowCol(i);
            char currentchar = board[rowcol[0]][rowcol[1]];
            if (currentchar != 0) {
                cout << currentchar;
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }

    void displayUnscrambledWords() { // assigns the characters to completedBoard and is used for the completed board
        cout << "Current board words: " << endl;
        for (int i = 0; i < size * size; i++) {
            int *rowcol = fromPosToRowCol(i);
            char currentchar = completedBoard[rowcol[0]][rowcol[1]];
            if (currentchar != 0) {
                cout << currentchar;
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }

    void drawCompletedBoard() { // draws completed board which is just like the draw function but uses completed board instead and uses completedBoard
        cout << "The completed board should look like: " << endl;
        int boardSize = (int) board.size();
        int dashes = boardSize * 4 - 1;
        cout << "    ";
        for (int a = 0; a < boardSize; a++) {
            cout << a << "   ";
        }
        cout << endl;
        for (int k = 0; k < boardSize; k++) {
            cout << "   ";
            for (int j = 0; j < dashes; j++) {
                cout << '-';
            }
            cout << endl;
            cout << k << " ";
            for (int i = 0; i < boardSize; i++) {
                char currentChar = completedBoard[k][i] == 0 ? ' ' : completedBoard[k][i];
                cout << '|' << ' ' << currentChar << ' ';
            }
            cout << '|';
            cout << endl;
        }
        cout << "   ";
        for (int j = 0; j < dashes; j++) {
            cout << "-";
        }
        cout << endl;
    }

    Board() = default;

    void resetBoard();

private:
    int size{};
    vector<string> currentWords; // stores the random words that were picked to store in the board
    vector<vector<char>> board; // stores the current state of the board
    vector<vector<char>> completedBoard; // stores the board containing the solution
    vector<vector<char>> initScrambledBoard; // stores the boards after scrambling it for the first time
};

void menuPartTwo() { // displays the second part of the menu
    cout << "Enter one of the following:" << endl;
    cout << "   R to rotate a row or column," << endl;
    cout << "   C to view what the completed board should look like," << endl;
    cout << "   B to reset the board back to the beginning," << endl;
    cout << "   G to generate a new board with new random words," << endl;
    cout << "   S to have the computer automatically solve a board that you enter (5 points extra credit),"
         << endl;
    cout << "   Q to quit." << endl;
}

void Board::draw() { // draws the normal board with the words put in
    if (!checkWinCondition()) {
        cout << "Current board:" << endl;
    }
    int boardSize = (int) board.size();
    int dashes = boardSize * 4 - 1;
    cout << "    ";
    for (int a = 0; a < boardSize; a++) {
        cout << a << "   ";
    }
    cout << endl;
    for (int k = 0; k < boardSize; k++) {
        cout << "   ";
        for (int j = 0; j < dashes; j++) {
            cout << '-';
        }
        cout << endl;
        cout << k << " ";
        for (int i = 0; i < boardSize; i++) {
            char currentChar = this->board[k][i] == 0 ? ' ' : this->board[k][i];
            cout << '|' << ' ' << currentChar << ' ';
        }
        cout << '|';
        cout << endl;
    }
    cout << "   ";
    for (int j = 0; j < dashes; j++) {
        cout << "-";
    }
    cout << endl;
}

void Board::resetBoard() { // it displays a new board with new words that are scrambled
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board.size(); j++) {
            board[i][j] = initScrambledBoard[i][j];
        }
    }
}

int main() {
    vector<string> dictionary;

    cout << "Program 5: Unscrambler" << endl; // first half of menu
    cout << "CS 141, Fall 2021, UIC" << endl;
    cout << endl;

    cout
            << "You will be presented with a board that is filled with scrambled words. Rotate the rows and columns to get the words displayed in the right order."
            << endl;
    cout << endl;

    readDictionary(dictionary);
    cout << dictionary.size() << " words have been read in from the dictionary." << endl;
    cout << endl;

    char quitBegin;
    cout << "Enter Q to quit or B to begin:" << endl;

    cin >> quitBegin;
    quitBegin = (char) tolower(quitBegin);

    if (quitBegin == 'q') {
        cout << "Thank you for playing!" << endl;
        cout << "Exiting program..." << endl;
    } else if (quitBegin == 'b') {
        int boardSize = 0;

        cout << "Choose your board size (must be a number greater than or equal to 4): " << endl;
        cin >> boardSize;


        while (boardSize < 4) {
            cout << "The board size must be at least 4. Retry." << endl;

            cout << "Choose your board size (must be a number greater than or equal to 4): " << endl;
            cin >> boardSize;
        }
        Board board(boardSize);
        int numScrambles = 0;
        cout << "Choose the number of times you would like the board to be scrambled (must be a single number >= 1): "
             << endl;
        cin >> numScrambles;
        while (numScrambles <= 0) {
            cout << "The number of times you select must be at least 1. Retry." << endl;


            cout
                    << "Choose the number of times you would like the board to be scrambled (must be a single number >= 1): "
                    << endl;
            cin >> numScrambles;
        }

        board.fitEverything(dictionary);
        board.scrambleEverything(numScrambles);


        char userInp;
        while (true) {
            board.printSolutionWords();
            board.draw();
            board.displayScrambledWords();
            menuPartTwo();
            cout << "Your choice:";
            cin >> userInp;
            userInp = (char) tolower(userInp);
            if (userInp == 'q') {
                cout << "Thank you for playing!" << endl;
                cout << "Exiting program..." << endl;
                break;
            }

            if (userInp == 'r') {
                while (true) {
                    cout << "Enter the row/column you would like to rotate, and the number of positions to rotate by."
                         << endl;
                    cout
                            << "This should be in the format of <R or C> <row/column number> <number of positions to rotate>,"
                            << endl;
                    cout << "where valid row and column numbers are between 0 and" << boardSize - 1 << "," << endl;

                    cout << "   E.g. R 0 1 would rotate the top row (row 0) of the board to the right once," << endl;
                    cout << "        c 1 -2 would rotate the second column (col 1) of the board upwards twice. "
                         << endl;
                    cout << "Your choice:";
                    string rowOrColStr, rowOrColNumStr, numRotateStr;
                    cin >> rowOrColStr >> rowOrColNumStr >> numRotateStr;


                    char rowOrCol = (char) tolower(rowOrColStr[0]);
                    int rowOrColNum = stoi(rowOrColNumStr);
                    int numRotate = stoi(numRotateStr);

                    if (rowOrCol != 'r' && rowOrCol != 'c') {
                        cout << "First input must be 'R' or 'C'. Try again." << endl;
                        continue;
                    }

                    if (rowOrColNum >= boardSize || rowOrColNum < 0) {
                        cout << "Number must be between 0 and " << boardSize - 1 << ". Try again." << endl;
                        continue;
                    }

                    if (rowOrCol == 'r') {
                        board.rotateRow(rowOrColNum, numRotate);
                    }
                    if (rowOrCol == 'c') {
                        board.rotateCol(rowOrColNum, numRotate);
                    }

                    break;
                }
            }
            if (userInp == 'c') {
                board.drawCompletedBoard();
                board.displayUnscrambledWords();

            }
            if (userInp == 'b') {
                board.resetBoard();
            }
            if (userInp == 'g') {
                Board boardNew(boardSize);
                boardNew.fitEverything(dictionary);
                boardNew.scrambleEverything(numScrambles);
                board = boardNew;
            }
            if (userInp == 's') {

            }
            if (board.checkWinCondition()) {
                board.draw();
                board.displayScrambledWords();
                cout << "Congratulations, you won! Thank you for playing!" << endl;
                cout << "Exiting program..." << endl;
                return 0;
            }
        }
    }
}