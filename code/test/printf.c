#include "syscall.h"
#include "vsprintf.h"

/* ./nachos -x ../test/printf
Permet de tester la fonction printf avec à la fois une chaine de caractères
et des nombres pour vérifier que la conversion est correctement éxecuté.
*/

int main() {

    printf("Result: %d + %d = %d", 3, 5, 3 + 5);

    return 0;
}