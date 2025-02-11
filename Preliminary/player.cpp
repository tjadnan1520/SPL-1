#include <iostream>
#include <graphics.h>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <cstring>
#define MAZE_ROWS 16
#define MAZE_COLS 21
#define CELL_SIZE 30

int mazeLayout[MAZE_ROWS][MAZE_COLS];
int goalFound = 0;
int pathTaken[MAZE_ROWS][MAZE_COLS];

using namespace std;
struct Point {
    int x, y;
    Point(int _x = -1, int _y = -1) : x(_x), y(_y) {}
};

// Add new global variables
struct PlayerStats {
    int steps;
    clock_t startTime;
    bool gameActive;
    int playerX, playerY;
    int playerColor;
    bool showSolution;
    int solutionPath[MAZE_ROWS][MAZE_COLS];
    bool timeExpired;
} player;

// Add new color definitions and constants
#define PLAYER_COLOR YELLOW
#define VISITED_PATH LIGHTMAGENTA
#define SUCCESS_COLOR LIGHTGREEN
#define FAILURE_COLOR LIGHTRED
#define SOLUTION_PATH_COLOR LIGHTCYAN
#define TIME_LIMIT 30
#define PATH_NOT_FOUND_COLOR RED

// Add direction arrays after color definitions
const int dx[] = {-1, 1, 0, 0};  // Up, Down, Left, Right
const int dy[] = {0, 0, -1, 1};

// Function prototypes
void generateMaze();
void saveMaze(const char *filename);
void loadMaze(const char *filename);
void renderMaze();
void customDelay(int milliseconds);
void drawPathOnMaze();
void playMazeGame();
void handlePlayerMovement();
void displayGameStats();
void showGameResult(bool success);
bool findSolutionPath(int startX, int startY);
void showSolutionPath();
void checkTimeLimit();

int main()
{
    int startingRow = 0;
    int startingCol = 0;

    char data[] = "C:\\MinGW\\lib\\libbgi.a";

    int graphicsDriver = DETECT, graphicsMode;
    initgraph(&graphicsDriver, &graphicsMode, data);

    generateMaze();
    cleardevice();
    renderMaze();

    outtextxy(10, 50, (char *)"Select an Option:");

   
    rectangle(10, 180, 275, 215);
    outtextxy(20, 190, (char *)"1. Save Maze");

    rectangle(10, 230, 275, 265);
    outtextxy(20, 240, (char *)"2. Load Maze");

    // Add new button for gameplay
    rectangle(10, 280, 275, 315);
    outtextxy(20, 290, (char*)"3. Play Maze Game");

    int x, y;

    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= 10 && x <= 275 && y >= 80 && y <= 115)
            {
                goalFound = 0;
                memset(pathTaken, 0, sizeof(pathTaken));
                cleardevice();
                renderMaze();
            

                if (goalFound)
                {
                    cout << "Path to reach the goal:" << endl;
                    for (int i = 0; i < MAZE_ROWS; i++)
                    {
                        for (int j = 0; j < MAZE_COLS; j++)
                        {
                            if (pathTaken[i][j] == 1)
                            {
                                cout << "-> (" << i << "," << j << ") ";
                            }
                        }
                    }
                }
                else
                {
                    cout << "Path to the goal not found!";
                }
            }
            else if (x >= 10 && x <= 275 && y >= 130 && y <= 165)
            {
                goalFound = 0;
                memset(pathTaken, 0, sizeof(pathTaken));
                cleardevice();
                renderMaze();
                

                if (goalFound)
                {
                    cout << "Path to reach the goal:" << endl;
                    for (int i = 0; i < MAZE_ROWS; i++)
                    {
                        for (int j = 0; j < MAZE_COLS; j++)
                        {
                            if (pathTaken[i][j] == 1)
                            {
                                cout << "-> (" << i << "," << j << ") ";
                            }
                        }
                    }
                }
                else
                {
                    cout << "Path to the goal not found!";
                }
            }
            else if (x >= 10 && x <= 275 && y >= 180 && y <= 215)
            {
                saveMaze("maze.txt");
            }
            else if (x >= 10 && x <= 275 && y >= 230 && y <= 265)
            {
                loadMaze("maze.txt");
                goalFound = 0;
                memset(pathTaken, 0, sizeof(pathTaken));
                cleardevice();
                renderMaze();
            }
            // Add new option for gameplay
            else if (x >= 10 && x <= 275 && y >= 280 && y <= 315) {
                playMazeGame();
            }
        }
    }

    getch();
    closegraph();

    return 0;
}

// Function to generate a random maze
void generateMaze()
{
    srand(time(0));

    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            mazeLayout[i][j] = (rand() % 4 == 0) ? 1 : 0;
        }
    }

    mazeLayout[0][0] = 0;                         
    mazeLayout[MAZE_ROWS - 1][MAZE_COLS - 1] = 2; 
}

