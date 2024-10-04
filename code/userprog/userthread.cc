#ifdef CHANGED
#include "userthread.h"
#include "userfork.h"

static void StartUserThread(void *schmurtz) {
    int *args = (int*) schmurtz;
    int f = args[0], arg = args[1], callback = args[2];
    free(args);
    
    currentThread->space->InitThreadRegisters(f, arg, callback);
    machine->DumpMem("threads.svg");
    machine->Run();
}

int do_ThreadCreate(int f, int arg, int callback) {
    Thread *newThread = new Thread("firstThread");
    if (newThread == NULL) return -1;

    int zone = currentThread->space->GetNewZone();
    if (zone < 0) {
        delete newThread;
        return -1;
    }

    newThread->space = currentThread->space;
    newThread->space->UpdateRunningThread(1);
    newThread->setZone(zone);

    // On stock les parametres pour les faire durer au dela de la fonction
    int *args = (int*) malloc(sizeof(int) * 3);
    args[0] = f;
    args[1] = arg;
    args[2] = callback;

    newThread->Start(StartUserThread, args);
    return 0;
}

void do_ThreadExit() {
    // On enleve 1 au nombre de thread qui tourne.
    currentThread->space->UpdateRunningThread(-1);
    DEBUG('x', "thread %s stop process\n", currentThread->getName());
    currentThread->space->FreeBitMap();
    // Si je suis le dernier thread, je termine nachos
    if (currentThread->space->ThreadAlone()) {
        DEBUG('x', "thread %s finish process\n", currentThread->getName());
        do_ForkExit();
    } else { 
        currentThread->Finish();
    }
}
#endif