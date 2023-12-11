#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> 

using namespace std;


class Player {
private:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol) : name(name), symbol(symbol) {}

    string getName() const {
        return name;
    }

    char getSymbol() const {
        return symbol;
    }
};

class Board {
private:
    static const int SIZE = 8;
    char board[SIZE][SIZE];

public:
    Board() {
        initializeBoard();
    }

    void initializeBoard() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                board[i][j] = '-';
            }
        }

        board[3][3] = 'O';
        board[3][4] = 'X';
        board[4][3] = 'X';
        board[4][4] = 'O';
    }

    char getCell(int row, int col) const {
        return board[row][col];
    }

    void setCell(int row, int col, char value) {
        board[row][col] = value;
    }

    void printBoard() const {
        cout << endl;
        cout << "   A  B  C  D  E  F  G  H" << endl;
        for (int i = 0; i < SIZE; ++i) {
            cout << i << "  ";
            for (int j = 0; j < SIZE; ++j) {
                cout << board[i][j] << "  ";
            }
            cout << endl;
        }
    }

    bool isValidIndex(int row, int col) const {
        return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
    }
};

class ReversiGame {
private:
    static string userInput;
    static const int SIZE = 8;

public:
    static void startGame(Board& board) {
        board.printBoard();
    }

    static bool isValidMove(const Board& board, const string& input, const Player& player) {
        string columns = "abcdefgh";
        string rows = "01234567";

        if (input == "start" || input == "exit") {
            userInput = input;
            return true;
        }

        if (input.length() != 2) {
            return false;
        }

        char columnSymbol = input[0];
        char rowSymbol = input[1];

        if (columns.find(columnSymbol) == string::npos || rows.find(rowSymbol) == string::npos) {
            return false;
        }

        int columnIndex = columns.find(columnSymbol);
        int rowIndex = rows.find(rowSymbol);

        if (!board.isValidIndex(rowIndex, columnIndex) || board.getCell(rowIndex, columnIndex) != '-') {
            return false;
        }

        vector<vector<int>> moves = getAvailableMoves(board, player);
        for (const auto& move : moves) {
            if (move[0] == columnIndex && move[1] == rowIndex) {
                return true;
            }
        }

        return false;
    }

    static vector<vector<int>> getAvailableMoves(const Board& board, const Player& player) {
        vector<vector<int>> moves;

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board.getCell(i, j) == '-') {
                    if (canFlip(board, i, j, player.getSymbol())) {
                        vector<int> move = { j, i };
                        moves.push_back(move);
                    }
                }
            }
        }
        return moves;
    }

    static void applyMove(Board& board, int rowIndex, int columnIndex, char playerSymbol) {
        board.setCell(rowIndex, columnIndex, playerSymbol);
        flipPieces(board, rowIndex, columnIndex, playerSymbol);
    }

    static void flipPieces(Board& board, int row, int col, char playerSymbol) {
        int surroundingOffsets[][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

        for (const auto& offset : surroundingOffsets) {
            int currentRow = row + offset[0];
            int currentCol = col + offset[1];

            while (board.isValidIndex(currentRow, currentCol) &&
                board.getCell(currentRow, currentCol) != playerSymbol &&
                board.getCell(currentRow, currentCol) != '-') {
                board.setCell(currentRow, currentCol, playerSymbol);
                currentRow += offset[0];
                currentCol += offset[1];
            }
        }
    }

    static bool canFlip(const Board& board, int row, int col, char playerSymbol) {
        char opponent = (playerSymbol == 'O') ? 'X' : 'O';

        int surroundingOffsets[][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

        for (const auto& offset : surroundingOffsets) {
            int currentRow = row + offset[0];
            int currentCol = col + offset[1];

            if (board.isValidIndex(currentRow, currentCol) && board.getCell(currentRow, currentCol) == opponent) {
                while (board.isValidIndex(currentRow, currentCol) &&
                    board.getCell(currentRow, currentCol) == opponent) {
                    currentRow += offset[0];
                    currentCol += offset[1];
                }

                if (board.isValidIndex(currentRow, currentCol) &&
                    board.getCell(currentRow, currentCol) == playerSymbol) {
                    return true;
                }
            }
        }

        return false;
    }

    static void printAvailableMoves(const Board& board, const Player& player) {
        int playerScore = countCurrentScore(board, player.getSymbol());
        int opponentScore = countCurrentScore(board, (player.getSymbol() == 'O') ? 'X' : 'O');
        cout << "                                    " << player.getName() << " scored: " << playerScore << " points" << endl;
        cout << "                                    Opponent scored: " << opponentScore << " points" << endl;
        cout << endl;

        string columns = "abcdefgh";
        cout << player.getName() << ", you can choose from the following moves:" << endl;

        vector<vector<int>> moves = getAvailableMoves(board, player);

        for (const auto& move : moves) {
            cout << "(" << columns[move[0]] << "," << move[1] << ")  ";
        }

        cout << endl << endl;
    }

    static int countCurrentScore(const Board& board, char player) {
        int total = 0;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board.getCell(i, j) == player) {
                    total += 1;
                }
            }
        }
        return total;
    }

    static void printWinner(const Board& board, const Player& player, const Player& opponent) {
        int playerScore = countCurrentScore(board, player.getSymbol());
        int opponentScore = countCurrentScore(board, opponent.getSymbol());
        if (playerScore == opponentScore) {
            cout << "It's a tie!" << endl;
        }
        else {
            cout << ((playerScore > opponentScore) ? player.getName() + " WINS!" :
                opponent.getName() + " WINS!") << endl;
        }
    }
};


string ReversiGame::userInput = "start";


int main() {
    Board board;
    Player player("Player", 'O');
    Player opponent("Opponent", 'X');

    ReversiGame::startGame(board);
    string userInput = "start";

    while (userInput != "exit") {
        vector<vector<int>> moves = ReversiGame::getAvailableMoves(board, player);
        ReversiGame::printAvailableMoves(board, player);

        cout << player.getName() << " chose: ";
        cin >> userInput;

        if (userInput == "exit") {
            break;
        }

        while (!ReversiGame::isValidMove(board, userInput, player)) {
            cout << "Invalid choice, try again: ";
            cin >> userInput;
        }

        int columnIndex = userInput[0] - 'a';
        int rowIndex = userInput[1] - '0';

        ReversiGame::applyMove(board, rowIndex, columnIndex, player.getSymbol());
        board.printBoard();

        vector<vector<int>> opponentMoves = ReversiGame::getAvailableMoves(board, opponent);
        ReversiGame::printAvailableMoves(board, opponent);

        try {
            int randomIndex = (opponentMoves.size() > 1) ? rand() % opponentMoves.size() : 0;
            cout << opponent.getName() << " chose: ("
                << static_cast<char>('A' + opponentMoves[randomIndex][0]) << ","
                << opponentMoves[randomIndex][1] << ")" << endl;
            Sleep(2000);
            system("cls");
            ReversiGame::applyMove(board, opponentMoves[randomIndex][1], opponentMoves[randomIndex][0], opponent.getSymbol());
            board.printBoard();
        }
        catch (...) {
            cout << "The " << opponent.getName() << " has no valid moves at the moment. It's your turn again."
                << endl;
            continue;
        }
    }

    ReversiGame::printWinner(board, player, opponent);
    cout << endl;

    return 0;
}

