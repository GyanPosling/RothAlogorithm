#ifndef ROTAOPERATION_H
#define ROTAOPERATION_H

#include "setsImplementation.h"

char starOperation(char x1, char x2);
char perOperation(char x1, char x2);
char sharpOperation(char x1, char x2);

Implicant* star(Implicant *implicant1, Implicant *implicant2);
Implicant* unionOp(Implicant *implicant1, Implicant *implicant2);
Implicant** sharp(Implicant* implicant1, Implicant* implicant2);


#endif //ROTAOPERATION_H
