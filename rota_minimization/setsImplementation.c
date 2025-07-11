#include "setsImplementation.h"

Implicant createImplicant(char* value) {
    Implicant newImplicant;
    newImplicant.length = strlen(value);
    newImplicant.value = (char*)malloc((newImplicant.length + 1) * sizeof(char));
    strcpy(newImplicant.value, value);

    newImplicant.xCount = 0;
    for (int i = 0; i < newImplicant.length; i++) {
        if (value[i] == 'x') {
            newImplicant.xCount++;
        }
    }

    newImplicant.isUsed = false;

    return newImplicant;
}

int getLength(Implicant* implicant) {
    return implicant->length;
}

int getCountX(Implicant *implicant){
    return implicant->xCount;
}

void freeImplicant(Implicant* implicant) {
    free(implicant->value);
}

bool implicantsAreEqual(Implicant* first, Implicant* second) {
    if (first->length != second->length) return false;
    return strcmp(first->value, second->value) == 0;
}

Implicant copyImplicant(Implicant* source) {
    Implicant copy;
    copy.value = strdup(source->value);
    copy.length = source->length;
    copy.xCount = source->xCount;
    copy.isUsed = source->isUsed;
    return copy;
}

Implicant inputImplicant() {
    char input[100];
    bool isValid = false;

    while (!isValid) {
        printf("Enter implicant (0/1/x): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf("Error: Implicant cannot be empty\n");
            continue;
        }

        isValid = true;
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] != '0' && input[i] != '1' && input[i] != 'x') {
                printf("Invalid character: '%c'. Only 0, 1, x allowed\n", input[i]);
                isValid = false;
                break;
            }
        }
    }

    return createImplicant(input);
}

Implicant* implicantCopy(const Implicant* original) {
    if (!original) return NULL;

    Implicant* copy = (Implicant*)malloc(sizeof(Implicant));
    if (!copy) {
        fprintf(stderr, "Memory allocation failed in copyImplicant.\n");
        exit(1);
    }

    copy->value = (char*)malloc(strlen(original->value) + 1);
    if (!copy->value) {
        fprintf(stderr, "Memory allocation failed for value in copyImplicant.\n");
        free(copy);
        exit(1);
    }
    strcpy(copy->value, original->value);
    copy->length = original->length;
    copy->xCount = original->xCount;
    copy->isUsed = original->isUsed;

    return copy;
}

void initializeSet(Set* set, int initialCapacity) {
    set->implicants = (Implicant*)malloc(initialCapacity * sizeof(Implicant));
    set->size = 0;
    set->capacity = initialCapacity;
}

void freeSet(Set* set) {
    for (int index = 0; index < set->size; index++) {
        freeImplicant(&set->implicants[index]);
    }
    free(set->implicants);
    set->size = 0;
    set->capacity = 0;
}

bool setContainsImplicant(Set* set, Implicant* implicant) {
    for (int index = 0; index < set->size; index++) {
        if (implicantsAreEqual(&set->implicants[index], implicant)) {
            return true;
        }
    }
    return false;
}

bool isSetEmpty(Set *set){
    return set->size == 0;
}

bool isSetsEqual(Set* set1, Set* set2) {
    if (set1->size != set2->size) return false;
    for (int i = 0; i < set1->size; i++) {
        if (!setContainsImplicant(set2, &set1->implicants[i])) {
            return false;
        }
    }
    return true;
}

void addToSet(Set* set, Implicant* implicant) {
    if (setContainsImplicant(set, implicant)) return;

    if (set->size >= set->capacity) {
        set->capacity = set->capacity == 0 ? 1 : set->capacity * 2;
        set->implicants = (Implicant*)realloc(set->implicants, set->capacity * sizeof(Implicant));
    }

    set->implicants[set->size] = copyImplicant(implicant);
    set->size++;
}

void removeFromSet(Set* set, Implicant* implicant) {
    for (int index = 0; index < set->size; index++) {
        if (implicantsAreEqual(&set->implicants[index], implicant)) {
            freeImplicant(&set->implicants[index]);
            for (int position = index; position < set->size - 1; position++) {
                set->implicants[position] = set->implicants[position + 1];
            }
            set->size--;
            return;
        }
    }
}

