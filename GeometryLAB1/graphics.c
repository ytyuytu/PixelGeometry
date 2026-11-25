
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "stack.h"

void initializeGrid(struct Grid* grid){
    int x, y;

    for (y = 0; y < GRID_HEIGHT; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {
            grid->pixels[y][x] = EMPTY;
        }
    }
}

int isInsideGrid(int x, int y)
{
    return x >= 0 && x < GRID_WIDTH &&
        y >= 0 && y < GRID_HEIGHT;
}

void printGrid(const struct Grid* grid)
{
    int x, y;

    for (y = 0; y < GRID_HEIGHT; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {

            if (grid->pixels[y][x] == EMPTY) {
                printf(".");
            }
            else if (grid->pixels[y][x] == BORDER) {
                printf("#");
            }
            else if (grid->pixels[y][x] == FILLED) {
                printf("*");
            }
        }

        printf("\n");
    }
}

void drawLine(struct Grid* grid,struct Point start,struct Point end,int* pixelCount,int* stepCount){
    int deltaX = abs(end.x - start.x);
    int deltaY = abs(end.y - start.y);

    int stepX;
    int stepY;

    int error = deltaX - deltaY;

    *pixelCount = 0;
    *stepCount = 0;

    if (start.x < end.x) {
        stepX = 1;
    }
    else {
        stepX = -1;
    }

    if (start.y < end.y) {
        stepY = 1;
    }
    else {
        stepY = -1;
    }

    while (1) {

        if (isInsideGrid(start.x, start.y)) {
            grid->pixels[start.y][start.x] = BORDER;
            (*pixelCount)++;
        }

        if (start.x == end.x && start.y == end.y) {
            break;
        }

        int doubleError = 2 * error;

        if (doubleError > -deltaY) {
            error = error - deltaY;
            start.x = start.x + stepX;
        }

        if (doubleError < deltaX) {
            error = error + deltaX;
            start.y = start.y + stepY;
        }

        (*stepCount)++;
    }
}

void drawLineDDA(struct Grid* grid,struct Point start,struct Point end,int* pixelCount,int* stepCount){
    int deltaX = end.x - start.x;
    int deltaY = end.y - start.y;

    int steps;
    int i;

    float xStep;
    float yStep;

    float x = (float)start.x;
    float y = (float)start.y;

    int pixelX, pixelY;

    if (abs(deltaX) > abs(deltaY)) {
        steps = abs(deltaX);
    }
    else {
        steps = abs(deltaY);
    }

    *pixelCount = 0;
    *stepCount = steps;

    if (steps == 0) {
        if (isInsideGrid(start.x, start.y)) {
            grid->pixels[start.y][start.x] = BORDER;
            (*pixelCount)++;
        }
        return;
    }

    xStep = (float)deltaX / steps;
    yStep = (float)deltaY / steps;

    for (i = 0; i <= steps; i++) {

        pixelX = (int)(x + 0.5f);
        pixelY = (int)(y + 0.5f);

        if (isInsideGrid(pixelX, pixelY)) {
            grid->pixels[pixelY][pixelX] = BORDER;
            (*pixelCount)++;
        }

        x = x + xStep;
        y = y + yStep;
    }
}

void drawTriangle(struct Grid* grid, struct Point vertices[],int vertexCount){
    int pixelCount;
    int stepCount;

    if (vertexCount != 3) {
        return;
    }

    drawLine(grid, vertices[0], vertices[1], &pixelCount, &stepCount);
    drawLine(grid, vertices[1], vertices[2], &pixelCount, &stepCount);
    drawLine(grid, vertices[2], vertices[0], &pixelCount, &stepCount);
}

