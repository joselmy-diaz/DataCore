#ifndef INTERFACE_H
#define INTERFACE_H

#include "obj.h"
// #include "stuctura.h"


typedef struct Entry {
    Obj* data;
    char *key;
    struct Entry *next;
} Entry;

Obj* newObj(ObjType type, void* as);
Obj* newObjString(char * str);
Entry newEntry(const char* key, Obj* data);

bool assignData(Obj** obj, Obj* data);
void printData(Obj* data);

bool insertD(Obj* obj, Entry data);
Obj* searchD(Obj* obj, const char *key);
Obj* searchDIndex (Obj* obj, int index);
bool isObjD(Obj* obj);
bool freeObjs(Obj* obj);

#endif // LISTA_H
