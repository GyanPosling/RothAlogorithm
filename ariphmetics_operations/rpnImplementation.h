#pragma once

#include"stackImplementation.h"
#include"ariphmeticValidators.h"

int getPriority(char op);
bool isOperator(char c);
double evaluateRPN(char *rpn);
char* shuntingYard(const char *expr);
