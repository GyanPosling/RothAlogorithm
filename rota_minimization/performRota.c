#include "performRota.h"
#include"rotaTable.h"
#include "fileHandling.h"
#include "../validators.h"

bool absorbs(Implicant* imp1, Implicant* imp2) {
    if (imp1->length != imp2->length) return false;

    for (int i = 0; i < imp1->length; i++) {
        char c1 = imp1->value[i];
        char c2 = imp2->value[i];

        if (c1 != 'x' && c1 != c2) {
            return false;
        }
    }
    return true;
}

Set setAbsorption(Set *original) {
    Set result;
    initializeSet(&result, original->capacity);
    for (int i = 0; i < original->size; i++) {
        bool isAbsorbed = false;
        for (int j = 0; j < result.size; j++) {
            if (absorbs(&result.implicants[j], &original->implicants[i])) {
                isAbsorbed = true;
                break;
            }
        }
        if (!isAbsorbed) {
            for (int j = 0; j < result.size; j++) {
                if (absorbs(&original->implicants[i], &result.implicants[j])) {
                    removeFromSet(&result, &result.implicants[j]);
                    j--;
                }
            }
            addToSet(&result, &original->implicants[i]);
        }
    }
    return result;
}

void resetUsageFlags(Set *set) {
    for (int i = 0; i < set->size; i++) {
        set->implicants[i].isUsed = false;
    }
}

Set getAiSetWithCiOnCi(Set *C, Set *A, int step) {
    Set Ai;
    initializeSet(&Ai, 1);

    for (int i = 0; i < A->size; i++) {
        for (int j = 0; j < C->size; j++) {
            if (implicantsAreEqual(&A->implicants[i], &C->implicants[j])) continue;
            Implicant* tempImp = star(&A->implicants[i], &C->implicants[j]);

            if (strcmp(tempImp->value, "null") != 0 && tempImp->xCount > step) {
                Implicant* copy = implicantCopy(tempImp);
                addToSet(&Ai, copy);
                A->implicants[i].isUsed = true;
                C->implicants[j].isUsed = true;
                free(copy);
            }

            freeImplicant(tempImp);
            free(tempImp);
        }
    }

    return Ai;
}

Set getZi(Set *C, Set *A, int step) {
    Set Zi;
    initializeSet(&Zi, 1);

    for (int i = 0; i < C->size; i++) {
        Implicant* current = &C->implicants[i];
        if (!current->isUsed && current->xCount <= step) {
            addToSet(&Zi, current);
        }
    }

    for (int j = 0; j < A->size; j++) {
        Implicant* current = &A->implicants[j];
        if (!current->isUsed && current->xCount <= step) {
            addToSet(&Zi, current);
        }
    }

    return Zi;
}

Set getSimpleImplicants(Set *L, Set *N) {
    Set finalZ;
    initializeSet(&finalZ, 1);

    Set Ci = unionOfSets(L, N);
    printf("\nInitial C0 = L U N \n");
    printSet(&Ci);

    bool hasNewImplicants = true;
    int step = 0;

    while (hasNewImplicants) {
        printf("\n--- Iteration %d ---\n", step + 1);
        
        resetUsageFlags(&Ci);
        Set Ai = getAiSetWithCiOnCi(&Ci, &Ci, step);
        printCiStarCiTable(&Ci);
        printf("\nA%d: ", step + 1);
        printSet(&Ai);

        Set Zi = getZi(&Ci, &Ai, step);
        printf("Z%d: ", step + 1);
        printSet(&Zi);

        Set tempFinal = unionOfSets(&finalZ, &Zi);
        freeSet(&finalZ);
        finalZ = tempFinal;

        Set Bi = differenceOfSets(&Ci, &Zi);
        printf("B%d = C%d - Z%d = ", step + 1, step, step);
        printSet(&Bi);

        Set newCi = unionOfSets(&Ai, &Bi);
        newCi = setAbsorption(&newCi);
        printf("C%d = A%d U B%d = ", step + 1, step + 1, step + 1);
        printSet(&newCi);

        hasNewImplicants = (newCi.size > 0);

        freeSet(&Ai);
        freeSet(&Bi);
        freeSet(&Ci);

        Ci = newCi;
        step++;
    }
    
    freeSet(&Ci);
    return finalZ;
}