void saveMaze(const char *filename)
{
    ofstream file(filename);
    if (!file)
    {
        cout << "Could not save the maze to the file." << endl;
        return;
    }

    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            file << mazeLayout[i][j] << " ";
        }
        file << endl;
    }

    file.close();
    cout << "Maze saved successfully to " << filename << endl;
}

void loadMaze(const char *filename)
{
    ifstream file(filename);
    if (!file)
    {
        cout << " Could not load the maze from the file." << endl;
        return;
    }

    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            file >> mazeLayout[i][j];
        }
    }

    file.close();
    cout << "Maze loaded successfully from " << filename << endl;
}

void renderMaze()
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            int xCoord = j * CELL_SIZE;
            int yCoord = i * CELL_SIZE;

            if (mazeLayout[i][j] == 1)
            {
                rectangle(xCoord, yCoord, xCoord + CELL_SIZE, yCoord + CELL_SIZE);
            }
            else if (mazeLayout[i][j] == 2)
            {
                circle(xCoord + CELL_SIZE / 2, yCoord + CELL_SIZE / 2, CELL_SIZE / 3);
            }
        }
    }
}

void customDelay(int milliseconds)
{
    int end_time = clock() + milliseconds;
    while (clock() < end_time)
    {
    }
}

void drawPathOnMaze()
{
    for (int i = 0; i < MAZE_ROWS; i++)
    {
        for (int j = 0; j < MAZE_COLS; j++)
        {
            if (pathTaken[i][j] == 1)
            {
                setfillstyle(SOLID_FILL, GREEN);
                bar(j * CELL_SIZE + CELL_SIZE / 6, i * CELL_SIZE + CELL_SIZE / 6,
                    (j + 1) * CELL_SIZE - CELL_SIZE / 6, (i + 1) * CELL_SIZE - CELL_SIZE / 6);
            }
        }
    }
    // Draw player's current position
    if(player.gameActive) {
        setfillstyle(SOLID_FILL, player.playerColor);
        bar(player.playerY * CELL_SIZE + 5, player.playerX * CELL_SIZE + 5,
            (player.playerY + 1) * CELL_SIZE - 5, (player.playerX + 1) * CELL_SIZE - 5);
    }
}

// Add new functions for gameplay
void playMazeGame() {
    cleardevice();
    renderMaze();
    
    // Initialize player
    player.steps = 0;
    player.startTime = clock();
    player.gameActive = true;
    player.playerX = 0;
    player.playerY = 0;
    player.playerColor = PLAYER_COLOR;
    player.showSolution = false;
    player.timeExpired = false;
    memset(player.solutionPath, 0, sizeof(player.solutionPath));
    
    // Find solution path first
    bool hasSolution = findSolutionPath(0, 0);
    if (!hasSolution) {
        showGameResult(false);
        return;
    }
    
    outtextxy(10, MAZE_ROWS * CELL_SIZE + 10, (char*)"Use Arrow Keys to Move, 'H' for Hint");
    
    while(player.gameActive) {
        handlePlayerMovement();
        displayGameStats();
        checkTimeLimit();
        
        if(player.showSolution) {
            showSolutionPath();
        }
        
        if(player.timeExpired) {
            showGameResult(false);
            break;
        }
        
        if(player.playerX == MAZE_ROWS-1 && player.playerY == MAZE_COLS-1) {
            showGameResult(true);
            break;
        }
        
        delay(50);
    }
}

// Improved Queue structure with better memory management and error checking
struct Queue {
    struct QPoint {
        int x, y;
        int parentX, parentY;  // Store parent coordinates directly
        QPoint(int _x = 0, int _y = 0, int px = -1, int py = -1) 
            : x(_x), y(_y), parentX(px), parentY(py) {}
    };
    
    QPoint* data;
    int capacity;
    int front, rear;
    
    Queue() {
        capacity = MAZE_ROWS * MAZE_COLS;
        data = new QPoint[capacity];
        front = rear = 0;
    }
    
    ~Queue() {
        delete[] data;
    }
    
    void push(int x, int y, int px = -1, int py = -1) {
        if(rear < capacity) {
            data[rear++] = QPoint(x, y, px, py);
        }
    }
    
    QPoint pop() {
        return front < rear ? data[front++] : QPoint(-1, -1);
    }
    
    bool empty() {
        return front == rear;
    }
};

