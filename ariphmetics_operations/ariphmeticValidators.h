#pragma once

#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include"performCalculation.h"

bool parseAndReplaceNumbers(char **expr, int base);
char* getExpression();
int getValidateBase(int min, int max);
bool isValidateNumber(const char *number, int base);
