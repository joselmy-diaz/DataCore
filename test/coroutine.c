#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <string.h>
#include <sched.h>

/* Simula tus includes */
#include "DataCore.h"
#include "obj.h"
#include "stuctura.h"


Obj* lisA;
_Atomic int running = 1;

typedef struct Asion {
    ObjR obj;
    Obj* watcher;
    Obj* strObj;
} Asion;

void watcherObjPush (Obj* watcher, Obj* obj) {
    Asion* asion = malloc(sizeof(Asion));
    asion->obj.type = 20;
    asion->watcher = watcher;
    asion->strObj = obj;
    insertArray(lisA, (Obj*)asion);
}



void* watcher_run(void* arg) {
    struct timespec tiempo_actual;

    while (atomic_load(&running)) {
        clock_gettime(CLOCK_MONOTONIC, &tiempo_actual);
        double actual_segundos = tiempo_actual.tv_sec + (double)tiempo_actual.tv_nsec / 1000000000.0;

        for (int i = 0; i < getArraySize(lisA); i++) {
            Asion* asion = (Asion*)searchArray(lisA, i);

            if (asion == NULL) continue;
            if (GetType(asion->watcher) == TYPE_TIME) {        // esperas de tienpo
                double tiempo_obj_segundos = getTimeSeconds(asion->watcher);
                if (actual_segundos - tiempo_obj_segundos >= 0) {
                    printf("⏱️  Tiempo transcurrido para objeto %d: %.5f segundos!!\n", i, actual_segundos - tiempo_obj_segundos);
                    printObjf(asion->strObj);
                    removeArray(lisA, i);
                    i--;
                    if (getArraySize(lisA) == 0) atomic_store(&running, 0);
                }
            } else{
                if (!IsBlock(asion->watcher)) {
                    printf("liberado bloqueo: %d : ", i);
                    printObjf(asion->strObj);
                    removeArray(lisA, i);
                    i--;
                    if (getArraySize(lisA) == 0) atomic_store(&running, 0);
                }
            }
        }
        nanosleep((const struct timespec[]){{0, 10000L}}, NULL);
    }

    printf("⚙️  Watcher finalizado.\n");
    return NULL;
}

/* -------- Main -------- */
int main(void) {
    lisA = initArray(10);
    Obj* varS = newObjString("hola mundo!");
    bloqueaObj(varS);
    pthread_t watcher_thread;


    for (int i = 0; i < 50; i++) {
        Obj* objTime = newTime();
        setTimeFuture(objTime, 50 - i, 1);
        watcherObjPush(objTime, newObjString("hola time"));
        watcherObjPush(varS, varS);
    }

    pthread_create(&watcher_thread, NULL, watcher_run, NULL);
    sleep(6);
    desbloqueaObj(varS);
    pthread_join(watcher_thread, NULL);
    
    // for (int i = 0; i < CORES; i++)
    //     pthread_join(threads[i], NULL);

    printf("\n📦 Estado final de varS: ");
    printObjf(varS);

    printf("\n🛠️  Sistema terminado.\n");
    return 0;
}


// prueba 1 de bloqueo


// #include <pthread.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

// #include "DataCore.h"
// #include "stuctura.h"

// Obj* root;

// // funsion que genera una espera activa


// void* foo(void* arg) {
//     Obj* this = (Obj*)arg;
//     Obj* data = searchTH(this, "key1");
//     bloqueaObj(data);
//     printf("Data from thread: ");
//     printObjf(data);
//     StringPush(data, "|");
//     desbloqueaObj(data);

//     return NULL;
// }

// // helper macro para crear una instrucción

// int main() {
//     pthread_t threads[30];
//     root = initTH(1500);
//     Obj* obj1 = newObjString("Hello, World!");

//     insertTH(root, newEntry("key1", obj1));
//     bloqueaObj(obj1);
//     for (int i = 0; i < 30; i++) {
//         pthread_create(&threads[i], NULL, foo, root);
//     }
//     desbloqueaObj(obj1);
//     for (int i = 0; i < 30; i++) {
//         pthread_join(threads[i], NULL);
//     }
//     printf("Final data: ");
//     printObjf(obj1);
//     freeTH(root);
//     return 0;
// }
