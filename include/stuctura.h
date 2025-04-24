#ifndef STUCTU_H
#define STUCTU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataCore.h"

typedef struct {
    Obj obj;
    int length;
    Entry **entries;
} ObjTebleH;

// Estructura del nodo del árbol AVL
typedef struct NodeEntry {
    char *key;
    Obj* data;
    struct NodeEntry* left;
    struct NodeEntry* right;
    int height;
} NodeEntry;


typedef struct {
    Obj obj;
    struct NodeEntry* root;
} ObjTree;


typedef struct {
    Obj obj;
    Obj** array;
    size_t length;
    int8_t free;
} ObjCon;


unsigned int hash(const char *key);

void freeEn (Entry* entry);

Obj* initTH(int size);
Obj* searchTH(ObjTebleH *table, const char *key);
void insertTH(ObjTebleH *table, Entry data);
bool freeTH(ObjTebleH *table);

Obj* initTR();
NodeEntry* insertAVL(NodeEntry* node, Entry data);
NodeEntry* newNode(Entry data);
Obj* searchTree(ObjTree* obj, const char *key);
void preOrder(NodeEntry *root);
bool freeTR(ObjTree* objT);

Obj* initList(int size, bool isTree);
bool insertL(ObjCon* objL, Obj* data);
int getziseL(ObjCon* objL);
bool insertLByKey(ObjCon* objL, Entry data);
Obj* searchL(ObjCon *objL, int index);
Obj* NewFunction(int index, ObjCon *objL);
Obj* searchLByKey(ObjCon* objL, const char* key);
bool freeArray(ObjCon* objL);

#endif