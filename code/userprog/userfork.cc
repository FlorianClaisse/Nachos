#ifdef CHANGED

#include "userfork.h"
#include "system.h"

void StartUserProc(void *arg) {
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    currentThread->space->InitMainThread();
    machine->DumpMem ("memory.svg");
    machine->Run();
}

int do_ForkExec(char *filename) {
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf ("Unable to open file %s\n", filename);
        delete [] filename;
        return -1;
    }

    space = new AddrSpace (executable);

    if (space == NULL) {
        printf("%s : Insufficient memory to start the process.\n", filename);
        delete executable;
        return -1;
    }

    delete executable;

    Thread *main = new Thread(filename);
    main->space = space;
    machine->UpdateRunningProcess(1);
    main->Start(StartUserProc, 0);

    return 0;
}

void do_ForkExit() {
    DEBUG('x', "thread: %s stop machine", currentThread->getName());
    machine->UpdateRunningProcess(-1);

    if (machine->ProcessAlone())
        interrupt->Powerdown();

    currentThread->space->beDestroyed = true;
    currentThread->Finish();
}

#endif