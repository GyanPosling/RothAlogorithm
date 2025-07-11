#ifndef CONVERTVALIDATOR_H
#define CONVERTVALIDATOR_H

#include<stdio.h>
#include <stdbool.h>
#include<string.h>

int getValidBase(int min, int max);
bool isValidNumber(const char* number, int base);
const char* getValidNumber(int base);

#endif //CONVERTVALIDATOR_H
