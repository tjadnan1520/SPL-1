
#include "A_star_R.h"

#define ROWS 16
#define COLS 21
#define CELL_SIZE 30




int calculateManhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


bool isValidCell(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS);
}

// Function to draw a rectangle for a cell
void drawCell(int x, int y, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
}

// Add this new function for maze generation
void generateMaze(int grid[ROWS][COLS]) {
    // Initialize all cells as walls
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = 1;
        }
    }

    // Create a path through the maze using random walks
    int currentX = 0;
    int currentY = 0;
    grid[currentX][currentY] = 0;

    while (currentX < ROWS-1 || currentY < COLS-1) {
        bool moved = false;
        
        // Try to move in a random direction
        for (int attempts = 0; attempts < 4; attempts++) {
            int direction = rand() % 4;
            int newX = currentX;
            int newY = currentY;
            
            if (direction == 0) newX += 2;  // Right
            if (direction == 1) newX -= 2;  // Left
            if (direction == 2) newY += 2;  // Down
            if (direction == 3) newY -= 2;  // Up

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && grid[newX][newY] == 1) {
                // Create path by setting cells to 0
                grid[(currentX + newX)/2][(currentY + newY)/2] = 0;
                grid[newX][newY] = 0;
                currentX = newX;
                currentY = newY;
                moved = true;
                break;
            }
        }
        
        if (!moved) {
            // If stuck, return to a previous path cell
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (grid[i][j] == 0) {
                        currentX = i;
                        currentY = j;
                        break;
                    }
                }
            }
        }
    }

    // Ensure start and end points are accessible
    grid[0][0] = 0;
    grid[ROWS-1][COLS-1] = 0;
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
                    drawCell(i, j, WHITE);  // Wall is white
                } else if (grid[i][j] == 2) {
                    drawCell(i, j, LIGHTGREEN);  // Path being explored is green
                } else {
                    drawCell(i, j, BLACK);  // Available path is black
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
    int grid[ROWS][COLS];
    srand(time(NULL));
    
    char key;
    do {
        // Clear the screen before generating new maze
        system("cls");
        
        // Generate new random maze
        generateMaze(grid);

        Node start = {0, 0, 0, 0, 0};
        Node goal = {ROWS-1, COLS-1, 0, 0, 0};

        if (aStar(grid, start, goal)) {
            printf("Path found! Press any key to generate new maze or ESC to exit\n");
        } else {
            printf("Path not found! Press any key to generate new maze or ESC to exit\n");
        }

        key = getch();  // Wait for key press
    } while (key != 27);  // 27 is ESC key
    
    return 0;
}