Set unionOfSets(Set* firstSet, Set* secondSet) {
    Set result;
    initializeSet(&result, firstSet->size + secondSet->size);

    for (int index = 0; index < firstSet->size; index++) {
        addToSet(&result, &firstSet->implicants[index]);
    }

    for (int index = 0; index < secondSet->size; index++) {
        addToSet(&result, &secondSet->implicants[index]);
    }

    return result;
}

Set intersectionOfSets(Set* firstSet, Set* secondSet) {
    Set result;
    initializeSet(&result, firstSet->size < secondSet->size ? firstSet->size : secondSet->size);

    for (int index = 0; index < firstSet->size; index++) {
        if (setContainsImplicant(secondSet, &firstSet->implicants[index])) {
            addToSet(&result, &firstSet->implicants[index]);
        }
    }

    return result;
}

Set differenceOfSets(Set* firstSet, Set* secondSet) {
    Set result;
    initializeSet(&result, firstSet->size);

    for (int index = 0; index < firstSet->size; index++) {
        if (!setContainsImplicant(secondSet, &firstSet->implicants[index])) {
            addToSet(&result, &firstSet->implicants[index]);
        }
    }

    return result;
}

int getSetSize(Set* set) {
    return set->size;
}

bool isSubsetOf(Set* subset, Set* superset) {
    for (int index = 0; index < subset->size; index++) {
        if (!setContainsImplicant(superset, &subset->implicants[index])) {
            return false;
        }
    }
    return true;
}

bool isSupersetOf(Set* superset, Set* subset) {
    return isSubsetOf(subset, superset);
}

Set copySet(const Set* original) {
    Set copy;
    initializeSet(&copy, original->size);
    for (int i = 0; i < original->size; i++) {
        addToSet(&copy, &original->implicants[i]);
    }
    return copy;
}

void printSet(Set* set) {
    if(set->size == 0){printf("{ }\n"); return;}
    printf("{ ");
    for (int index = 0; index < set->size; index++) {
        printf("%s", set->implicants[index].value);
        if (index < set->size - 1) printf(", ");
    }
    printf(" }\n");
}

Set inputSet() {
    Set userSet;
    initializeSet(&userSet, 1);

    char input[100];
    int expectedLength = -1;

    while (1) {
        printf("Implicant %d (or empty to finish): ", userSet.size + 1);

        if (fgets(input, sizeof(input), stdin) == NULL) break;

        int len = strlen(input);
        if (input[len-1] == '\n') input[--len] = '\0';

        int start = 0, end = len - 1;
        while (start <= end && input[start] == ' ') start++;
        while (end >= start && input[end] == ' ') end--;
        int newLen = end - start + 1;

        if (newLen <= 0) break;

        char processedInput[100];
        strncpy(processedInput, input + start, newLen);
        processedInput[newLen] = '\0';

        int hasError = 0;
        for (int i = 0; processedInput[i]; i++) {
            if (processedInput[i] == ' ') hasError = 1;
            if (processedInput[i] != '0' && processedInput[i] != '1' && processedInput[i] != 'x') hasError = 1;
        }

        if (hasError) {
            printf("Error! Invalid characters or spaces inside. Try again.\n");
            continue;
        }

        if (expectedLength == -1) {
            expectedLength = newLen;
        } else if (newLen != expectedLength) {
            printf("Error! All implicants must have length %d. Try again.\n", expectedLength);
            continue;
        }

        Implicant newImp = createImplicant(processedInput);
        addToSet(&userSet, &newImp);
        freeImplicant(&newImp);
    }

    return userSet;
}


SetsArray initSetsArray() {
    SetsArray arr;
    arr.sets = NULL;
    arr.size = 0;
    return arr;
}

void freeSetsArray(SetsArray* arr) {
    for (int i = 0; i < arr->size; i++) freeSet(&arr->sets[i]);
    free(arr->sets);
    arr->size = 0;
}

void addToSetsArray(SetsArray* arr, const Set* set) {
    arr->sets = realloc(arr->sets, (arr->size + 1) * sizeof(Set));
    arr->sets[arr->size] = copySet(set);
    arr->size++;
}