Set filterByL(Set *residuals, Set *L) {
    Set filtered;
    initializeSet(&filtered, residuals->size);
    for (int i = 0; i < residuals->size; i++) {
        Implicant *r = &residuals->implicants[i];
        bool hasIntersection = false;
        for (int j = 0; j < L->size; j++) {
            Implicant *l = &L->implicants[j];
            Implicant *intersection = unionOp(r, l);
            if (strcmp(intersection->value, "null") != 0) hasIntersection = true;
            freeImplicant(intersection);
            free(intersection);
            if (hasIntersection) break;
        }
        if (hasIntersection) addToSet(&filtered, r);
    }
    return filtered;
}

Set getCompulsoryImplicants(Set *Z, Set *L) {
    resetUsageFlags(Z);
    Set compulsory;
    initializeSet(&compulsory, Z->size);
    for (int i = 0; i < Z->size; i++) {
        Implicant z = Z->implicants[i];
        Set residuals;
        initializeSet(&residuals, 1);
        addToSet(&residuals, &z);
        for (int j = 0; j < Z->size; j++) {
            if (i == j) continue;
            Set new_residuals;
            initializeSet(&new_residuals, residuals.size * 2);
            for (int k = 0; k < residuals.size; k++) {
                Implicant current = residuals.implicants[k];
                Implicant** sharp_res = sharp(&current, &Z->implicants[j]);
                for (int m = 0; sharp_res[m] != NULL; m++) {
                    if (strcmp(sharp_res[m]->value, "null") != 0)
                        addToSet(&new_residuals, sharp_res[m]);
                    freeImplicant(sharp_res[m]);
                    free(sharp_res[m]);
                }
                free(sharp_res);
            }
            freeSet(&residuals);
            residuals = new_residuals;
            if (residuals.size == 0) break;
        }
        Set filtered = filterByL(&residuals, L);
        if (filtered.size > 0){
            addToSet(&compulsory, &z);
            z.isUsed = true;
        }
            
        freeSet(&residuals);
        freeSet(&filtered);
    }

    while(isSetEmpty(&compulsory)){
        printf("\nNo compulsory implicants. Using branching algorithm (find the chipest):\n");
        addToSet(&compulsory, findTheCheapestImplicant(Z));
        Set filtered = filterByL(&compulsory, L);
        if (filtered.size == 0) removeFromSet(&compulsory, findTheCheapestImplicant(Z));
        else findTheCheapestImplicant(Z)->isUsed = true;
    }
    return compulsory;
}

Set getResidualsSet(Set *Z) {
    Set allResiduals;
    initializeSet(&allResiduals, Z->size * 2); 

    for (int i = 0; i < Z->size; i++) {
        Implicant z = Z->implicants[i];
        Set residuals;
        initializeSet(&residuals, 1);
        addToSet(&residuals, &z); 

        for (int j = 0; j < Z->size; j++) {
            if (i == j) continue; 

            Set new_residuals;
            initializeSet(&new_residuals, residuals.size * 2); 
            for (int k = 0; k < residuals.size; k++) {
                Implicant current = residuals.implicants[k];
                Implicant** sharp_res = sharp(&current, &Z->implicants[j]);

                for (int m = 0; sharp_res[m] != NULL; m++) {
                    if (strcmp(sharp_res[m]->value, "null") != 0) {
                        addToSet(&new_residuals, sharp_res[m]);
                    }
                    freeImplicant(sharp_res[m]); 
                    free(sharp_res[m]);
                }
                free(sharp_res);
            }

            freeSet(&residuals);
            residuals = new_residuals;

            if (residuals.size == 0) break; 
        }

        for (int k = 0; k < residuals.size; k++) {
            addToSet(&allResiduals, &residuals.implicants[k]);
        }

        freeSet(&residuals); 
    }

    return allResiduals;
}


