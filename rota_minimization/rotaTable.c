#include"rotaTable.h"


void printCiStarCiTable(Set *ci) {
    printf("\nC%d*C%d table:\n", ci->implicants[0].xCount, ci->implicants[0].xCount);
    printf("%-8s|", "");
    for(int i=0; i<ci->size; i++) 
        printf(" %-6s |", ci->implicants[i].value);
    
    printf("\n--------|");
    for(int i=0; i<ci->size; i++) printf("--------|");
    printf("\n");

    for(int i=0; i<ci->size; i++) {
        printf("%-8s|", ci->implicants[i].value);
        for(int j=0; j<ci->size; j++) {
            if(j <= i) {
                printf(" %-6s |", (i==j) ? "-" : " ");
                continue;
            }
            
            Implicant* res = star(&ci->implicants[i], &ci->implicants[j]);
            printf(" %-6s |", (strcmp(res->value,"null")==0) ? "-" : res->value);
            freeImplicant(res);
            free(res);
        }
        printf("\n");
    }
}


int calculateMaxWidth(char* term, char* title) {
    int term_width = (int)strlen(term);
    int title_width = (int)strlen(title);

    if (term_width > title_width) return term_width;
    else return title_width; 
}


void printSubtractionTable(const char* nameOfTable, Set Z, Set Y) {
    int max_width = strlen(nameOfTable);
    for (int i = 0; i < Z.size; i++) {
        int len = strlen(Z.implicants[i].value);
        if (len > max_width) max_width = len;
    }
    for (int i = 0; i < Y.size; i++) {
        int len = strlen(Y.implicants[i].value);
        if (len > max_width) max_width = len;
    }
    int col_width = max_width + 2;

    printf("\n%-*s |", col_width, nameOfTable);
    for (int i = 0; i < Z.size; i++) 
        printf(" %-*s |", col_width, Z.implicants[i].value);
    printf("\n");

    printf("%-*s-|", col_width, "");
    for (int i = 0; i < Z.size; i++) 
        printf("%.*s|", col_width + 2, "--------------------------------");
    printf("\n");

    for (int y_idx = 0; y_idx < Y.size; y_idx++) {
        printf("%-*s |", col_width, Y.implicants[y_idx].value);
        
        Set current_residues;
        initializeSet(&current_residues, 1);
        addToSet(&current_residues, &Y.implicants[y_idx]);

        for (int z_idx = 0; z_idx < Z.size; z_idx++) {
            if (implicantsAreEqual(&Y.implicants[y_idx], &Z.implicants[z_idx])) {
                printf(" %-*s |", col_width, "-");
                continue;
            }

            Set new_residues;
            initializeSet(&new_residues, 1);

            for (int r_idx = 0; r_idx < current_residues.size; r_idx++) {
                Implicant** sharp_res = sharp(&current_residues.implicants[r_idx], &Z.implicants[z_idx]);
                
                for (int m = 0; sharp_res[m] != NULL; m++) {
                    if (strcmp(sharp_res[m]->value, "null") != 0) {
                        addToSet(&new_residues, sharp_res[m]);
                    }
                    freeImplicant(sharp_res[m]);
                    free(sharp_res[m]);
                }
                free(sharp_res);
            }

            char cell[1024] = "";
            if (new_residues.size > 0) {
                for (int i = 0; i < new_residues.size; i++) {
                    strcat(cell, new_residues.implicants[i].value);
                    if (i < new_residues.size - 1) strcat(cell, ",");
                }
            } else {
                strcpy(cell, "-");
            }

            printf(" %-*s |", col_width, cell);

            freeSet(&current_residues);
            current_residues = new_residues;
        }
        
        freeSet(&current_residues);
        printf("\n");
    }
}


void printUnionTables(const char *nameOfTable, Set *Z1, Set *L1) {
    printf("%s", nameOfTable);
    for (int i = 0; i < L1->size; i++) {
        printf(" %-6s |", L1->implicants[i].value);
    }
    printf("\n|---------|");
    for (int i = 0; i < L1->size; i++) printf("--------|");
    printf("\n");

    for (int i = 0; i < Z1->size; i++) {
        printf("| %-7s |", Z1->implicants[i].value);
        for (int j = 0; j < L1->size; j++) {
            bool isCovered = absorbs(&Z1->implicants[i], &L1->implicants[j]);
            printf(" %-6s |", isCovered ? unionOp(&Z1->implicants[i], &L1->implicants[j])->value : "-");
        }
        printf("\n");
    }
}
