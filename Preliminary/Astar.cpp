#include <bits/stdc++.h>
#include <graphics.h>
#include <stack>
#include <ctime>
#include <conio.h>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

#define MAZE_ROWS_of_Astar 16
#define MAZE_COLS_of_Astar 21
#define CELL_SIZE_of_Astar 30

int mazeLayoutOfA_star[MAZE_ROWS_of_Astar][MAZE_COLS_of_Astar] =
    {
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
};

int goalFoundofAstar = 0;
int pathTakenofAstar[MAZE_ROWS_of_Astar][MAZE_COLS_of_Astar];
int distanceofAstar[MAZE_ROWS_of_Astar][MAZE_COLS_of_Astar];

// Function to render the maze
void renderMazeOfAstar()
{
    for (int i = 0; i < MAZE_ROWS_of_Astar; i++)
    {
        for (int j = 0; j < MAZE_COLS_of_Astar; j++)
        {
            int xCoord = j * CELL_SIZE_of_Astar;
            int yCoord = i * CELL_SIZE_of_Astar;

            if (mazeLayoutOfA_star[i][j] == 1)
            {
                rectangle(xCoord, yCoord, xCoord + CELL_SIZE_of_Astar, yCoord + CELL_SIZE_of_Astar);
            }
            else if (mazeLayoutOfA_star[i][j] == 2)
            {
                circle(xCoord + CELL_SIZE_of_Astar / 2, yCoord + CELL_SIZE_of_Astar / 2, CELL_SIZE_of_Astar / 3);
            }
        }
    }
}

void customDelayOfAstar(int milliseconds)
{
    int end_time = clock() + milliseconds;
    while (clock() < end_time) {}
}

void drawPathOnMazebyAstar()
{
    for (int i = 0; i < MAZE_ROWS_of_Astar; i++)
    {
        for (int j = 0; j < MAZE_COLS_of_Astar; j++)
        {
            if (pathTakenofAstar[i][j] == 1)
            {
                setfillstyle(SOLID_FILL, CYAN);
                bar(j * CELL_SIZE_of_Astar + CELL_SIZE_of_Astar / 6, i * CELL_SIZE_of_Astar + CELL_SIZE_of_Astar / 6,
                    (j + 1) * CELL_SIZE_of_Astar - CELL_SIZE_of_Astar / 6, (i + 1) * CELL_SIZE_of_Astar - CELL_SIZE_of_Astar / 6);
            }
        }
    }
}

// Heuristic function for A* algorithm
int calculateHeuristic(int x, int y, int endX, int endY)
{
    // Euclidean distance heuristic
    return static_cast<int>(sqrt((x - endX) * (x - endX) + (y - endY) * (y - endY)));
}

void aStar(int startX, int startY, int endX, int endY)
{
    const int INF = INT_MAX;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < MAZE_ROWS_of_Astar; i++)
    {
        for (int j = 0; j < MAZE_COLS_of_Astar; j++)
        {
            ::distanceofAstar[i][j] = INF;
            pathTakenofAstar[i][j] = 0;
        }
    }

    ::distanceofAstar[startX][startY] = 0;

    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > > pq;
    pq.push({calculateHeuristic(startX, startY, endX, endY), {startX, startY}});

    while (!pq.empty())
    {
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        pq.pop();

        pathTakenofAstar[x][y] = 1;

        if (x == endX && y == endY)
        {
            goalFoundofAstar = 1;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < MAZE_ROWS_of_Astar && ny >= 0 && ny < MAZE_COLS_of_Astar && !pathTakenofAstar[nx][ny] && mazeLayoutOfA_star[nx][ny] != 1)
            {
                int newDist = ::distanceofAstar[x][y] + 1;
                if (newDist < ::distanceofAstar[nx][ny])
                {
                    ::distanceofAstar[nx][ny] = newDist;
                    pq.push({newDist + calculateHeuristic(nx, ny, endX, endY), {nx, ny}});
                    customDelayOfAstar(100);

                    cleardevice();
                    renderMazeOfAstar();
                    drawPathOnMazebyAstar();
                }
            }
        }
    }
}

void A_Star()
{
    int startingRow = 0;
    int startingCol = 0;

    char data[] = "C:\\MinGW\\lib\\libbgi.a";

    int graphicsDriver = DETECT, graphicsMode;
    initgraph(&graphicsDriver, &graphicsMode, data);

    // Initially, display a black screen
    cleardevice();

    outtextxy(10, 50,(char*) "Hit the Button to solve the Maze");

    rectangle(10, 80, 275, 115);
    outtextxy(20, 90, (char*)"1. Solve Maze by A* Algorithm");

    int x, y;

    //cout << "Enter Starting Row and Column: ";
    //cin >> startingRow;
    //cin >> startingCol;

    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= 10 && x <= 275 && y >= 20 && y <= 115)
            {
                goalFoundofAstar = 0;
                cleardevice();
                renderMazeOfAstar();
                aStar(startingRow, startingCol, 15, 15);

                if (goalFoundofAstar)
                {
                    cout << "Path to reach the goal: " << endl;
                    for (int i = 0; i < MAZE_ROWS_of_Astar; i++)
                    {
                        for (int j = 0; j < MAZE_COLS_of_Astar; j++)
                        {
                            if (pathTakenofAstar[i][j] == 1)
                            {
                                cout << "-> [" << i << "," << j << "] ";
                            }
                        }
                    }
                }
                else
                {
                    cout << "Path to the goal not found!";
                }
            }
        }
    }

    getch();
    closegraph();
}
