#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
//i am writning this functions to creat a maze
int** createMaze(int rows, int cols) {
    int** maze = new int* [rows];
    for (int i = 0; i < rows; i++)
        maze[i] = new int[cols];
    return maze;
}
// this function is used to generate random 0s and 1s in the matrix
void generateMaze(int** maze, int rows, int cols) {
    srand(time(0));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            maze[i][j] = (rand() % 100 < 70) ? 1 : 0;
   // it confrims that starting and ending cell must be 1
    maze[0][0] = 1;
    maze[rows - 1][cols - 1] = 1;
}

// this function is used dislpay the maze again and again
void displayMaze(int** maze, int rows, int cols, int pr, int pc) // pr is for present row and pc is for present coloum
{
    cout << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == pr && j == pc)
                cout << "X ";
            else
                cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//this function is telling whether you can move to the next cell or not
bool canMove(int** maze, int rows, int cols, int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols)
        return false;
    if (maze[r][c] == 0)
        return false;
    return true;
}

// this function helps us to find the shortest path 
void findShortestPath(int** maze, int rows, int cols,int r, int c,int endR, int endC,int** visited,int* currentPath, int currentLen,int* bestPath, int& bestLen) 
{

    if (!canMove(maze, rows, cols, r, c) || visited[r][c])
        return;

    visited[r][c] = 1;
    currentPath[currentLen++] = r * cols + c;

    if (r == endR && c == endC) {
        if (currentLen < bestLen) {
            bestLen = currentLen;
            for (int i = 0; i < bestLen; i++)
                bestPath[i] = currentPath[i];
        }
    }
    else {
        findShortestPath(maze, rows, cols, r - 1, c, endR, endC, //it is used to track upward direction
            visited, currentPath, currentLen, bestPath, bestLen);
        findShortestPath(maze, rows, cols, r + 1, c, endR, endC, //it is used to track downward direction
            visited, currentPath, currentLen, bestPath, bestLen);
        findShortestPath(maze, rows, cols, r, c - 1, endR, endC,//it is used to track left direction
            visited, currentPath, currentLen, bestPath, bestLen);
        findShortestPath(maze, rows, cols, r, c + 1, endR, endC,//it is used to track right direction
            visited, currentPath, currentLen, bestPath, bestLen);
    }

    visited[r][c] = 0; // here we are backtracking it is for using the visisted cell for anyother path
}

// this function is used to display the path
void displayPath(int** maze, int rows, int cols,
    int sr, int sc,
    int er, int ec) {

    int** visited = createMaze(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            visited[i][j] = 0;

    int* currentPath = new int[rows * cols];
    int* bestPath = new int[rows * cols];

    int bestLen = rows * cols + 1;

    findShortestPath(maze, rows, cols,sr, sc,er, ec,visited, currentPath, 0, bestPath, bestLen);

    cout << "\n THE SHORTEST PATH IS \n";

    if (bestLen == rows * cols + 1) {
        cout << "PATH => NO PATH EXIST" << endl;
    }
    else {
        cout << "Path => ";
        for (int i = 1; i < bestLen; i++) {

            if (i > 1) cout << ", ";

            int prev = bestPath[i - 1];
            int curr = bestPath[i];

            int pr = prev / cols;
            int pc = prev % cols;
            int cr = curr / cols;
            int cc = curr % cols;

            if (cr < pr) cout << "up";
            else if (cr > pr) cout << "down";
            else if (cc < pc) cout << "left";
            else cout << "right";
        }
        cout << endl;
    }

    delete[] currentPath;
    delete[] bestPath;

 
    for (int i = 0; i < rows; i++)
        delete[] visited[i];
    delete[] visited;
}

// this is used to check whether next move is allowed or not
bool checkIfTrapped(int** maze, int rows, int cols, int r, int c) {

    int** visited = createMaze(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            visited[i][j] = 0;

    int* temp = new int[rows * cols];
    int* best = new int[rows * cols];
    int bestLen = rows * cols + 1;

    findShortestPath(maze, rows, cols,r, c,rows - 1, cols - 1,visited, temp, 0,best, bestLen);

    delete[] temp;
    delete[] best;
  
    for (int i = 0; i < rows; i++)
        delete[] visited[i];
    delete[] visited;

    return (bestLen == rows * cols + 1);
}

// game logic is defined here player movement is also controlled here 
void playGame(int** maze, int rows, int cols) {

    int cur_row = 0, cur_col = 0;
    int moves = 0;

    cout << "\n   MAZE GAME   \n";

    while (true) {

        displayMaze(maze, rows, cols, cur_row, cur_col);
        cout << "Position: (" << cur_row << ", "
            << cur_col << ") | Moves: "
            << moves << endl;

        if (cur_row == rows - 1 && cur_col == cols - 1) {
            cout << "\nYou won in " << moves << " moves!\n";
            displayPath(maze, rows, cols,
                0, 0,
                rows - 1, cols - 1);
            break;
        }

        if (checkIfTrapped(maze, rows, cols, cur_row, cur_col)) {
            cout << "\nPath => no path exists" << endl;
            break;
        }

        cout << "Move: ";
        char command;
        cin >> command;

        int newrow = cur_row;
        int newcol = cur_col;

        if      (command == 'w') newrow--;
        else if (command == 's') newrow++;
        else if (command == 'a') newcol--;
        else if (command == 'd') newcol++;
        else {
            cout << "Invalid command!" << endl;
            continue;
        }

        if (canMove(maze, rows, cols, newrow, newcol)) {
            cur_row = newrow;
            cur_col = newcol;
            moves++;
        }
        else {
            cout << "Cannot move there!" << endl;
        }
    }
}

int main() {

    int rows, cols;

    cout << "Enter maze rows: ";
    cin >> rows;

    cout << "Enter maze cols: ";
    cin >> cols;

    int** maze = createMaze(rows, cols);

    generateMaze(maze, rows, cols);

    cout << "\nGenerated Maze:" << endl;
    displayMaze(maze, rows, cols, 0, 0);

    playGame(maze, rows, cols);

    for (int i = 0; i < rows; i++)
        delete[] maze[i];
    delete[] maze;
}
