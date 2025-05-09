#ifndef STUCTU_H
#define STUCTU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataCore.h"


typedef struct {
    ObjR obj;
    int length;
    Entry **entries;
} ObjTebleH;

// Estructura del nodo del árbol AVL
typedef struct NodeEntry {
    ObjR obj;
    char *key;
    Obj* data;
    struct NodeEntry* left;
    struct NodeEntry* right;
    int height;
} NodeEntry;


typedef struct {
    ObjR obj;
    struct NodeEntry* root;
} ObjTree;


typedef struct {
    ObjR obj;
    Obj** array;
    size_t length;
    int8_t free;
} ObjCon;


unsigned int hash(const char *key);

Obj* newObj(ObjType type, As* as);
Obj* newObjString(char * str);
Entry* newEntry(const char* key, Obj* data);
char* getString(Obj* obj);

void freeEn (Entry* entry);

Obj* initTH(int size);
Obj* searchTH(ObjTebleH *table, const char *key);
void insertTH(ObjTebleH *table, Entry data);
bool freeTH(ObjTebleH *table);

Obj *initTR();
NodeEntry* insertAVL(ObjTree* objT, Entry data);
NodeEntry* newNode(Entry data);
Obj* searchTree(ObjTree* obj, const char *key);
void preOrder(NodeEntry *root);
bool freeTR(ObjTree* objT);

Obj *initArray(int8_t size, bool isTree);
bool insertArray(ObjCon* objL, Obj* data);
int getziseL(ObjCon* objL);
bool insertLByKey(ObjCon* objL, Entry data);
Obj* searchArray(ObjCon *objL, int index);
Obj* searchLByKey(ObjCon* objL, const char* key);
bool freeArray(ObjCon* objL);

bool freeString(ObjString* obj);
bool freeEntry(Entry* entry);

#endif