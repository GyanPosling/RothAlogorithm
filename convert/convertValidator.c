#include "convertValidator.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isValidNumber(const char* number, int base) {
    if (base < 2 || base > 16) return false;
    if (!number || !*number) return false;

    bool hasDot = false;
    for (; *number; number++) {
        if (*number == '.') {
            if (hasDot) return false;
            hasDot = true;
            continue;
        }

        char currentChar = toupper(*number);
        int digitValue;

        if (currentChar >= '0' && currentChar <= '9') {
            digitValue = currentChar - '0';
        } else if (currentChar >= 'A' && currentChar <= 'F') {
            digitValue = currentChar - 'A' + 10;
        } else {
            return false;
        }

        if (digitValue >= base) return false;
    }
    return true;
}

const char* getValidNumber(int base) {
    static char numberBuffer[100];

    while (1) {
        printf("Enter number in base %d: ", base);
        if (!fgets(numberBuffer, sizeof(numberBuffer), stdin)) continue;

        numberBuffer[strcspn(numberBuffer, "\n")] = '\0';

        if (isValidNumber(numberBuffer, base)) {
            return numberBuffer;
        }

        printf("Invalid number for base %d! Try again.\n", base);
    }
}

int getValidBase(int min, int max) {
    int num;
    char symbol;

    while (1) {
        char buffer[100];

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input!\nTry again: ");
            continue;
        }

        if (sscanf(buffer, "%d %c", &num, &symbol) != 1) {
            printf("Invalid input! Please enter a number.\nTry again: ");
            continue;
        }

        if (num < min || num > max) {
            printf("The base is not within the range [%d, %d]!\nTry again: ", min, max);
            continue;
        }

        return num;
    }
}
