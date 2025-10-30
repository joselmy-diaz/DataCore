#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataCore.h"


typedef struct {
    ObjR obj;
    int length;
    Entry **entries;
} ObjTableH;

// Estructura del nodo del árbol AVL
typedef struct NodeEntry {
    Entry data;
    struct NodeEntry* left;
    struct NodeEntry* right;
    int height;
} NodeEntry;


typedef struct {
    ObjR obj;
    struct NodeEntry* root;
    int size;
} ObjTree;


typedef struct {
    ObjR obj;
    Obj** array;
    size_t length;
    int8_t free;
} ObjArray;


unsigned int hash(const char *key);

Obj* newObj(ObjType type, As* as);
Obj* newNumberF (float NumF);
Obj* newNumber (int Num);
Obj* newTime ();
void setTimeFuture(Obj* obj, int segundos, int nanos);
double getTimeSeconds(Obj* obj);
Obj* newEntry(const char* key, Obj* data);

Obj* newObjString(char * str);
bool setObjString (Obj* objString, const char* str);
void StringPush (Obj* obj1, const char* str);
int getStringSize(Obj* obj);
const char* getString(Obj* obj);

void freeEn (Entry* entry);

Obj* initTH(int size);
Obj *searchTH(Obj *table, const char *key);
bool insertTH(Obj*table, Obj* data);
bool freeTH(Obj *table);

Obj *initTR();
Obj* searchTree(Obj* obj, const char *key);
bool insertAVL(Obj* objT, Obj* data);
int getTreeSize (Obj* obj);
NodeEntry* newNode(Entry* data);
void preOrder(NodeEntry *root);
bool freeTR(Obj* objT);

Obj *initArray(int8_t size);
bool insertIArray (ObjArray* objL, int index, Obj* data);
bool insertArray(Obj* objL, Obj* data);
int getArraySize(Obj* objL);
Obj* searchArray(Obj* objL, int index);
Obj* removeArray(Obj* obj, int index);
Obj* popArray(Obj* obj);
bool freeArray(Obj* objL);

bool freeString(ObjString* obj);
bool freeEntry(Entry* entry);

#endif