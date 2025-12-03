#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "DataCore.h"
#include "obj.h"
#include "structure.h"
#include "waiter.h"


void funsiEspera(Obj* this) {
    printf("⏳ Esperando en: ");
    printObjf(this);
}

void handle_signal(int sig) {
    atomic_store(&running, 0);
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    watcherObjInit();
    Obj* varS = newObjString("hola mundo!");
    bloqueaObj(varS);
    watcherObjCreate(varS, varS, funsiEspera);
    pthread_t watcher_thread;


    for (int i = 0; i < 10; i++) {
        Obj* objTime = newTime();
        setTimeFuture(objTime, 10 - i, 1);
        if (i == 1) watcherObjCreate(objTime,  varS, desbloqueaObj);
        else watcherObjCreate(objTime,  objTime, funsiEspera);
    }

    pthread_create(&watcher_thread, NULL, waitHandle, NULL);

    pthread_join(watcher_thread, NULL);
    freeObjs(varS);
    printf("\n🛠️  Sistema terminado.\n");
    return 0;
}