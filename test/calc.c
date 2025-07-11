#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

typedef struct Stack {
    double data;
    struct Stack *next;
} Stack;

char* initString() {
    char* str = NULL;
    char ch;
    int length = 0;
    int capacity = 10;

    str = malloc(sizeof(char) * capacity);
    if (str == NULL) exit(1);

    while ((ch = getchar()) != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            str = realloc(str, capacity * sizeof(char));
            if (str == NULL) exit(1);
        }
        str[length++] = (ch == ',' ? '.' : ch);
    }

    str[length] = '\0';
    return str;
}

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

int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

double evaluateRPN(char *rpn) {
    Stack *stack = NULL;
    char *token = strtok(rpn, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&stack, atof(token));
        } else if (isOperator(token[0])) {
            double b = pop(&stack);
            double a = pop(&stack);
            if (isnan(a) || isnan(b)) return NAN;
            switch (token[0]) {
                case '+': push(&stack, a + b); break;
                case '-': push(&stack, a - b); break;
                case '*': push(&stack, a * b); break;
                case '/': 
                    if (b == 0) return NAN;
                    push(&stack, a / b); 
                    break;
            }
        }
        token = strtok(NULL, " ");
    }
    double result = pop(&stack);
    if (!isEmpty(stack)) return NAN;
    return result;
}

char* shuntingYard(const char *expr) {
    Stack *opStack = NULL;
    char *output = malloc(strlen(expr) * 2);
    output[0] = '\0';
    char buffer[50];
    int pos = 0;

    while (*expr) {
        if (isspace(*expr)) {
            expr++;
            continue;
        }

        if ((isalnum(*expr) || *expr == '.' || (*expr == '-' && (pos == 0 || output[pos-1] == '(')))) {
            int i = 0;
            while ((isalnum(*expr) || *expr == '.' || (*expr == '-' && (i == 0 || *(expr-1) == '(')))) {
                buffer[i++] = tolower(*expr++);
            }
            buffer[i] = '\0';
            pos += sprintf(output + pos, "%s ", buffer);
            continue;
        }

        if (*expr == '(') {
            push(&opStack, '(');
            expr++;
        } else if (*expr == ')') {
            while (!isEmpty(opStack) && (char)opStack->data != '(') {
                pos += sprintf(output + pos, "%c ", (char)pop(&opStack));
            }
            if (isEmpty(opStack)) return NULL;
            pop(&opStack);
            expr++;
        } else if (isOperator(*expr)) {
            while (!isEmpty(opStack) && getPriority((char)opStack->data) >= getPriority(*expr)) {
                pos += sprintf(output + pos, "%c ", (char)pop(&opStack));
            }
            push(&opStack, *expr);
            expr++;
        } else {
            return NULL;
        }
    }

    while (!isEmpty(opStack)) {
        if ((char)opStack->data == '(') return NULL;
        pos += sprintf(output + pos, "%c ", (char)pop(&opStack));
    }

    return output;
}

bool isValidNumber(const char *number, int base) {
    if (base < 2 || base > 16) return false;
    bool hasDot = false;
    for (; *number; number++) {
        if (*number == '.' || *number == ',') {
            if (hasDot) return false;
            hasDot = true;
            continue;
        }
        char c = tolower(*number);
        int digit = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : -1;
        if (digit < 0 || digit >= base) return false;
    }
    return true;
}

double toDecimal(const char *number, int base) {
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

char* fromDecimal(double num, int base) {
    if (base < 2 || base > 16) return NULL;
    long intPart = (long)num;
    double fracPart = fabs(num - intPart);

    char intBuffer[65];
    int i = 0;

    if (intPart == 0) {
        intBuffer[i++] = '0';
    } else {
        while (intPart > 0) {
            int digit = intPart % base;
            if (digit >= 10) digit += 'a' - 10;
            intBuffer[i++] = (digit < 10) ? digit + '0' : digit;
            intPart /= base;
        }
    }

    for (int j = 0; j < i/2; j++) {
        char temp = intBuffer[j];
        intBuffer[j] = intBuffer[i-j-1];
        intBuffer[i-j-1] = temp;
    }
    intBuffer[i] = '\0';

    char *result = malloc(128);
    strcpy(result, intBuffer);

    if (fracPart > 0) {
        strcat(result, ".");
        for (int j = 0; j < 10; j++) {
            fracPart *= base;
            int digit = (int)fracPart;
            if (digit >= 10) digit += 'a' - 10;
            result[strlen(result)] = (digit < 10) ? digit + '0' : digit;
            fracPart -= digit;
            if (fracPart == 0) break;
        }
    }
    return result;
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

char* getExpression() {
    printf("Enter expression: ");
    return initString();
}

bool parseAndReplaceNumbers(char **expr, int base) {
    char *src = *expr;
    char *dest = malloc(strlen(src) * 2);
    dest[0] = '\0';

    while (*src) {
        if (isspace(*src)) {
            strcat(dest, " ");
            src++;
            continue;
        }

        if ((isalnum(*src) || *src == '.' || *src == ',' || (*src == '-' && (src == *expr || *(src-1) == '(')))) {
            char numBuffer[100];
            int i = 0;
            while ((isalnum(*src) || *src == '.' || *src == ',' || (*src == '-' && (i == 0 || *(src-1) == '(')))) {
                numBuffer[i++] = (*src == ',' ? '.' : tolower(*src++));
            }
            numBuffer[i] = '\0';

            if (!isValidNumber(numBuffer, base)) {
                free(dest);
                return false;
            }

            double dec = toDecimal(numBuffer, base);
            char decStr[50];
            sprintf(decStr, "%.10f", dec);
            strcat(dest, decStr);
        } else if (strchr("+-*/()", *src)) {
            char op[2] = {*src++, '\0'};
            strcat(dest, op);
        } else {
            free(dest);
            return false;
        }
    }

    free(*expr);
    *expr = dest;
    return true;
}

int main() {
    printf("Enter base in which u want to perform operation(2, 16): \n");
    int base = getValidBase(2, 16);
    char *expr = getExpression();

    if (!parseAndReplaceNumbers(&expr, base)) {
        printf("Invalid expression\n");
        free(expr);
        return 1;
    }

    char *rpn = shuntingYard(expr);
    if (!rpn) {
        printf("Invalid expression\n");
        free(expr);
        return 1;
    }

    double result = evaluateRPN(rpn);
    if (isnan(result)) {
        printf("Invalid expression\n");
    } else {
        char *converted = fromDecimal(result, base);
        printf("Result: %s\n", converted);
        free(converted);
    }

    free(expr);
    free(rpn);
    return 0;
}