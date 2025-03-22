#ifndef A_STAR_R_H
#define A_STAR_R_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>  
#include <graphics.h>
#include <time.h>  
using namespace std;
typedef struct {
    int x, y;   
    int g;      
    int h;      
    int f;      
} Node;
#define ROWS 16
#define COLS 21
int calculateManhattanDistance(int x1, int y1, int x2, int y2);
bool isValidCell(int x, int y);
void drawCell(int x, int y, int color);
void generateMaze(int grid[ROWS][COLS]);
bool aStar(int grid[ROWS][COLS], Node start, Node goal);

#endif