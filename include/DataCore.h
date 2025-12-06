#ifndef INTERFACE_H
#define INTERFACE_H

#include "obj.h"
#include "structure.h"
#include "buffer.h"

enum {
    IS_BOOL = 1,
    IS_NATIVE =2,
    IS_OBJ = 3
};

typedef bool (*FreeFn)(Obj*);

#define MAX_TYPES 32
#define OBJ_TYPES_X \
    X(TYPE_ENTRY,      freeEntry) \
    X(TYPE_STRING,     freeString) \
    X(TYPE_BUFFER,     freeBuffer) \
    X(TYPE_ARRAY,      freeArray) \
    X(TYPE_AVL_TREE,   freeTR) \
    X(TYPE_HASH_TABLE, freeTH) 


extern FreeFn destructorTable[MAX_TYPES];


typedef struct {
    bool (*insert)(Obj*, Obj*);
    Obj* (*search)(Obj*, const char*);
} ObjOps;


void hash_foreach(Obj *table, void (*callback)(Entry*));

bool assignData(Obj** obj, Obj* data);
void printObjf(Obj* data);
void printObj (Obj* obj);

bool insertD(Obj* obj, Entry* data);
Obj* searchD(Obj* obj, const char *key);
Obj* searchDIndex (Obj* obj, int index);

bool freeObjs(Obj* obj);

void bloqueaObj (Obj* obj);
void desbloqueaObj (Obj* obj);

#endif // LISTA_H
