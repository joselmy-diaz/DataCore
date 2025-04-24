#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "obj.h"
// #include "stuctura.h"


typedef struct Entry {
    Obj* data;
    char *key;
    struct Entry *next;
} Entry;

Obj* newData(ObjType type, As as);
Obj* newObjString(char * str);
Entry initEntry(const char* key, Obj* data);

bool assignData(Obj** obj, Obj* data);
void printData(Obj* data);

bool insertD(Obj* obj, Entry data);
Obj* searchD(Obj* obj, const char *key);
Obj* searchDIndex (Obj* obj, int index);
bool isObjD(Obj* obj);
bool freeObjs(Obj* obj);

#endif // LISTA_H
