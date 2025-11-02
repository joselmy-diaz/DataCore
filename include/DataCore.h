#ifndef INTERFACE_H
#define INTERFACE_H

#include "obj.h"
#include "structure.h"

enum {
    IS_BOOL = 1,
    IS_NATIVE =2,
    IS_OBJ = 3
};

typedef struct {
    bool (*insert)(Obj*, Obj*);
    Obj* (*search)(Obj*, const char*);
    bool (*free)(Obj*);
} ObjOps;


extern ObjOps ObjOpsList[];


void hash_foreach(Obj *table, void (*callback)(Entry*));

bool assignData(Obj** obj, Obj* data);
void printObjf(Obj* data);
void printObj (Obj* obj);

bool insertD(Obj* obj, Entry* data);
Obj* searchD(Obj* obj, const char *key);
Obj* searchDIndex (Obj* obj, int index);
;
bool freeObjs(Obj* obj);

void bloqueaObj (Obj* obj);
void desbloqueaObj (Obj* obj);

#endif // LISTA_H
