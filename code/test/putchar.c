#include "syscall.h"

/* ./nachos -x ../test/putchar
écrit une suite de caractère "abcd" suivi de l'écriture d'un caractère de retour ligne.
*/

void print(char c, int n) {
    int i;
    #if 1
    for (i = 0; i < n; i++) {
        PutChar(c + i);
    }
    PutChar('\n');
    #endif
}

int main() {
    print('a',4);
    //Halt();
    return 12;
}