int findZeroCount(Implicant *implicant){
    int zeroCount = 0;
    for (int i = 0; i < implicant->length; i++){
        if(implicant->value[i] == '0')
            zeroCount++;
    }
    return zeroCount;
}

Implicant* findTheCheapestImplicant(Set *set) {
    if (set->size == 0) return NULL;

    Implicant* cheapest = &set->implicants[0];
    for (int i = 1; i < set->size; i++) {
        if (cheapest->isUsed == false) continue;
        Implicant* current = &set->implicants[i];
        if (current->xCount > cheapest->xCount) {
            cheapest = current;
        } else if (current->xCount == cheapest->xCount) {
            int zerosCurrent = findZeroCount(current);
            int zerosCheapest = findZeroCount(cheapest);
            if (zerosCurrent < zerosCheapest) {
                cheapest = current;
            }
        }
    }
    return implicantCopy(cheapest);
}

Set getL1(Set *L, Set *E) {
    Set L1;
    initializeSet(&L1, L->size);

    for (int i = 0; i < L->size; i++) {
        Implicant current = L->implicants[i];
        Set residuals;
        initializeSet(&residuals, 1);
        addToSet(&residuals, &current);

        for (int j = 0; j < E->size; j++) {
            Set new_residuals;
            initializeSet(&new_residuals, residuals.size * 2);

            for (int k = 0; k < residuals.size; k++) {
                Implicant residual = residuals.implicants[k];
                Implicant** sharp_result = sharp(&residual, &E->implicants[j]);

                for (int m = 0; sharp_result[m] != NULL; m++) {
                    if (strcmp(sharp_result[m]->value, "null") != 0) {
                        addToSet(&new_residuals, sharp_result[m]);
                    }
                    freeImplicant(sharp_result[m]);
                    free(sharp_result[m]);
                }
                free(sharp_result);
            }

            freeSet(&residuals);
            residuals = new_residuals;

            if (residuals.size == 0) break;
        }

        if (residuals.size > 0) {
            addToSet(&L1, &current);
        }

        freeSet(&residuals);
    }

    return L1;
}

Set getUncompolsorySimpleImplicant(Set *Z, Set *E) {
    return differenceOfSets(Z, E);
}

bool coversL1(const Set* subset, const Set* L1) {
    for (int i = 0; i < L1->size; i++) {
        bool covered = false;
        for (int j = 0; j < subset->size; j++) {
            if (absorbs(&subset->implicants[j], &L1->implicants[i])) {
                covered = true;
                break;
            }
        }
        if (!covered) return false;
    }
    return true;
}

SetsArray getAllMinForms(Set* L1, Set* Z1, Set* L_extremals) {
    SetsArray result = initSetsArray();
    SetsArray allCovers = initSetsArray();

    int maxSubsets = 1 << Z1->size;
    for (int mask = 1; mask < maxSubsets; mask++) {
        Set currentSubset;
        initializeSet(&currentSubset, Z1->size);
        for (int i = 0; i < Z1->size; i++) {
            if (mask & (1 << i)) addToSet(&currentSubset, &Z1->implicants[i]);
        }
        if (coversL1(&currentSubset, L1)) addToSetsArray(&allCovers, &currentSubset);
        freeSet(&currentSubset);
    }

    int minSize = INT_MAX;
    for (int i = 0; i < allCovers.size; i++) {
        if (allCovers.sets[i].size < minSize) minSize = allCovers.sets[i].size;
    }

    for (int i = 0; i < allCovers.size; i++) {
        if (allCovers.sets[i].size == minSize) {
            Set combined = unionOfSets(L_extremals, &allCovers.sets[i]);
            combined = setAbsorption(&combined);
            addToSetsArray(&result, &combined);
            freeSet(&combined);
        }
    }

    freeSetsArray(&allCovers);
    return result;
}

