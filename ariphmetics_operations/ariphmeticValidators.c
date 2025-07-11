#include "ariphmeticValidators.h"


bool isValidateNumber(const char *number, int base) {
    if (base < 2 || base > 16) return false;
    bool hasDot = false;
    const char *start = number;
    if (*number == '-') number++; // Skip negative sign
    for (; *number; number++) {
        if (*number == '.') {
            if (hasDot || number == start || *(number + 1) == '\0') return false;
            hasDot = true;
            continue;
        }
        char c = tolower(*number);
        int digit = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : -1;
        if (digit < 0 || digit >= base) return false;
    }
    return true;
}

int getValidateBase(int min, int max) {
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

char* getExpression() {
    printf("Enter expression: ");
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
        char *str = strdup(buffer);
        if (!str) exit(1);
        for (int i = 0; str[i]; i++) {
            if (str[i] == ',') str[i] = '.';
        }
        return str;
    }
    return NULL;
}

bool parseAndReplaceNumbers(char **expr, int base) {
    char *src = *expr;
    char *dest = malloc(strlen(src) * 3 + 1);
    if (!dest) return false;
    dest[0] = '\0';

    int bracket_count = 0;
    char numBuffer[100];
    int i = 0;
    bool inNumber = false;

    while (*src) {
        if (isspace(*src)) {
            if (inNumber && i > 0) {
                numBuffer[i] = '\0';
                if (!isValidateNumber(numBuffer, base)) {
                    free(dest);
                    return false;
                }
                char decStr[50];
                snprintf(decStr, sizeof(decStr), "%.10f", toDecimale(numBuffer, base));
                // Remove trailing zeros after decimal
                char *dot = strchr(decStr, '.');
                if (dot) {
                    char *end = decStr + strlen(decStr) - 1;
                    while (end > dot && *end == '0') *end-- = '\0';
                    if (*end == '.') *end = '\0';
                }
                strcat(dest, decStr);
                i = 0;
                inNumber = false;
            }
            strcat(dest, " ");
            src++;
            continue;
        }
        if (*src == '(') {
            if (inNumber && i > 0) {
                numBuffer[i] = '\0';
                if (!isValidateNumber(numBuffer, base)) {
                    free(dest);
                    return false;
                }
                char decStr[50];
                snprintf(decStr, sizeof(decStr), "%.10f", toDecimale(numBuffer, base));
                char *dot = strchr(decStr, '.');
                if (dot) {
                    char *end = decStr + strlen(decStr) - 1;
                    while (end > dot && *end == '0') *end-- = '\0';
                    if (*end == '.') *end = '\0';
                }
                strcat(dest, decStr);
                i = 0;
                inNumber = false;
            }
            bracket_count++;
            strcat(dest, "(");
            src++;
        }
        else if (*src == ')') {
            if (inNumber && i > 0) {
                numBuffer[i] = '\0';
                if (!isValidateNumber(numBuffer, base)) {
                    free(dest);
                    return false;
                }
                char decStr[50];
                snprintf(decStr, sizeof(decStr), "%.10f", toDecimale(numBuffer, base));
                char *dot = strchr(decStr, '.');
                if (dot) {
                    char *end = decStr + strlen(decStr) - 1;
                    while (end > dot && *end == '0') *end-- = '\0';
                    if (*end == '.') *end = '\0';
                }
                strcat(dest, decStr);
                i = 0;
                inNumber = false;
            }
            if (bracket_count <= 0) {
                free(dest);
                return false;
            }
            bracket_count--;
            strcat(dest, ")");
            src++;
        }
        else if (strchr("+-*/", *src)) {
            if (inNumber && i > 0) {
                numBuffer[i] = '\0';
                if (!isValidateNumber(numBuffer, base)) {
                    free(dest);
                    return false;
                }
                char decStr[50];
                snprintf(decStr, sizeof(decStr), "%.10f", toDecimale(numBuffer, base));
                char *dot = strchr(decStr, '.');
                if (dot) {
                    char *end = decStr + strlen(decStr) - 1;
                    while (end > dot && *end == '0') *end-- = '\0';
                    if (*end == '.') *end = '\0';
                }
                strcat(dest, decStr);
                i = 0;
                inNumber = false;
            }
            char op[2] = {*src, '\0'};
            strcat(dest, " ");
            strcat(dest, op);
            src++;
        }
        else if (isalnum(*src) || *src == '.' || (*src == '-' && (src == *expr || *(src-1) == '(' || isspace(*(src-1))))) {
            inNumber = true;
            numBuffer[i++] = tolower(*src++);
            if (i >= 99) {
                free(dest);
                return false;
            }
        }
        else {
            free(dest);
            return false;
        }
    }

    if (inNumber && i > 0) {
        numBuffer[i] = '\0';
        if (!isValidateNumber(numBuffer, base)) {
            free(dest);
            return false;
        }
        char decStr[50];
        snprintf(decStr, sizeof(decStr), "%.10f", toDecimale(numBuffer, base));
        char *dot = strchr(decStr, '.');
        if (dot) {
            char *end = decStr + strlen(decStr) - 1;
            while (end > dot && *end == '0') *end-- = '\0';
            if (*end == '.') *end = '\0';
        }
        strcat(dest, decStr);
    }

    if (bracket_count != 0) {
        free(dest);
        return false;
    }

    free(*expr);
    *expr = dest;
    return true;
}