void fillArea(struct Grid* grid,struct Point seed,enum Connectivity connectivity){
    struct Stack stack;
    struct Point current;
    struct Point neighbor;

    int dx, dy;

    if (!isInsideGrid(seed.x, seed.y)) {
        return;
    }

    if (grid->pixels[seed.y][seed.x] != EMPTY) {
        return;
    }

    init_stack(&stack);

    grid->pixels[seed.y][seed.x] = FILLED;

    if (!push(&stack, seed)) {
        free_stack(&stack);
        return;
    }

    while (pop(&stack, &current)) {

        for (dy = -1; dy <= 1; dy++) {
            for (dx = -1; dx <= 1; dx++) {

                if (dx == 0 && dy == 0) {
                    continue;
                }

                if (connectivity == FOUR_CONNECTED && dx != 0 && dy != 0) {
                    continue;
                }

                neighbor.x = current.x + dx;
                neighbor.y = current.y + dy;

                if (isInsideGrid(neighbor.x, neighbor.y) && grid->pixels[neighbor.y][neighbor.x] == EMPTY) {
                    grid->pixels[neighbor.y][neighbor.x] = FILLED;

                    if (!push(&stack, neighbor)) {
                        free_stack(&stack);
                        return;
                    }
                }
            }
        }
    }

    free_stack(&stack);
}

int countPixels(const struct Grid* grid, enum Pixel type)
{
    int x, y;
    int count = 0;

    for (y = 0; y < GRID_HEIGHT; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {
            if (grid->pixels[y][x] == type) {
                count++;
            }
        }
    }

    return count;
}

void saveGrid(const struct Grid* grid, const char* filename)
{
    FILE* file;
    int x, y;

    file = fopen_s(&file, filename, "w");

    if (file == NULL) {
        printf("File error\n");
        return;
    }

    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", GRID_WIDTH, GRID_HEIGHT);
    fprintf(file, "255\n");

    for (y = 0; y < GRID_HEIGHT; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {

            if (grid->pixels[y][x] == EMPTY) {
                fprintf(file, "255 ");
            }
            else if (grid->pixels[y][x] == BORDER) {
                fprintf(file, "0 ");
            }
            else if (grid->pixels[y][x] == FILLED) {
                fprintf(file, "120 ");
            }
        }

        fprintf(file, "\n");
    }

    fclose(file);

    printf("Image saved successfully\n");
}


int quiksort(double* arr, int start, int stop) {
    int i, j;
    double pivot, elem;

    if (stop - start <= 1) {
        return 0;
    }

    pivot = arr[(start + stop) / 2];

    for (i = start, j = stop - 1; i <= j;) {

        for (; i <= j && arr[i] < pivot; i++) {
        }

        for (; i <= j && arr[j] > pivot; j--) {
        }

        if (i = j) {
            i++;
            j--;
        }
        else if (i < j) {
            elem = arr[i];
            arr[i] = arr[j];
            arr[j] = elem;

            i++;
            j--;
        }
    }
}



void scanlineFill1(struct Grid* grid, struct Point vertices[], int vertexCount){
    int minY, maxY;
    int i, j, row, column;
    int intersectionCount;

    double* intersections;
    double temp;
    double x;

	intersections = malloc(vertexCount * sizeof(double));

    if (grid == NULL || vertices == NULL) {
        return;
    }

    minY = vertices[0].y;
    maxY = vertices[0].y;

    for (i = 0; i < vertexCount; i++) {

        if (vertices[i].x < 0 || vertices[i].x >= GRID_WIDTH ||vertices[i].y < 0 || vertices[i].y >= GRID_HEIGHT) {
            return;
        }

        if (vertices[i].y < minY) {
            minY = vertices[i].y;
        }

        if (vertices[i].y > maxY) {
            maxY = vertices[i].y;
        }
    }

    for (row = minY; row < maxY; row++) {

        intersectionCount = 0;

        for (i = 0; i < vertexCount; i++) {

            j = (i + 1) % vertexCount;

            int x1 = vertices[i].x;
            int y1 = vertices[i].y;
            int x2 = vertices[j].x;
            int y2 = vertices[j].y;

            if (y1 == y2) {
                continue;
            }
            if ((row >= y1 && row < y2) || (row >= y2 && row < y1)) {

                x = x1 + (double)(row - y1) * (x2 - x1) / (y2 - y1);

                intersections[intersectionCount] = x;
                intersectionCount++;
            }
        }

        if (intersectionCount % 2 != 0) {
            continue;
        }
       
		quiksort(intersections, 0, intersectionCount);

        for (i = 0; i < intersectionCount; i += 2) {

            for (column = 0; column < GRID_WIDTH; column++) {

                if (column >= intersections[i] && column <= intersections[i + 1]) {

                    if (grid->pixels[row][column] == EMPTY) {
                        grid->pixels[row][column] = FILLED;
                    }
                }
            }
        }
    }
    free(intersections);
}

