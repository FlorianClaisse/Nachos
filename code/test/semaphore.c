#include <syscall.h>

#define BUFFER_SIZE 5

sem_t empty, full, mutex;
int buffer[BUFFER_SIZE];
int counter = 0;

void producer(char *arg) {
    int item;
    int i;
    for (i = 0; i < 5; i++) {
        item = i + 1;
        SemWait(empty);
        SemWait(mutex);
        buffer[counter] = item;
        counter++;
        PutString(arg);
        PutInt(item);
        SemPost(mutex);
        SemPost(full);
    }
}

void consumer(char *arg) {
    int item;
    int i;
    for(i = 0; i < 5; i++) {
        SemWait(full);
        SemWait(mutex);
        item = buffer[counter - 1];
        counter--;
        PutString(arg);
        PutInt(item);
        SemPost(mutex);
        SemPost(empty);
    }
}

int main() {
    int ret;
    // Check syscall work use -d s
    sem_t sem = SemCreate(1);
    SemWait(sem);
    PutChar('r');
    SemPost(sem);
    SemDelete(sem);
    
    // Check in action
    empty = SemCreate(BUFFER_SIZE);
    full = SemCreate(0);
    mutex = SemCreate(1);

    ret = ThreadCreate(producer, "Producer");
    if (ret == -1)
            PutString("Erreur lors de la creation du thread 1\n");
    ret = ThreadCreate(consumer, "Consumer");
    if (ret == -1)
            PutString("Erreur lors de la creation du thread 2\n");

    ThreadExit();
}