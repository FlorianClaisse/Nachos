#include "syscall.h"

/* ./nachos -x ../test/getchar
On teste GetChar avec PutChar pour vérifier le résultat obtenue.
*/

int main() {
#if 1
    PutChar(GetChar());
#endif

    return 0;
}