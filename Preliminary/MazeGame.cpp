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
#define TIME_LIMIT 30

using namespace std;

namespace SolverProgram
{
    int mazeLayout[MAZE_ROWS][MAZE_COLS];
    int pathTaken[MAZE_ROWS][MAZE_COLS];
    int goalFound = 0;

    struct Queue
    {
        struct Point
        {
            int x, y;
        } data[MAZE_ROWS * MAZE_COLS];
        int front = 0, rear = 0;

        void push(int x, int y) { data[rear++] = {x, y}; }
        Point pop() { return data[front++]; }
        bool empty() { return front == rear; }

    };

    struct Stack
    {
        struct Point
        {
            int x, y;
        } data[MAZE_ROWS * MAZE_COLS];
        int top = -1;

        void push(int x, int y) { data[++top] = {x, y}; }
        Point pop() { return data[top--]; }
        bool empty() { return top == -1; }
    };

    typedef struct
    {
        int x, y;
        int g, h, f;
    } Node;

    void customDelay(int milliseconds)
    {
        int end_time = clock() + milliseconds;
        while (clock() < end_time)
        {
        }
    }
    
    //inorder to create a random maze all the time
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
                    setfillstyle(SOLID_FILL, WHITE);
                    bar(xCoord, yCoord, xCoord + CELL_SIZE, yCoord + CELL_SIZE);

                }
                else if (mazeLayout[i][j] == 2)
                {
                    setfillstyle(SOLID_FILL, RED);
                    circle(xCoord + CELL_SIZE / 2, yCoord + CELL_SIZE / 2, CELL_SIZE / 3);
                    floodfill(xCoord + CELL_SIZE / 2, yCoord + CELL_SIZE / 2, WHITE);

                }
                else
                {
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(xCoord, yCoord, xCoord + CELL_SIZE, yCoord + CELL_SIZE);

                }
            }
        }
    }

    //for visualizing solution path 
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
    }

    //for saving the maze
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

    //for loading the maze
    void loadMaze(const char *filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Could not load the maze from the file." << endl;
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

    //for generating the maze
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

    //for solving the maze using BFS
    void bfsSolveMaze(int startX, int startY)
    {
        Queue q;
        bool visited[MAZE_ROWS][MAZE_COLS] = {false};
        q.push(startX, startY);
        visited[startX][startY] = true;

        while (!q.empty())
        {
            Queue::Point current = q.pop();
            int x = current.x, y = current.y;
            pathTaken[x][y] = 1;

            if (mazeLayout[x][y] == 2)
            {
                goalFound = 1;
                break;
            }

            int dx[] = {-1, 0, 1, 0};
            int dy[] = {0, 1, 0, -1};

            for (int i = 0; i < 4; i++)
            {
                int newX = x + dx[i];
                int newY = y + dy[i];
                if (newX >= 0 && newX < MAZE_ROWS && newY >= 0 && newY < MAZE_COLS &&
                    !visited[newX][newY] && mazeLayout[newX][newY] != 1)
                {
                    visited[newX][newY] = true;
                    q.push(newX, newY);
                }
            }

            customDelay(50);
            cleardevice();
            renderMaze();
            drawPathOnMaze();

        }
    }

    //for solving the maze using DFS
    void dfsExplorePaths(Stack &s)
    {
        while (!s.empty() && !goalFound)
        {
            Stack::Point current = s.pop();
            int x = current.x, y = current.y;

            if (x < 0 || y < 0 || x >= MAZE_ROWS || y >= MAZE_COLS || mazeLayout[x][y] == 1 || pathTaken[x][y] == 1)
            {
                continue;
            }

            pathTaken[x][y] = 1;
            if (mazeLayout[x][y] == 2)
            {
                goalFound = 1;
                return;
            }

            s.push(x + 1, y);
            s.push(x - 1, y);
            s.push(x, y + 1);
            s.push(x, y - 1);

            customDelay(100);
            cleardevice();
            renderMaze();
            drawPathOnMaze();

        }
    }

    //implementation of DFS
    void dfsSolveMaze(int startX, int startY)
    {
        Stack s;
        s.push(startX, startY);
        dfsExplorePaths(s);

    }

    int calculateManhattanDistance(int x1, int y1, int x2, int y2)
    {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    bool isValidCell(int x, int y)
    {
        return (x >= 0 && x < MAZE_ROWS && y >= 0 && y < MAZE_COLS);
    }

    bool aStar(int startX, int startY, int goalX, int goalY)
    {
        Node openList[MAZE_ROWS * MAZE_COLS];
        Node closedList[MAZE_ROWS * MAZE_COLS];
        int openCount = 0, closedCount = 0;

        Node start = {startX, startY, 0, calculateManhattanDistance(startX, startY, goalX, goalY), 0};
        start.f = start.g + start.h;
        openList[openCount++] = start;

        bool visited[MAZE_ROWS][MAZE_COLS] = {false};

        while (openCount > 0)
        {
            int currentIndex = 0;

            for (int i = 1; i < openCount; i++)
            {
                if (openList[i].f < openList[currentIndex].f)
                    currentIndex = i;
            }

            Node current = openList[currentIndex];
            openCount--;
            for (int i = currentIndex; i < openCount; i++)
                openList[i] = openList[i + 1];

            if (current.x == goalX && current.y == goalY)
            {
                pathTaken[current.x][current.y] = 1;
                return true;
            }

            visited[current.x][current.y] = true;
            pathTaken[current.x][current.y] = 1;

            int dx[] = {0, 1, 0, -1};
            int dy[] = {-1, 0, 1, 0};
            for (int i = 0; i < 4; i++)
            {
                int newX = current.x + dx[i];
                int newY = current.y + dy[i];

                if (isValidCell(newX, newY) && !visited[newX][newY] && mazeLayout[newX][newY] != 1)
                {
                    Node neighbor = {newX, newY, current.g + 1, calculateManhattanDistance(newX, newY, goalX, goalY), 0};
                    neighbor.f = neighbor.g + neighbor.h;

                    bool inOpenList = false;
                    for (int j = 0; j < openCount; j++)
                    {
                        if (openList[j].x == neighbor.x && openList[j].y == neighbor.y && openList[j].f <= neighbor.f)
                        {
                            inOpenList = true;
                            break;
                        }
                    }
                    if (!inOpenList)
                        openList[openCount++] = neighbor;
                }
            }
            customDelay(50);
            cleardevice();
            renderMaze();
            drawPathOnMaze();

        }
        return false;
    }
}

namespace GameProgram
{
    int mazeLayout[MAZE_ROWS][MAZE_COLS];
    int goalFound = 0;
    int pathTaken[MAZE_ROWS][MAZE_COLS];

    struct Point
    {
        int x, y;
        Point(int _x = -1, int _y = -1) : x(_x), y(_y) {}
    };

    struct PlayerStats
    {
        int steps;
        clock_t startTime;
        bool gameActive;
        int playerX, playerY;
        int playerColor;
        bool showSolution;
        int solutionPath[MAZE_ROWS][MAZE_COLS];
        bool timeExpired;

    } player;

#define PLAYER_COLOR YELLOW
#define VISITED_PATH LIGHTMAGENTA
#define SUCCESS_COLOR LIGHTGREEN
#define FAILURE_COLOR LIGHTRED
#define SOLUTION_PATH_COLOR LIGHTCYAN
#define PATH_NOT_FOUND_COLOR RED

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    void generateMaze();
    void saveMaze(const char *filename);
    void loadMaze(const char *filename);
    void renderMaze();
    void customDelay(int milliseconds);
    void drawPathOnMaze();
    bool findSolutionPath(int startX, int startY);
    void showSolutionPath();
    void handlePlayerMovement();
    void displayGameStats();
    void checkTimeLimit();
    void showGameResult(bool success);

    //for playing the maze game
    void playMazeGame()
    {
        cleardevice();
        renderMaze();
        player.steps = 0;
        player.startTime = clock();
        player.gameActive = true;
        player.playerX = 0;
        player.playerY = 0;
        player.playerColor = PLAYER_COLOR;
        player.showSolution = false;
        player.timeExpired = false;
        memset(player.solutionPath, 0, sizeof(player.solutionPath));

        bool hasSolution = findSolutionPath(0, 0);
        if (!hasSolution)
        {
            showGameResult(false);
            return;
        }

        outtextxy(10, MAZE_ROWS * CELL_SIZE + 10, (char *)"Use Arrow Keys to Move, 'H' for Hint");

        while (player.gameActive)
        {
            handlePlayerMovement();
            displayGameStats();
            checkTimeLimit();

            if (player.showSolution)
            {
                showSolutionPath();
            }

            if (player.timeExpired)
            {
                showGameResult(false);
                break;
            }

            if (player.playerX == MAZE_ROWS - 1 && player.playerY == MAZE_COLS - 1)
            {
                showGameResult(true);
                break;
            }

            delay(50);
        }
    }

    //for generating the maze
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
    //for saving the maze
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
    //for loading the maze
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

    //for rendering the maze
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

    //for custom delay
    void customDelay(int milliseconds)
    {
        int end_time = clock() + milliseconds;
        while (clock() < end_time)
        {
        }
    }

    //for drawing the path on the maze
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
        if (player.gameActive)
        {

            setfillstyle(SOLID_FILL, player.playerColor);
            bar(player.playerY * CELL_SIZE + 5, player.playerX * CELL_SIZE + 5,
                (player.playerY + 1) * CELL_SIZE - 5, (player.playerX + 1) * CELL_SIZE - 5);

        }
    }

    struct Queue
    {
        struct QPoint
        {
            int x, y;
            int parentX, parentY;
            QPoint(int _x = 0, int _y = 0, int px = -1, int py = -1)
                : x(_x), y(_y), parentX(px), parentY(py) {}
        };
        QPoint *data;
        int capacity;
        int front, rear;

        Queue()
        {
            capacity = MAZE_ROWS * MAZE_COLS;
            data = new QPoint[capacity];
            front = rear = 0;
        }

        ~Queue()
        {
            delete[] data;
        }

        void push(int x, int y, int px = -1, int py = -1)
        {
            if (rear < capacity)
            {
                data[rear++] = QPoint(x, y, px, py);
            }
        }

        QPoint pop()
        {
            return front < rear ? data[front++] : QPoint(-1, -1);
        }

        bool empty()
        {
            return front == rear;
        }
    };

    //for finding the solution path
    bool findSolutionPath(int startX, int startY)
    {
        Queue q;
        bool visited[MAZE_ROWS][MAZE_COLS] = {false};

        q.push(startX, startY);
        visited[startX][startY] = true;

        while (!q.empty())
        {
            Queue::QPoint current = q.pop();
            int x = current.x, y = current.y;

            if (x == -1 || y == -1)
                continue;

            if (mazeLayout[x][y] == 2)
            {
                int cx = x, cy = y;
                while (cx != startX || cy != startY)
                {
                    player.solutionPath[cx][cy] = 1;

                    bool found = false;
                    for (int i = q.front - 1; i >= 0; i--)
                    {
                        if (q.data[i].x == cx && q.data[i].y == cy)
                        {
                            cx = q.data[i].parentX;
                            cy = q.data[i].parentY;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        break;
                }

                player.solutionPath[startX][startY] = 1;
                return true;

            }

            const int priorityDx[] = {0, 1, 0, -1};
            const int priorityDy[] = {1, 0, -1, 0};

            for (int i = 0; i < 4; i++)
            {
                int newX = x + priorityDx[i];
                int newY = y + priorityDy[i];

                if (newX >= 0 && newX < MAZE_ROWS && newY >= 0 && newY < MAZE_COLS &&
                    !visited[newX][newY] && mazeLayout[newX][newY] != 1)
                {
                    q.push(newX, newY, x, y);
                    visited[newX][newY] = true;
                }
            }
        }
        return false;
    }
    //for showing the solution path
    void showSolutionPath()
    {
        for (int i = 0; i < MAZE_ROWS; i++)
        {
            for (int j = 0; j < MAZE_COLS; j++)
            {
                if (player.solutionPath[i][j])
                {

                    setfillstyle(SOLID_FILL, SOLUTION_PATH_COLOR);
                    bar(j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE + CELL_SIZE / 4,
                        (j + 1) * CELL_SIZE - CELL_SIZE / 4, (i + 1) * CELL_SIZE - CELL_SIZE / 4);

                }
            }
        }
    }

    //for checking the time limit
    void checkTimeLimit()
    {
        double elapsed = (double)(clock() - player.startTime) / CLOCKS_PER_SEC;
        if (elapsed >= TIME_LIMIT)
        {
            player.timeExpired = true;
            player.gameActive = false;
        }
    }

    //for handling the player movements
    void handlePlayerMovement()
    {
        if (kbhit())
        {
            char key = getch();
            int newX = player.playerX;
            int newY = player.playerY;

            switch (key)
            {
            case 72:
            case 'w':
            case 'W':
                newX--;
                break;
            case 80:
            case 's':
            case 'S':
                newX++;
                break;
            case 75:
            case 'a':
            case 'A':
                newY--;
                break;
            case 77:
            case 'd':
            case 'D':
                newY++;
                break;
            case 'h':
                player.showSolution = !player.showSolution;
                break;
            case 27:
                player.gameActive = false;
                return;
            }

            if (newX >= 0 && newX < MAZE_ROWS && newY >= 0 && newY < MAZE_COLS && mazeLayout[newX][newY] != 1)
            {

                setfillstyle(SOLID_FILL, VISITED_PATH);
                bar(player.playerY * CELL_SIZE + 5, player.playerX * CELL_SIZE + 5,
                    (player.playerY + 1) * CELL_SIZE - 5, (player.playerX + 1) * CELL_SIZE - 5);

                player.playerX = newX;
                player.playerY = newY;
                player.steps++;

                setfillstyle(SOLID_FILL, player.playerColor);
                bar(player.playerY * CELL_SIZE + 5, player.playerX * CELL_SIZE + 5,
                    (player.playerY + 1) * CELL_SIZE - 5, (player.playerX + 1) * CELL_SIZE - 5);

                cleardevice();

                renderMaze();
                drawPathOnMaze();

            }
        }
    }

    //for displaying the game stats
    void displayGameStats()
    {   

        char buffer[100];
        double elapsed = (double)(clock() - player.startTime) / CLOCKS_PER_SEC;
        sprintf(buffer, "Steps: %d  Time: %.1f s", player.steps, elapsed);
        setcolor(WHITE);
        outtextxy(10, MAZE_ROWS * CELL_SIZE + 30, buffer);

    }

    //for showing the game result
    void showGameResult(bool success)
    {
        cleardevice();

        setcolor(success ? SUCCESS_COLOR : FAILURE_COLOR);

        char buffer[100];
        double finalTime = (double)(clock() - player.startTime) / CLOCKS_PER_SEC;

        if (success)
        {
            sprintf(buffer, "Congratulations! Completed in %.1f seconds with %d steps",
                    finalTime, player.steps);
        }
        else if (player.timeExpired)
        {
            sprintf(buffer, "Time's up! Try again later. Steps taken: %d",
                    player.steps);
        }
        else
        {
            sprintf(buffer, "No valid path to destination! Try a different maze.");
        }

        outtextxy(50, getmaxy() / 2, buffer);

        delay(2000);
    }
}

//for drawing the menu
void drawMenu()
{
    cleardevice();

    setbkcolor(BLACK);
    setcolor(WHITE);

    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, getmaxx(), getmaxy());

    outtextxy(10, 30, (char *)"Select an Option:");

    setcolor(WHITE);
    rectangle(10, 80, 275, 115);
    outtextxy(20, 90, (char *)"1. Play Maze Game");

    rectangle(10, 130, 275, 165);
    outtextxy(20, 140, (char *)"2. Solve Maze by BFS");

    rectangle(10, 180, 275, 215);
    outtextxy(20, 190, (char *)"3. Solve Maze by DFS");

    rectangle(10, 230, 275, 265);
    outtextxy(20, 240, (char *)"4. Solve Maze by A*");

    rectangle(10, 280, 275, 315);
    outtextxy(20, 290, (char *)"5. Save Maze (Solver)");

    rectangle(10, 330, 275, 365);
    outtextxy(20, 340, (char *)"6. Load Maze (Solver)");
}

int main()
{   
    
    int startingRow = 0, startingCol = 0;
    int goalRow = MAZE_ROWS - 1, goalCol = MAZE_COLS - 1;

    char data[] = "C:\\MinGW\\lib\\libbgi.a";

    int graphicsDriver = DETECT, graphicsMode;
    initgraph(&graphicsDriver, &graphicsMode, data);

    SolverProgram::generateMaze();
    drawMenu();

    int x, y;
    while (true)
    {

        if (ismouseclick(WM_LBUTTONDOWN))
        {

            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= 10 && x <= 275)
            {

                if (y >= 80 && y <= 115)
                {
                    for (int i = 0; i < MAZE_ROWS; i++)
                    {
                        for (int j = 0; j < MAZE_COLS; j++)
                        {
                            GameProgram::mazeLayout[i][j] = SolverProgram::mazeLayout[i][j];
                        }
                    }

                    GameProgram::goalFound = 0;
                    memset(GameProgram::pathTaken, 0, sizeof(GameProgram::pathTaken));
                    GameProgram::playMazeGame();

                    for (int i = 0; i < MAZE_ROWS; i++)
                    {
                        for (int j = 0; j < MAZE_COLS; j++)
                        {
                            SolverProgram::mazeLayout[i][j] = GameProgram::mazeLayout[i][j];
                        }
                    }

                    drawMenu();

                }
                else if (y >= 130 && y <= 165)
                {

                    SolverProgram::goalFound = 0;
                    memset(SolverProgram::pathTaken, 0, sizeof(SolverProgram::pathTaken));
                    cleardevice();

                    SolverProgram::renderMaze();
                    SolverProgram::bfsSolveMaze(startingRow, startingCol);
                    cout << (SolverProgram::goalFound ? "Goal found with BFS!" : "No path found with BFS!") << endl;

                    drawMenu();

                }
                else if (y >= 180 && y <= 215)
                {

                    SolverProgram::goalFound = 0;
                    memset(SolverProgram::pathTaken, 0, sizeof(SolverProgram::pathTaken));
                    cleardevice();

                    SolverProgram::renderMaze();
                    SolverProgram::dfsSolveMaze(startingRow, startingCol);

                    cout << (SolverProgram::goalFound ? "Goal found with DFS!" : "No path found with DFS!") << endl;
                    drawMenu();

                }
                else if (y >= 230 && y <= 265)
                {

                    SolverProgram::goalFound = 0;
                    memset(SolverProgram::pathTaken, 0, sizeof(SolverProgram::pathTaken));
                    cleardevice();
                    SolverProgram::renderMaze();
                    if (SolverProgram::aStar(startingRow, startingCol, goalRow, goalCol))
                    {
                        cout << "Goal found with A*!" << endl;
                    }
                    else
                    {
                        cout << "No path found with A*!" << endl;
                    }
                    drawMenu();

                }
                else if (y >= 280 && y <= 315)
                {

                    SolverProgram::saveMaze("maze_solver.txt");
                    drawMenu();

                }
                else if (y >= 330 && y <= 365)
                {

                    SolverProgram::loadMaze("maze_solver.txt");
                    SolverProgram::goalFound = 0;
                    memset(SolverProgram::pathTaken, 0, sizeof(SolverProgram::pathTaken));
                    cleardevice();

                    SolverProgram::renderMaze();

                    drawMenu();

                }
            }
        }
    }

    getch();
    closegraph();


    return 0;
}