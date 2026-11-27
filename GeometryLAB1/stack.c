#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void init_stack(struct Stack* s){
    s->top = NULL;
}

int is_empty(struct Stack* s){
    return s->top == NULL;
}

int push(struct Stack* s, struct Point point){
    struct Node* new_node = malloc(sizeof(struct Node));

    if (new_node == NULL) {
        return 0;
    }

    new_node->data = point;
    new_node->next = s->top;
    s->top = new_node;

    return 1;
}

int pop(struct Stack* s, struct Point* point){
    struct Node* temp;

    if (s->top == NULL) {
        return 0;
    }

    temp = s->top;
    *point = temp->data;

    s->top = temp->next;

    free(temp);

    return 1;
}

void free_stack(struct Stack* s){
    struct Point point;

    while (!is_empty(s)) {
        pop(s, &point);
    }
}


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
