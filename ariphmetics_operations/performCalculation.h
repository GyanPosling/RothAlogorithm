#pragma once

#include"rpnImplementation.h"
#include"ariphmeticValidators.h"


double toDecimale(const char *number, int base);
char* fromDecimale(double num, int base);
void performCalculation();