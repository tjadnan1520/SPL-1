#ifndef BFS_R_H 
#define BFS_R_H

#include <iostream>
#include <graphics.h>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <cstring>
using namespace std;
void generateMaze();
void saveMaze(const char *filename);
void loadMaze(const char *filename);
void renderMaze();
void customDelay(int milliseconds);
void drawPathOnMaze();
void bfsSolveMaze(int startX, int startY);
void bfsExploreMaze(int startX, int startY);
#endif