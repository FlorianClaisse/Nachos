#include <syscall.h>

/*void print(char *c) {
    PutString("Quelques ");
    PutString("caracteres ");
    PutString("a l'ecran\n");
}*/


int main() {
    PutString("Debut du pere\n...");
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    PutString("Fin du pere\n");
    //return 0;
}

/*int main() {
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    ThreadCreate(print, "encore");

    PutChar('Q');
    PutChar('u');
    PutChar('e');
    PutChar('l');
    PutChar('q');
    PutChar('u');
    PutChar('e');
    PutChar('s');
    PutChar(' ');
    PutChar('c');
    PutChar('a');
    PutChar('r');
    PutChar('a');
    PutChar('c');
    PutChar('t');
    PutChar('e');
    PutChar('r');
    PutChar('e');
    PutChar('s');
    PutChar(' ');
    PutChar('a');
    PutChar(' ');
    PutChar('l');
    PutChar('\'');
    PutChar('e');
    PutChar('c');
    PutChar('r');
    PutChar('a');
    PutChar('n');
    PutChar('.');
    PutChar('\n');

    ThreadExit();
}*/