void printRotaMenu() {
    printf("\n");
    printf("+-----------------------------------+\n");
    printf("|          INPUT METHOD MENU        |\n");
    printf("+-----------------------------------+\n");
    printf("|                                   |\n");
    printf("|   1. Console input                |\n");
    printf("|   2. Load from preset file        |\n");
    printf("|   3. Load from file & save to new |\n");
    printf("|   4. Exit.                        |\n");
    printf("|                                   |\n");
    printf("+-----------------------------------+\n");
    printf("\nEnter your choice (1-4): ");
}

void performRota() {
     printf("\n===================================== Performing Rota Minimization ================================\n");
    Set L, N;
    char filename[256];

    printRotaMenu();
    int choice = getValidInput(1, 4);

    switch (choice) {
        case 1:
            printf("\nEnter set L:\n");
            L = inputSet();
            printf("\nEnter set N:\n");
            N = inputSet();
            break;
        
        case 2:
            printf("Enter filename to load: ");
            scanf("%255s", filename);
            L = parseLFromFile(filename);
            N = parseNFromFile(filename);
            break;

        case 3:
            printf("\nEnter set L:\n");
            L = inputSet();
            printf("\nEnter set N:\n");
            N = inputSet();
            
            printf("Enter filename to save: ");
            scanf("%255s", filename);
            writeLToFile(&L, filename);
            writeNToFile(&N, filename);
            break;

        case 4:
            printf("\n=== Exiting from Rota Minimization!... ===\n");
            return;
    }

    printf("\n==================== Your input data (L and N sets) ====================\n");
    printf("Your L-set: \n");
    printSet(&L);
    printf("Your N-set: \n");
    printSet(&N);

    printf("\n==================== First step: search for prime implicants ====================\n");
    Set Z = getSimpleImplicants(&L, &N);
    printf("\nPrime Implicants (Z):\n");
    printSet(&Z);
    printf("\n==================== Second step: serach for L-extremals ========================\n");
    Set L_extremals = getCompulsoryImplicants(&Z, &L);
    Set remainSet = getResidualsSet(&Z);
    printf("\nTable for find L-extremals:\n");
    printSubtractionTable("z#(Z-z)", Z, Z);
    printf("\nTable for filter L_extermals by L-set\n");
    printUnionTables("|Z#(Z-z)UL|", &remainSet, &L);
    printf("\nL_extremals (E):\n");
    printSet(&L_extremals);

    printf("\n==================== Third step: search for all min forms ====================\n");
    Set L1 = getL1(&L, &L_extremals);
    printf("\nTable for finding implicants from L-set which dosen't cover by L-extremals:\n");
    printSubtractionTable("L#E", L_extremals, L);
    if (L1.size > 0) {
        Set Z1 = differenceOfSets(&Z, &L_extremals);
        printf("\nImplicants from L-set, witch doesn't cover by L-extremals (L1):\n");
        printSet(&L1);
        printf("\nRemaining prime implicants(without L-extremals) (Z1):\n");
        printSet(&Z1);

        SetsArray minForms = getAllMinForms(&L1, &Z1, &L_extremals);
        printf("\nTable for find all min forms\n\n");
        printUnionTables("| Z1 U L1 |", &Z1, &L1);
        printf("\n==================== ANSWER ====================\n");
        printf("All Minimal Forms (%d)\n", minForms.size);
        for (int i = 0; i < minForms.size; i++) {
            printf("Form %d: ", i + 1);
            printSet(&minForms.sets[i]);
        }
        freeSetsArray(&minForms);
        freeSet(&Z1);
    } else {
        printf("\n==================== ANSWER ====================\n");
        printf("All covered by L_extremals.\nMinimal form:\n");
        printSet(&L_extremals);
    }

    freeSet(&L);
    freeSet(&N);
    freeSet(&Z);
    freeSet(&L_extremals);
    freeSet(&L1);
}
