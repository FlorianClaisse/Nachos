#include "syscall.h"

/* ./nachos -x ../test/getstring
Test GetString avec un buffer égale à 21 alors que MAX_STRING_SIZE vaut 10
pour vérifier qu'on arrive à stocker plus que notre MAX_STRING_SIZE et
21 pour être sur de ne pas s'arreter trop tôt.
*/

int main() {
#if 1
    char buffer[21];
    GetString(buffer, 21);
    PutString(buffer);
#endif

    return 0;
}