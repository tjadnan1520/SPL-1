#include "DFS_R.h"

#define CELL_SIZE 30

int mazeLayout[MAZE_ROWS][MAZE_COLS];
int goalFound = 0;
int pathTaken[MAZE_ROWS][MAZE_COLS];

using namespace std;
// Implementation of a stack


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
}

void dfsExplorePaths(Stack &s)
{
    while (!s.empty() && !goalFound)
    {
        Stack::Point current = s.pop();

        int x = current.x;
        int y = current.y;

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

void dfsSolveMaze(int startX, int startY)
{
    Stack s;
    s.push(startX, startY);
    dfsExplorePaths(s);
}

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

    rectangle(10, 130, 275, 165);
    outtextxy(20, 140, (char *)"2. Solve Maze by DFS");

    rectangle(10, 180, 275, 215);
    outtextxy(20, 190, (char *)"3. Save Maze");

    rectangle(10, 230, 275, 265);
    outtextxy(20, 240, (char *)"4. Load Maze");

    int x, y;

    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
if (x >= 10 && x <= 275 && y >= 130 && y <= 165)
            {
                goalFound = 0;
                memset(pathTaken, 0, sizeof(pathTaken));
                cleardevice();
                renderMaze();
                dfsSolveMaze(startingRow, startingCol);

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
        }
    }

    getch();
    closegraph();

    return 0;
}
