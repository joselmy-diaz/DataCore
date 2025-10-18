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
    Entry* data;
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
} ObjArray;


unsigned int hash(const char *key);

Obj* newObj(ObjType type, As* as);
Obj* newObjString(char * str);
bool setObjString (Obj* objString, const char* str);
int getStringSize(Obj* obj);
char* getString(Obj* obj);
Entry* newEntry(const char* key, Obj* data);

void freeEn (Entry* entry);

Obj* initTH(int size);
Obj *searchTH(Obj *table, const char *key);
void NewFunction(Entry *entry);
bool insertTH(Obj*table, Entry* data);
bool freeTH(Obj *table);

Obj *initTR();
Obj* searchTree(Obj* obj, const char *key);
bool insertAVL(Obj* objT, Entry* data);
NodeEntry* newNode(Entry* data);
void preOrder(NodeEntry *root);
bool freeTR(Obj* objT);

Obj *initArray(int8_t size);
bool insertIArray (ObjArray* objL, int index, Obj* data);
bool insertArray(Obj* objL, Obj* data);
int getArraySize(Obj* objL);
Obj* searchArray(Obj* objL, int index);;
Obj* popArray(Obj* obj);
bool freeArray(Obj* objL);

bool freeString(ObjString* obj);
bool freeEntry(Entry* entry);

#endif