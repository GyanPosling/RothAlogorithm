#include"performCalculation.h"

double toDecimale(const char *number, int base) {
    double result = 0.0;
    const char *dot = strpbrk(number, ".,");
    const char *end = dot ? dot : number + strlen(number);

    for (const char *p = number; p < end; p++) {
        if (*p == '.' || *p == ',') continue;
        char c = tolower(*p);
        int digit = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
        result = result * base + digit;
    }

    if (dot) {
        double frac = 0.0;
        double power = 1.0 / base;
        for (const char *p = dot + 1; *p; p++) {
            if (*p == '.' || *p == ',') continue;
            char c = tolower(*p);
            int digit = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
            frac += digit * power;
            power /= base;
        }
        result += frac;
    }
    return result;
}

char* fromDecimale(double num, int base) {
    if (base < 2 || base > 16) return NULL;

    long intPart = (long)num;
    double fracPart = fabs(num - intPart);

    char intBuffer[65];
    int i = 0;

    if (intPart == 0) {
        intBuffer[i++] = '0';
    } else {
        long temp = labs(intPart);
        while (temp > 0) {
            int digit = temp % base;
            intBuffer[i++] = (digit < 10) ? digit + '0' : digit - 10 + 'A';
            temp /= base;
        }

        if (num < 0) {
            intBuffer[i++] = '-';
        }
    }

    for (int j = 0; j < i / 2; j++) {
        char tmp = intBuffer[j];
        intBuffer[j] = intBuffer[i - j - 1];
        intBuffer[i - j - 1] = tmp;
    }
    intBuffer[i] = '\0';

    char *result = malloc(128);
    if (!result) return NULL;
    strcpy(result, intBuffer);

    if (fracPart > 0) {
        strcat(result, ".");
        int len = strlen(result);
        for (int j = 0; j < 10 && fracPart > 0; j++) {
            fracPart *= base;
            int digit = (int)fracPart;
            result[len++] = (digit < 10) ? digit + '0' : digit - 10 + 'A';
            fracPart -= digit;
        }
        result[len] = '\0';
    }

    return result;
}


void performCalculation() {
    printf("\n==================== Performing Calculation ===================\n");
    printf("Enter base in which u want to perform operation(2, 16): \n");
    int base = getValidateBase(2, 16);

    char *expr = NULL;
    char *rpn = NULL;
    double result;
    int isValidExpression = 0;

    do {
        if (expr != NULL) {
            free(expr);
            expr = NULL;
        }
        if (rpn != NULL) {
            free(rpn);
            rpn = NULL;
        }

        expr = getExpression();

        if (!parseAndReplaceNumbers(&expr, base)) {
            printf("Invalid expression. Please try again.\n");
            continue; 
        }

        rpn = shuntingYard(expr);
        if (!rpn) {
            printf("Invalid expression. Please try again.\n");
            continue;
        }

        result = evaluateRPN(rpn);
        if (isnan(result)) {
            printf("Invalid expression. Please try again.\n");
            isValidExpression = 0;
        } else {
            char *converted = fromDecimale(result, base);
            printf("Result: %s\n", converted);
            free(converted);
            isValidExpression = 1;
        }

    } while (!isValidExpression);

    free(expr);
    free(rpn);
}