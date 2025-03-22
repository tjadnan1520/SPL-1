#ifndef DFS_R_H
#define DFS_R_H
#include <iostream>
#include <graphics.h>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <cstring>

#define MAZE_ROWS 16
#define MAZE_COLS 21

using namespace std;

struct Stack
{
    struct Point
    {
        int x, y;
    } data[MAZE_ROWS * MAZE_COLS];

    int top = -1;

    void push(int x, int y)
    {
        data[++top] = {x, y};
    }

    Point pop()
    {
        return data[top--];
    }

    bool empty()
    {
        return top == -1;
    }
}; 

void dfsSolveMaze(int startX, int startY);
void dfsExplorePaths(Stack &s);
void drawPathOnMaze();
void customDelay(int milliseconds);
void renderMaze();
void loadMaze(const char *filename);
void saveMaze(const char *filename);
void generateMaze();
#endif