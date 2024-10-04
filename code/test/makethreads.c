#include <syscall.h>

void print(int n) {
    int i;
    for(i = 0; i < 5; i++)
        PutChar('a');

    PutInt(i);
    PutString("Thread: ");
    PutInt(n);
    // ThreadExit(); plus obligatoire
}

int main() {
    int i;
    PutString("Test sur les threads: \n");
    for(i = 0; i < 20; i++) {
        int res = ThreadCreate(print, &i);
        if (res == -1)
            PutString("Erreur lors de la creation du thread\n");
    }
    ThreadExit();
}