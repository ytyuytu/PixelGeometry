
#ifndef GRAPHICS_H
#define GRAPHICS_H

enum {
    GRID_WIDTH = 40,
    GRID_HEIGHT = 20,
    MAX_VERTICES = 8
};

enum Pixel {
    EMPTY,
    BORDER,
    FILLED
};

enum Connectivity {
    FOUR_CONNECTED = 4,
    EIGHT_CONNECTED = 8
};

struct Grid {
    enum Pixel pixels[GRID_HEIGHT][GRID_WIDTH];
};

struct Point {
    int x;
    int y;
};

void initializeGrid(struct Grid* grid);

void printGrid(const struct Grid* grid);

int isInsideGrid(int x, int y);

void drawLine(struct Grid* grid, struct Point start, struct Point end, int* pixelCount, int* stepCount);

void drawLineDDA(struct Grid* grid, struct Point start, struct Point end, int* pixelCount, int* stepCount);

void drawTriangle(struct Grid* grid,struct Point vertices[], int vertexCount);

void fillArea(struct Grid* grid,struct Point seed,enum Connectivity connectivity);

void scanlineFill(struct Grid* grid,struct Point vertices[], int vertexCount);

int countPixels(const struct Grid* grid, enum Pixel type);

void saveGrid(const struct Grid* grid, const char* filename);

#endif
