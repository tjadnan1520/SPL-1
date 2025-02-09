 #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>  
#include <graphics.h>

#define ROWS 16
#define COLS 21
#define CELL_SIZE 30

typedef struct {
    int x, y;   
    int g;      
    int h;      
    int f;      
} Node;


int calculateManhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


bool isValidCell(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS);
}
void drawCell(int x, int y, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
}


bool aStar(int grid[ROWS][COLS], Node start, Node goal) {
    
    char data[] = "C:\\MinGW\\lib\\libbgi.a";

    int graphicsDriver = DETECT, graphicsMode;
    initgraph(&graphicsDriver, &graphicsMode, data);

    
    Node openList[ROWS * COLS];
    Node closedList[ROWS * COLS];

    int openCount = 0;
    int closedCount = 0;

    
    openList[openCount++] = start;

    while (openCount > 0) {
        int currentIndex = 0;
        for (int i = 1; i < openCount; i++) {
            if (openList[i].f < openList[currentIndex].f) {
                currentIndex = i;
            }
        }

        
        Node current = openList[currentIndex];
        openCount--;
        for (int i = currentIndex; i < openCount; i++) {
            openList[i] = openList[i + 1];
        }

        
        grid[current.x][current.y] = 2;

        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (grid[i][j] == 1) {
                    drawCell(i, j, BLACK);  
                } else if (grid[i][j] == 2) {
                    drawCell(i, j, LIGHTGREEN);  
                } else {
                    drawCell(i, j, WHITE);  
                }
            }
        }

            delay(100);

        
        if (current.x == goal.x && current.y == goal.y) {
            closegraph();
            return true;
        }

       
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            
            if (isValidCell(newX, newY) && grid[newX][newY] != 1) {
                Node neighbor;
                neighbor.x = newX;
                neighbor.y = newY;
                neighbor.g = current.g + 1;
                neighbor.h = calculateManhattanDistance(newX, newY, goal.x, goal.y);
                neighbor.f = neighbor.g + neighbor.h;

                
                bool inOpenList = false;
                for (int j = 0; j < openCount; j++) {
                    if (openList[j].x == neighbor.x && openList[j].y == neighbor.y && openList[j].f <= neighbor.f) {
                        inOpenList = true;
                        break;
                    }
                }

                
                if (!inOpenList) {
                    openList[openCount++] = neighbor;
                }
            }
        }
    }

    closegraph();
    printf("Path not found!\n");
    return false;
}

int main() {
    int grid[ROWS][COLS] = {
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1}
    };

    Node start = {0, 0, 0, 0, 0};
    Node goal = {15, 15, 0, 0, 0};

    if (aStar(grid, start, goal)) {
        printf("Path found!\n");
    }

    getch();  
    return 0;    ;
}
