#ifndef INTERFACE_H
#define INTERFACE_H

#include "obj.h"
// #include "stuctura.h"

enum {
    IS_BOOL = 1,
    IS_NATIVE =2,
    IS_OBJ = 3
};

typedef struct Entry {
    ObjR obj;
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
char getTypeCategory(char res);
bool freeObjs(Obj* obj);

#endif // LISTA_H
