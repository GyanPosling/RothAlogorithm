#ifndef PERFORMROTA_H
#define PERFORMROTA_H

#include "rotaOperation.h"
#include "setsImplementation.h"

bool absorbs(Implicant* imp1, Implicant* imp2);
Set setAbsorption(Set *original);
void resetUsageFlags(Set *set);

Set getAiSetWithCiOnCi(Set *C, Set *A, int step);
Set getZi(Set *C, Set *A, int step);
Set getSimpleImplicants(Set *L, Set *N);

Set filterByL(Set *residuals, Set *L);
Set getCompulsoryImplicants(Set *Z, Set *L);
Set getResidualsSet(Set *Z);
int findZeroCount(Implicant *implicant);
Implicant* findTheCheapestImplicant(Set *set);

Set getL1(Set *L, Set *E);
Set getUncompolsorySimpleImplicant(Set *Z, Set *E);
bool coversL1(const Set* subset, const Set* L1);
SetsArray getAllMinForms(Set* L1, Set* Z1, Set* L_extremals);

void performRota();

#endif //PERFORMROTA_H
