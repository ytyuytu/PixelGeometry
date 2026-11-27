
#ifndef STACK_H
#define STACK_H

#include "graphics.h"

struct Node {
    struct Point data;
    struct Node* next;
};

struct Stack {
    struct Node* top;
};

void init_stack(struct Stack* s);

int is_empty(struct Stack* s);

int push(struct Stack* s, struct Point point);

int pop(struct Stack* s, struct Point* point);

void free_stack(struct Stack* s);

#endif 
