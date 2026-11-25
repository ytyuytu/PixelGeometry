
#include <stdio.h>

#include "graphics.h"
#include "input.h"
#include "scanline.h"

int main(){
    struct Grid grid;
    struct Point start, end;
    struct Point vertices[3];
    struct Point polygon[8];
    struct Point seed;

    int choice;
    int vertexCount;
    int connectivity;
    int pixelCount = 0;
    int stepCount = 0;

    initializeGrid(&grid);

    while (1) {
        printf("\n");
        printf("====================================\n");
        printf("          RASTER GRAPHICS           \n");
        printf("====================================\n");
        printf("  1. Draw line (Bresenham)\n");
        printf("  2. Draw line (DDA)\n");
        printf("  3. Draw triangle\n");
        printf("  4. Show draw line statistics\n");
        printf("  5. Save grid to file\n");
        printf("  6. Fill area by Flood Fill\n");
        printf("  7. Fill area by Scanline Fill\n");
        printf("  8. Set up new grid\n");
        printf("  9. Print grid\n");
        printf("  0. Exit\n");
        printf("------------------------------------\n");
        printf("  Choose an option: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("\nInput error!\n");
            return 1;
        }

        switch (choice) {

        case 1:
            readPoint(&start);
            readPoint(&end);

            drawLine(&grid, start, end, &pixelCount, &stepCount);

            printf("Line drawn using Bresenham!\n");
            printGrid(&grid);
            break;

        case 2:
            readPoint(&start);
            readPoint(&end);

            drawLineDDA(&grid, start, end, &pixelCount, &stepCount);

            printf("Line drawn using DDA!\n");
            printGrid(&grid);
            break;

        case 3:
            printf("Enter coordinates of three vertices:\n");

            readTriangle(vertices);

            drawTriangle(&grid, vertices, 3);

            printf("Triangle drawn!\n");
            printGrid(&grid);
            break;

        case 4:
            printf("Pixel count: %d\n", pixelCount);
            printf("Step count: %d\n", stepCount);
            break;

        case 5:
            saveGrid(&grid, "result.pgm");
            break;

        case 6:
            printf("Choose connectivity (4 or 8): ");

            if (scanf_s("%d", &connectivity) != 1 ||
                (connectivity != 4 && connectivity != 8)) {
                printf("Invalid connectivity!\n");
                break;
            }

            readPoint(&seed);
            fillArea(&grid, seed, (enum Connectivity)connectivity);

            printf("Area filled using Flood Fill!\n");
            printGrid(&grid);
            break;

        case 7:
            printf("Enter number of vertices: ");

            if (scanf_s("%d", &vertexCount) != 1 ||
                vertexCount < 3 || vertexCount > 8) {
                printf("Invalid number of vertices!\n");
                break;
            }

            printf("Enter polygon vertices:\n");

            for (int i = 0; i < vertexCount; i++) {
                printf("Vertex %d:\n", i);

                if (!readPoint(&polygon[i])) {
                    printf("Invalid coordinates!\n");
                    break;
                }
            }

            scanlineFill(&grid, polygon, vertexCount);

            printf("Area filled using Scanline Fill!\n");
            printGrid(&grid);
            break;

        case 8:
            initializeGrid(&grid);
            pixelCount = 0;
            stepCount = 0;
            printf("New grid created!\n");
            break;

        case 9:
            printGrid(&grid);
            break;

        case 0:
            printf("Goodbye!\n");
            return 0;

        default:
            printf("Unknown option! Try again.\n");
            break;
        }
    }

    return 0;
}
