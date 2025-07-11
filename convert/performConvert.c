#include "performConvert.h"
#include "convertValidator.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double toDecimal(const char* number, int base) {
    double result = 0.0;
    const char* dotPos = strchr(number, '.');
    const char* end = dotPos ? dotPos : number + strlen(number);

    for (const char* p = number; p < end; p++) {
        char c = toupper(*p);
        int digit = isdigit(c) ? c - '0' : c - 'A' + 10;
        result = result * base + digit;
    }

    if (dotPos) {
        double fraction = 0.0;
        double basePower = 1.0 / base;

        for (const char* p = dotPos + 1; *p; p++) {
            char c = toupper(*p);
            int digit = isdigit(c) ? c - '0' : c - 'A' + 10;
            fraction += digit * basePower;
            basePower /= base;
        }
        result += fraction;
    }

    return result;
}

char* fromDecimal(double value, int base, int precision) {
    long intPart = (long)value;
    double fracPart = fabs(value - intPart);

    char intBuffer[65];
    char* ptr = intBuffer + sizeof(intBuffer) - 1;
    *ptr = '\0';

    if (intPart == 0) {
        *(--ptr) = '0';
    } else {
        while (intPart > 0) {
            int digit = intPart % base;
            *(--ptr) = digit < 10 ? digit + '0' : digit - 10 + 'A';
            intPart /= base;
        }
    }

    char fracBuffer[20] = {0};
    if (fracPart > 0 && precision > 0) {
        fracBuffer[0] = '.';
        for (int i = 1; i <= precision; i++) {
            fracPart *= base;
            int digit = (int)fracPart;
            fracBuffer[i] = digit < 10 ? digit + '0' : digit - 10 + 'A';
            fracPart -= digit;
        }
    }

    char* result = malloc(strlen(ptr) + strlen(fracBuffer) + 1);
    strcpy(result, ptr);
    strcat(result, fracBuffer);

    return result;
}

void performConversion(int fromBase, int toBase, const char* number) {
    double decimalValue = toDecimal(number, fromBase);
    char* convertedNumber = fromDecimal(decimalValue, toBase, 10);

    printf("Conversion result: \n%s (base %d) -> %s (base %d)\n",
           number, fromBase, convertedNumber, toBase);

    free(convertedNumber);
}

void performConvert(){
    printf("\n==================== Convert from different number systems ====================\n\n");
    printf("Enter the base from which you want to convert the number: ");
    int fromBase = getValidBase(2, 16);
    printf("\nEnter number in %d base: ", fromBase);
    const char* number = getValidNumber(fromBase);
    printf("\nEnter the base in which you want to convert your number: ");
    int toBase = getValidBase(2, 16);
    performConversion(fromBase, toBase, number);
}