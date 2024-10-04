#include "syscall.h"

void print(char c) {
    int j = 0, i=0;
    for (j=0; j<3; j++) {
        for (i=0; i<3000; i++) {}
        PutChar(c);
    }
}

int main() {
    int i;
    for (i=0; i<3000; i++) {}
    
    PutString("Début du fils 0 : lancement des deux threads a et z\n");

    ThreadCreate(print, (void *)'a');
    ThreadCreate(print, (void *)'z');
    ThreadCreate(print, (void *)'a');
    ThreadCreate(print, (void *)'z');
    ThreadCreate(print, (void *)'a');
    ThreadCreate(print, (void *)'z');
    ThreadCreate(print, (void *)'a');
    ThreadCreate(print, (void *)'z');
    ThreadCreate(print, (void *)'a');
    ThreadCreate(print, (void *)'z');
    ThreadCreate(print, (void *)'a');
    ThreadCreate(print, (void *)'z');
    
    PutString("\nFin du thread main du fils 0\n");
    //return 0;
    ThreadExit();
}