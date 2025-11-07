#ifndef WAITER_H
#define WAITER_H
#include <pthread.h>
#include "obj.h"
#include "DataCore.h"
#include "Serializer.h"
#include "buffer.h"
// Instrucciones de la Máquina Virtual (VM)
// Cada instrucción ocupa 1 byte en el bytecode

#define REG_COUNT 16

typedef Obj* (*IsReadyFunc)(Obj*);

typedef struct Waiter {
    ObjR obj;
    Obj* watcher;
    IsReadyFunc isReady;
    Obj* this;
} Waiter;

extern Obj* listWaiters;

void watcherObjInit();
void* waitHandle(void* arg);
void watcherObjCreate(Obj* watcher, Obj* this, IsReadyFunc IsReady);
void liberateWaiter(Waiter* waiter, int* i);

#endif // WAITER_H
