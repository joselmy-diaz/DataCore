#ifndef INTERFACE_H
#define INTERFACE_H

#include "obj.h"
// #include "stuctura.h"


typedef struct Entry {
    char *key;
    Obj* data;
    struct Entry *next;
} Entry;


bool assignData(Obj** obj, Obj* data);
void printObjf(Obj* data);
void printObj (Obj* obj);

bool insertD(Obj* obj, Entry data);
Obj* searchD(Obj* obj, const char *key);
Obj* searchDIndex (Obj* obj, int index);
bool isObjD(Obj* obj);
bool freeObjs(Obj* obj);

#endif // LISTA_H
