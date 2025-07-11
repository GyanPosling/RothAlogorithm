#include "stackImplementation.h"

void push(Stack **s, double value) {
    Stack *newNode = malloc(sizeof(Stack));
    newNode->data = value;
    newNode->next = *s;
    *s = newNode;
}

double pop(Stack **s) {
    if (*s == NULL) return NAN;
    Stack *temp = *s;
    double val = temp->data;
    *s = (*s)->next;
    free(temp);
    return val;
}

bool isEmpty(Stack *s) {
    return s == NULL;
}