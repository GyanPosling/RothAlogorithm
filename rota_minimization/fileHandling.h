#pragma once

#include "setsImplementation.h"

Set parseLFromFile(const char* filename);
Set parseNFromFile(const char* filename);
Set parseSetFromFile(const char* filename, const char* setName);

void writeLToFile(Set* L, const char* filename);
void writeNToFile(Set* N, const char* filename);
void writeSetToFile(Set* set, const char* setName, const char* filename, const char* mode);