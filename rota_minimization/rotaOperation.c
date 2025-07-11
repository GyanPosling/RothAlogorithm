#include "rotaOperation.h"

char starOperation(char x1, char x2) {
    if ((x1 != 'x' && x2 != 'x') && (x1 != x2)) {
        return 'y';
    } else if (x1 == x2) {
        return x1;
    }
    return x1 == 'x' ? x2 : x1;
}

char perOperation(char x1, char x2) {
    if ((x1 != 'x' && x2 != 'x') && (x1 != x2)) {
        return 'o';
    }
    return x1 == 'x' ? x2 : x1;
}

char sharpOperation(char x1, char x2) {
    if ((x2 == 'x') || (x1 == x2)) {
        return 'z';
    } else if ((x1 != 'x' && x2 != 'x') && (x1 != x2)) {
        return 'y';
    }
    return x1 == 'x' ? (x2 == '0' ? '1' : '0') : '0';
}

Implicant* star(Implicant *implicant1, Implicant *implicant2) {
    char* result = malloc(implicant1->length + 1);
    int countY = 0;

    for (int i = 0; i < implicant1->length; i++) {
        result[i] = starOperation(implicant1->value[i], implicant2->value[i]);
        if (result[i] == 'y') countY++;
    }

    result[implicant1->length] = '\0';

    bool isValid = (countY == 1);
    if (isValid) {
        for (int i = 0; i < implicant1->length; i++) {
            if (result[i] == 'y') result[i] = 'x';
        }
    } else {
        strcpy(result, "null");
    }

    Implicant* resultImplicant = malloc(sizeof(Implicant));
    *resultImplicant = createImplicant(result);
    free(result);
    return resultImplicant;
}

Implicant* unionOp(Implicant *implicant1, Implicant *implicant2) {
    char* result = malloc(implicant1->length + 1);
    bool isValid = true;

    for(int i = 0; i < implicant1->length; i++) {
        result[i] = perOperation(implicant1->value[i], implicant2->value[i]);
        if(result[i] == 'o') {
            isValid = false;
            break;
        }
    }
    result[implicant1->length] = '\0';

    if(!isValid) {
        strcpy(result, "null");
    }

    Implicant* resultImplicant = malloc(sizeof(Implicant));
    *resultImplicant = createImplicant(result);
    free(result);
    return resultImplicant;
}

Implicant** sharp(Implicant* implicant1, Implicant* implicant2) {
    char* tempResult = (char*)malloc((implicant1->length + 1) * sizeof(char));
    int countZ = 0;
    bool hasY = false;

    for (int i = 0; i < implicant1->length; i++) {
        tempResult[i] = sharpOperation(implicant1->value[i], implicant2->value[i]);
        if (tempResult[i] == 'z') countZ++;
        else if (tempResult[i] == 'y') hasY = true;
    }
    tempResult[implicant1->length] = '\0';

    if (hasY) {
        Implicant** result = (Implicant**)malloc(2 * sizeof(Implicant*));
        result[0] = implicantCopy(implicant1);
        result[1] = NULL;
        free(tempResult);
        return result;
    }
    else if (countZ == implicant1->length) {
        Implicant** result = (Implicant**)malloc(2 * sizeof(Implicant*));
        Implicant nullImp = createImplicant("null");
        result[0] = implicantCopy(&nullImp);
        freeImplicant(&nullImp);
        result[1] = NULL;
        free(tempResult);
        return result;
    }
    else {
        int digitCount = 0;
        for (int i = 0; i < implicant1->length; i++) {
            if (tempResult[i] == '0' || tempResult[i] == '1') digitCount++;
        }

        Implicant** result = (Implicant**)malloc((digitCount + 1) * sizeof(Implicant*));
        int resultIndex = 0;

        for (int i = 0; i < implicant1->length; i++) {
            if (tempResult[i] == '0' || tempResult[i] == '1') {
                char* newStr = (char*)malloc((implicant1->length + 1) * sizeof(char));
                strcpy(newStr, implicant1->value);
                newStr[i] = tempResult[i];
                Implicant newImp = createImplicant(newStr);
                result[resultIndex] = implicantCopy(&newImp);
                freeImplicant(&newImp);
                free(newStr);
                resultIndex++;
            }
        }
        result[resultIndex] = NULL;
        free(tempResult);
        return result;
    }
}
