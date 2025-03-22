#ifndef PLAYER_R_H 
#define PLAYER_R_H  

using namespace std; 

#include <iostream>
#include <graphics.h>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <cstring> 
#define MAZE_ROWS 16
#define MAZE_COLS 21

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

#endif