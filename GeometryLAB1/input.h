#ifndef INPUT_H
#define INPUT_H

#include "graphics.h"

int readPoint(struct Point* point);

int readTriangle(struct Point vertices[]);

int readConnectivity(enum Connectivity* connectivity);

#endif