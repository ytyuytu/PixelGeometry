#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "scanline.h"


static void initIntervals(struct Interval* intervals){
    int i;

    for (i = 0; i < GRID_WIDTH; i++) {
        intervals[i].start = 0;
        intervals[i].end = 0;
        intervals[i].type = EMPTY_INTERVAL;
    }
}


static void clearIntervals(struct Interval* intervals){
    int i;

    for (i = 0; intervals[i].type != EMPTY_INTERVAL; i++) {
        intervals[i].start = 0;
        intervals[i].end = 0;
        intervals[i].type = EMPTY_INTERVAL;
    }
}

static void addInterval(struct Interval* intervals,int start, int end,enum IntervalType type){
    int i = 0;

    while (i < GRID_WIDTH - 1 &&
        intervals[i].type != EMPTY_INTERVAL) {
        i++;
    }

    if (i < GRID_WIDTH - 1) {
        intervals[i].start = start;
        intervals[i].end = end;
        intervals[i].type = type;

        intervals[i + 1].start = 0;
        intervals[i + 1].end = 0;
        intervals[i + 1].type = EMPTY_INTERVAL;
    }
}

static void findIntervals(struct Grid* grid, int row, struct Interval* foundIntervals) {

    int column = 0;
    int start;
    int end;
    int borderCount = 0;
    int leftBorderEnd = 0;

    while (column < GRID_WIDTH) {

        if (grid->pixels[row][column] != BORDER) {
            column++;
            continue;
        }

        start = column;

        while (column < GRID_WIDTH &&  grid->pixels[row][column] == BORDER) {
            column++;
        }

        end = column - 1;
        borderCount++;

        if (borderCount % 2 == 1) {
            leftBorderEnd = end;
        }

        else {
            addInterval(foundIntervals, leftBorderEnd + 1, start - 1, NORMAl_INTERVAL);
        }
    }
}

static int intervalsOverlap(struct Interval first, struct Interval second){
    return first.start <= second.end && first.end >= second.start;
}

static int overlapsPrevious(struct Interval interval, struct Interval* previousIntervals){
    int i = 0;

    while (previousIntervals[i].type != EMPTY_INTERVAL) {
        if (intervalsOverlap(interval, previousIntervals[i])) {
            return 1;
        }

        i++;
    }
    return 0;
}

static void fillInterval(struct Grid* grid,int row,int start, int end){
    int column;

    for (column = start; column <= end; column++) {
        if (column >= 0 && column < GRID_WIDTH && grid->pixels[row][column] != BORDER) {
            grid->pixels[row][column] = FILLED;
        }
    }
}

static void findIntervalsZERO(struct Point vertices[], int vertexCount, int* topRow, struct Interval* foundIntervals) {
    int i;
    int toprow_index = 0;

    *topRow = vertices[0].y;

    for (i = 1; i < vertexCount; i++) {
        if (vertices[i].y < *topRow) {
            *topRow = vertices[i].y;
        }
    }

    for (i = 0; i < vertexCount; i++) {
        if (vertices[i].y == *topRow && vertices[(i + vertexCount - 1) % vertexCount].y != *topRow) {
            toprow_index = i;
        }
        if (vertices[i].y == *topRow && vertices[(i + 1) % vertexCount].y != *topRow) {

            addInterval(foundIntervals,vertices[toprow_index].x,vertices[i].x,NORMAl_INTERVAL);
        }
    }
}




void scanlineFill(struct Grid* grid, struct Point vertices[], int vertexCount) {
    struct Interval* foundIntervals;
    struct Interval* currentIntervals;
    struct Interval* previousIntervals;
    struct Interval* tempIntervals;

    int topRow;
    int botRow;
    int row;
    int i;

    if (grid == NULL || vertices == NULL || vertexCount < 3) {
        return;
    }

    foundIntervals = malloc(GRID_WIDTH * sizeof(struct Interval));
    currentIntervals = malloc(GRID_WIDTH * sizeof(struct Interval));
    previousIntervals = malloc(GRID_WIDTH * sizeof(struct Interval));

    if (foundIntervals == NULL || currentIntervals == NULL || previousIntervals == NULL) {
        free(foundIntervals);
        free(currentIntervals);
        free(previousIntervals);
        return;
    }

    initIntervals(foundIntervals);
    initIntervals(currentIntervals);
    initIntervals(previousIntervals);



	findIntervalsZERO(vertices, vertexCount, &topRow, previousIntervals);

	botRow = vertices[0].y;

    for (i = 1; i < vertexCount; i++) {
        if (vertices[i].y > botRow) {
            botRow = vertices[i].y;
        }
    }

    for (row = topRow + 1; row <= botRow; row++) {
        clearIntervals(foundIntervals);
        clearIntervals(currentIntervals);

        findIntervals(grid, row, foundIntervals);

        for (i = 0; i < GRID_WIDTH &&
            foundIntervals[i].type != EMPTY_INTERVAL; i++) {

            if (overlapsPrevious(foundIntervals[i], previousIntervals)) {
                fillInterval(grid, row,foundIntervals[i].start,foundIntervals[i].end);
                addInterval(currentIntervals,foundIntervals[i].start,foundIntervals[i].end,NORMAl_INTERVAL);
            }
        }

        tempIntervals = previousIntervals;
        previousIntervals = currentIntervals;
        currentIntervals = tempIntervals;
    }

    free(foundIntervals);
    free(currentIntervals);
    free(previousIntervals);
}
