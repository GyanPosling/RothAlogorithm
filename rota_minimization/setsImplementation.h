#ifndef SETSIMPLEMENTATION_H
#define SETSIMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* value;
    int length;
    int xCount;
    bool isUsed;
} Implicant;

typedef struct {
    Implicant* implicants;
    int size;
    int capacity;
} Set;

typedef struct {
    Set* sets;
    int size;
} SetsArray;

Implicant createImplicant(char* value);
int getLength(Implicant* implicant);
int getCountX(Implicant *implicant);
void freeImplicant(Implicant* implicant);
bool implicantsAreEqual(Implicant* first, Implicant* second);
Implicant copyImplicant(Implicant* source);
Implicant inputImplicant();

Implicant* implicantCopy(const Implicant* original);
void initializeSet(Set* set, int initialCapacity);
Set inputSet();
int getSetSize(Set* set);
void freeSet(Set* set);
bool setContainsImplicant(Set* set, Implicant* implicant);
bool isSetEmpty(Set *set);
bool isSetsEqual(Set* set1, Set* set2);
void addToSet(Set* set, Implicant* implicant);
void removeFromSet(Set* set, Implicant* implicant);
Set copySet(const Set* original);
void printSet(Set* set);

Set unionOfSets(Set* firstSet, Set* secondSet);
Set intersectionOfSets(Set* firstSet, Set* secondSet);
Set differenceOfSets(Set* firstSet, Set* secondSet);
bool isSubsetOf(Set* subset, Set* superset);
bool isSupersetOf(Set* superset, Set* subset);

SetsArray initSetsArray();
void freeSetsArray(SetsArray* arr);
void addToSetsArray(SetsArray* arr, const Set* set);






#endif //SETSIMPLEMENTATION_H
