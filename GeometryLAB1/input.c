#include <stdio.h>
#include "input.h"

int readPoint(struct Point* point)
{
    printf("Enter coordinates x and y: ");

    if (scanf_s("%d %d", &point->x, &point->y) != 2) {
        return 0;
    }

    if (!isInsideGrid(point->x, point->y)) {
        return 0;
    }

    return 1;
}


int readTriangle(struct Point vertices[]){
    int i;

    for (i = 0; i < 3; i++) {
        printf("Enter vertex %d:\n", i);

        if (readPoint(&vertices[i]) == 0) {
            printf("Invalid coordinates\n");
            return 0;
        }
    }

    return 1;
}


int readConnectivity(enum Connectivity* connectivity){
    int choice;

    printf("Choose connectivity (4 or 8): ");

    if (scanf_s("%d", &choice) != 1) {
        return 0;
    }

    if (choice != 4 && choice != 8) {
        return 0;
    }

    *connectivity = (enum Connectivity)choice;

    return 1;
}