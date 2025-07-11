#include "fileHandling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Set parseSetFromFile(const char* filename, const char* setName) {
    FILE* file = fopen(filename, "r");
    Set set;
    initializeSet(&set, 1);

    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return set;
    }

    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, setName)) {
            found = 1;
            char* start = strchr(line, '{');
            char* end = strchr(line, '}');
            
            if (!start || !end) {
                fprintf(stderr, "Invalid format in file: %s\n", filename);
                break;
            }

            *end = '\0';
            char* token = strtok(start + 1, ",");
            
            while (token) {
                while (*token == ' ') token++;
                char* end_token = token + strlen(token) - 1;
                while (end_token > token && *end_token == ' ') *end_token-- = '\0';

                if (*token != '\0') {
                    Implicant imp = createImplicant(token);
                    addToSet(&set, &imp);
                    freeImplicant(&imp);
                }
                token = strtok(NULL, ",");
            }
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Set %s not found in file %s\n", setName, filename);
    }

    fclose(file);
    return set;
}

Set parseLFromFile(const char* filename) {
    return parseSetFromFile(filename, "L =");
}

Set parseNFromFile(const char* filename) {
    return parseSetFromFile(filename, "N =");
}

void writeSetToFile(Set* set, const char* setName, const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    fprintf(file, "%s = { ", setName);
    for (int i = 0; i < set->size; i++) {
        fprintf(file, "%s", set->implicants[i].value);
        if (i < set->size - 1) fprintf(file, ", ");
    }
    fprintf(file, " }\n");

    fclose(file);
}

void writeLToFile(Set* L, const char* filename) {
    writeSetToFile(L, "L", filename, "w");
}

void writeNToFile(Set* N, const char* filename) {
    writeSetToFile(N, "N", filename, "a");
}