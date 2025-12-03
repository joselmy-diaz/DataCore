#include "waiter.h"

Obj* listWaiters;

void watcherObjInit() {
    listWaiters = initArray(3);
}

void watcherObjCreate(Obj* watcher, Obj* this, IsReadyFunc IsReady) {
    Waiter* asion = malloc(sizeof(Waiter));
    asion->obj.type = 20;
    asion->watcher = watcher;
    asion->isReady = IsReady;
    asion->this = this;
    insertArray(listWaiters, (Obj*)asion);
}

/* -------- Watcher (Scheduler) -------- */
_Atomic int running = 1;

bool isWatcher(Waiter* asion, double actual_segundos) {
    if (IsBlock(asion->watcher)) return false; // liberasion por bloqueo
    if (GetType(asion->watcher) == TYPE_TIME) {        // liberasion por esperas de tiempo
        double tiempo_obj_segundos = getTimeSeconds(asion->watcher);
        if (actual_segundos - tiempo_obj_segundos >= 0) {
            freeTime(asion->watcher);
        } else return false;
    }

    return true;
}

void* waitHandle(void* arg) {
    struct timespec tiempo_actual;

    while (atomic_load(&running)) {
        clock_gettime(CLOCK_MONOTONIC, &tiempo_actual);
        double actual_segundos = tiempo_actual.tv_sec + (double)tiempo_actual.tv_nsec / 1000000000.0;

        for (int i = 0; i < getArraySize(listWaiters); i++) {
            Waiter* asion = (Waiter*)searchArray(listWaiters, i);
            if (asion == NULL) continue;
            if (!isWatcher(asion, actual_segundos)) continue;
            asion->isReady(asion->this);
            removeArray(listWaiters, i);
            free(asion);
            i--;
            if (getArraySize(listWaiters) == 0) atomic_store(&running, 0);
        }
        nanosleep((const struct timespec[]){{0, 10000L}}, NULL);
    }

    printf("⚙️  Watcher finalizado.\n");
    return NULL;
}



