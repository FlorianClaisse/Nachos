#include "syscall.h"

// ./nachos -x ../test/putstring
// Test l'écriture d'un unique caractère en console d'une chaine
// de caractères plus grande que la taille max de notre buffer
// et d'une chaine stocké dans une variable pour vérifier qu'on ne modifie
// pas le pointeur.
int main() {
    const char* test = "abcdssfsefs<efsef";
    PutString("une variable");
    PutString(test);
    PutString("\n");
    PutString(test);
    PutString("\n");
    
    return 0;
}