#include "rpnImplementation.h"

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
    char *rpn_copy = strdup(rpn);
    char *token = strtok(rpn_copy, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&stack, atof(token));
        }
        else if (isOperator(token[0])) {
            double b = pop(&stack);
            double a = pop(&stack);
            if (isnan(a) || isnan(b)) {
                free(rpn_copy);
                return NAN;
            }
            switch (token[0]) {
                case '+': push(&stack, a + b); break;
                case '-': push(&stack, a - b); break;
                case '*': push(&stack, a * b); break;
                case '/':
                    if (b == 0) {
                        free(rpn_copy);
                        return NAN;
                    }
                    push(&stack, a / b);
                    break;
            }
        }
        token = strtok(NULL, " ");
    }
    free(rpn_copy);
    double result = pop(&stack);
    if (!isEmpty(stack)) return NAN;
    return result;
}

char* shuntingYard(const char *expr) {
    Stack *opStack = NULL;
    char *output = malloc(strlen(expr) * 20 + 1);
    if (!output) return NULL;
    output[0] = '\0';
    char buffer[100];
    int pos = 0;
    const char *current_expr = expr;

    while (*current_expr) {
        if (isspace(*current_expr)) {
            current_expr++;
            continue;
        }

        if (isdigit(*current_expr) || (*current_expr == '-' && isdigit(*(current_expr + 1))) || *current_expr == '.') {
            int i = 0;
            while (isdigit(*current_expr) || *current_expr == '.' || (*current_expr == '-' && i == 0)) {
                buffer[i++] = *current_expr++;
                if (i >= sizeof(buffer) - 1) break;
            }
            buffer[i] = '\0';
            pos += sprintf(output + pos, "%s ", buffer);
            continue;
        }

        if (*current_expr == '(') {
            push(&opStack, (double)'(');
            current_expr++;
        }
        else if (*current_expr == ')') {
            while (!isEmpty(opStack) && (char)opStack->data != '(') {
                pos += sprintf(output + pos, "%c ", (char)pop(&opStack));
            }
            if (isEmpty(opStack)) {
                free(output);
                while(!isEmpty(opStack)) pop(&opStack);
                return NULL;
            }
            pop(&opStack);
            current_expr++;
        }
        else if (isOperator(*current_expr)) {
            while (!isEmpty(opStack) && getPriority((char)opStack->data) >= getPriority(*current_expr)) {
                pos += sprintf(output + pos, "%c ", (char)pop(&opStack));
            }
            push(&opStack, (double)*current_expr);
            current_expr++;
        }
        else {
            free(output);
            while(!isEmpty(opStack)) pop(&opStack);
            return NULL;
        }
    }

    while (!isEmpty(opStack)) {
        if ((char)opStack->data == '(') {
            free(output);
            while(!isEmpty(opStack)) pop(&opStack);
            return NULL;
        }
        pos += sprintf(output + pos, "%c ", (char)pop(&opStack));
    }

    if (pos > 0 && output[pos-1] == ' ') {
        output[pos-1] = '\0';
    } else {
        output[pos] = '\0';
    }

    return output;
}