
#ifndef SCANLINE_H
#define SCANLINE_H

#include "graphics.h"

enum IntervalType {
    EMPTY_INTERVAL,
	NORMAl_INTERVAL,
};

struct Interval {
    int start;
    int end;
    enum IntervalType type;
};

void scanlineFill(struct Grid* grid,struct Point* vertices,int vertexCount);;
#endif
