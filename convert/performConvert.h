#ifndef PERFORMCONVERT_H
#define PERFORMCONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double toDecimal(const char* number, int base);
char* fromDecimal(double value, int base, int precision);
void performConversion(int fromBase, int toBase, const char* number);
void performConvert();
#endif