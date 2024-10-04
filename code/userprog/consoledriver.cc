#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

ConsoleDriver::ConsoleDriver(const char *in, const char *out) {
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    this->readMutex = new Semaphore("read mutex", 1);
    this->writeMutex = new Semaphore("write mutex", 1);
    this->putStringMutex = new Semaphore("put string mutex", 1);
    this->getStringMutex = new Semaphore("get String mutex", 1);

    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}

ConsoleDriver::~ConsoleDriver() {
    delete console;
    delete writeDone;
    delete readAvail;
    delete readMutex;
    delete writeMutex;
    delete putStringMutex;
    delete getStringMutex;
}

void ConsoleDriver::PutChar(int ch) {
    writeMutex->P();
    console->TX (ch);
    writeDone->P();
    writeMutex->V();
}
int ConsoleDriver::GetChar() {
    readMutex->P();
    readAvail->P();        // wait for character to arrive
    readMutex->V();
    return console->RX();
}

void ConsoleDriver::PutString(const char *s) {
    putStringMutex->P();
    while(*s != '\0') {
        char t = *s;
        this->PutChar(t);
        s = s + sizeof(char);
    }
    putStringMutex->V();
}

void ConsoleDriver::GetString(char *s, int n) {
    int i;
    char c;
    this->getStringMutex->P();
    for(i = 0; i < n - 1; i++) {
        c = this->GetChar();
        if (c == EOF) break;
        else if (c == '\n') { s[i] = c; i++; break; }
        else s[i] = c;
    }
    this->getStringMutex->P();
    s[i] = '\0';
}

void ConsoleDriver::PutInt(int i) {
    char* buffer = new char[MAX_STRING_SIZE];
    snprintf(buffer, MAX_STRING_SIZE, "%d", i);
    this->PutString(buffer);
    delete [] buffer;
}

int ConsoleDriver::GetInt() {
    int value;
    char* buffer = new char[MAX_STRING_SIZE];
    this->GetString(buffer, MAX_STRING_SIZE);
    sscanf(buffer, "%d", &value);
    delete [] buffer;
    return value;
}

#endif // CHANGED