// Improved findSolutionPath function with better path tracking
bool findSolutionPath(int startX, int startY) {
    Queue q;
    bool visited[MAZE_ROWS][MAZE_COLS] = {false};
    
    q.push(startX, startY);
    visited[startX][startY] = true;
    
    while(!q.empty()) {
        Queue::QPoint current = q.pop();
        int x = current.x, y = current.y;
        
        // Check for invalid coordinates
        if(x == -1 || y == -1) continue;
        
        // Found the goal
        if(mazeLayout[x][y] == 2) {
            // Reconstruct path using parent coordinates
            int cx = x, cy = y;
            while(cx != startX || cy != startY) {
                player.solutionPath[cx][cy] = 1;
                
                // Find the parent from visited positions
                bool found = false;
                for(int i = q.front - 1; i >= 0; i--) {
                    if(q.data[i].x == cx && q.data[i].y == cy) {
                        cx = q.data[i].parentX;
                        cy = q.data[i].parentY;
                        found = true;
                        break;
                    }
                }
                if(!found) break;
            }
            player.solutionPath[startX][startY] = 1;
            return true;
        }
        
        // Try all four directions in priority order (optimized path finding)
        const int priorityDx[] = {0, 1, 0, -1};  // Prioritize right and down
        const int priorityDy[] = {1, 0, -1, 0};
        
        for(int i = 0; i < 4; i++) {
            int newX = x + priorityDx[i];
            int newY = y + priorityDy[i];
            
            if(newX >= 0 && newX < MAZE_ROWS && newY >= 0 && newY < MAZE_COLS &&
               !visited[newX][newY] && mazeLayout[newX][newY] != 1) {
                q.push(newX, newY, x, y);
                visited[newX][newY] = true;
            }
        }
    }
    return false;
}

// Add function to show solution path
void showSolutionPath() {
    for(int i = 0; i < MAZE_ROWS; i++) {
        for(int j = 0; j < MAZE_COLS; j++) {
            if(player.solutionPath[i][j]) {
                setfillstyle(SOLID_FILL, SOLUTION_PATH_COLOR);
                bar(j * CELL_SIZE + CELL_SIZE/4, i * CELL_SIZE + CELL_SIZE/4,
                    (j + 1) * CELL_SIZE - CELL_SIZE/4, (i + 1) * CELL_SIZE - CELL_SIZE/4);
            }
        }
    }
}

// Add function to check time limit
void checkTimeLimit() {
    double elapsed = (double)(clock() - player.startTime) / CLOCKS_PER_SEC;
    if(elapsed >= TIME_LIMIT) {
        player.timeExpired = true;
        player.gameActive = false;
    }
}

// Modify handlePlayerMovement to include hint feature
void handlePlayerMovement() {
    if(kbhit()) {
        char key = getch();
        int newX = player.playerX;
        int newY = player.playerY;
        
        // Fix duplicate case labels by using ASCII values
        switch(key) {
            case 72: case 'w': case 'W': newX--; break; // Up
            case 80: case 's': case 'S': newX++; break; // Down
            case 75: case 'a': case 'A': newY--; break; // Left
            case 77: case 'd': case 'D': newY++; break; // Right
            case 'h':  
                player.showSolution = !player.showSolution; 
                break;
            case 27: // ESC
                player.gameActive = false; 
                return;
        }
        
        // Check if move is valid
        if(newX >= 0 && newX < MAZE_ROWS && newY >= 0 && newY < MAZE_COLS 
           && mazeLayout[newX][newY] != 1) {
            
            // Mark old position as visited
            setfillstyle(SOLID_FILL, VISITED_PATH);
            bar(player.playerY * CELL_SIZE + 5, player.playerX * CELL_SIZE + 5,
                (player.playerY + 1) * CELL_SIZE - 5, (player.playerX + 1) * CELL_SIZE - 5);
            
            player.playerX = newX;
            player.playerY = newY;
            player.steps++;
            
            // Draw new player position
            setfillstyle(SOLID_FILL, player.playerColor);
            bar(player.playerY * CELL_SIZE + 5, player.playerX * CELL_SIZE + 5,
                (player.playerY + 1) * CELL_SIZE - 5, (player.playerX + 1) * CELL_SIZE - 5);
            
            cleardevice();
            renderMaze();
            drawPathOnMaze();
        }
    }
}

void displayGameStats() {
    char buffer[100];
    double elapsed = (double)(clock() - player.startTime) / CLOCKS_PER_SEC;
    sprintf(buffer, "Steps: %d  Time: %.1f s", player.steps, elapsed);
    setcolor(WHITE);
    outtextxy(10, MAZE_ROWS * CELL_SIZE + 30, buffer);
}

void showGameResult(bool success) {
    cleardevice();
    setcolor(success ? SUCCESS_COLOR : FAILURE_COLOR);
    
    char buffer[100];
    double finalTime = (double)(clock() - player.startTime) / CLOCKS_PER_SEC;
    
    if(success) {
        sprintf(buffer, "Congratulations! Completed in %.1f seconds with %d steps", 
                finalTime, player.steps);
    } else if(player.timeExpired) {
        sprintf(buffer, "Time's up! Try again later. Steps taken: %d", 
                player.steps);
    } else {
        sprintf(buffer, "No valid path to destination! Try a different maze.");
    }
    
    outtextxy(50, getmaxy()/2, buffer);
    delay(2000);
}
