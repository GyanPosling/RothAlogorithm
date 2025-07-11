#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

typedef struct Stack {
    double data;
    struct Stack *next;
} Stack;

void push(Stack **s, double value);
double pop(Stack **s);
bool isEmpty(Stack *s);
