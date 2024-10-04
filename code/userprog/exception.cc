// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "syscall.h"
#include "userthread.h"
#include "userfork.h"

#ifdef CHANGED 

#define MAX_USER_SEM 5
BitMap *bitMapSem = new BitMap(MAX_USER_SEM);
Semaphore **listSem = new Semaphore*[MAX_USER_SEM];

unsigned copyStringFromMachine(int from, char *to, unsigned size) {
    unsigned i;
    for(i = 0; i < size - 1; i++) {
        int c;
        bool res = machine->ReadMem(from + i, 1, &c, true);
        if (!res) {
            *(to + i) = '\0';
            return i + 1;
        }
        to[i] = (char) c;
        if (*(to + i) == '\0') return i + 1;
    }

    to[i] = '\0';
    return i + 1;
}

unsigned copyStringToMachine(char* s, int to, unsigned size) {
    unsigned i;
    for (i = 0; i < size; i++) {
        bool res = machine->WriteMem(to + i, 1, (int) s[i]);
        if (!res) { s[i] = '\0'; return i; }
        if (s[i] == '\0') break;
    }

    return i + 1;
}
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void UpdatePC () {
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//int do_ThreadCreate(int f, int arg) r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler (ExceptionType which) {
    int type = machine->ReadRegister (2);
    int address = machine->ReadRegister (BadVAddrReg);

    switch (which) {
        case SyscallException: {
            switch (type) {
                case SC_Halt: {
                    DEBUG ('s', "Shutdown, initiated by user program.\n");
                    interrupt->Powerdown ();
                    break;
                }
#ifdef CHANGED
                case SC_Exit: {
                    DEBUG('s', "Shutdown, initiated by end of program.\n");
                    printf("Exit with code : %d\n", machine->ReadRegister(4));
                    //interrupt->Powerdown();
                    do_ForkExit();
                    break;
                }
                case SC_PutChar: {
                    DEBUG ('s', "PutChar\n");
                    int r4 = machine->ReadRegister(4);
                    consoledriver->PutChar(r4);
                    break;
                }
                case SC_PutString: {
                    DEBUG('s', "PutString\n");
                    int from = machine->ReadRegister(4);
                    unsigned copySize = 0;
                    do {
                        char to[MAX_STRING_SIZE];
                        copySize = copyStringFromMachine(from, to, MAX_STRING_SIZE);
                        from += copySize - 1; // Pour un MAX_STRING_SIZE = 3 Pour ne pas écrire "123\0456\0" mais "123456\0"
                        consoledriver->PutString(to);
                    } while (copySize == MAX_STRING_SIZE);
                    break;
                }
                case SC_GetChar: {
                    DEBUG('s', "GetChar\n");
                    int value = consoledriver->GetChar();
                    machine->WriteRegister(2, value == EOF ? '\0' : value);
                    break;
                }
                case SC_GetString: { 
                    DEBUG('s', "GetString\n");
                    int to = machine->ReadRegister(4);
                    int size = machine->ReadRegister(5);
                    unsigned copySize = 0;
                    printf("to: %d, size: %d\n", to, size);
                    do {
                        char buffer[MAX_STRING_SIZE];
                        if (size >= MAX_STRING_SIZE) {
                            consoledriver->GetString(buffer, MAX_STRING_SIZE);
                            copySize = copyStringToMachine(buffer, to, MAX_STRING_SIZE);
                            to += MAX_STRING_SIZE - 1;
                            size -= MAX_STRING_SIZE - 1;
                        } else {
                            consoledriver->GetString(buffer, size);
                            copySize = copyStringToMachine(buffer, to, size);
                        }
                        printf("copySize: %d, buffer: %s, to: %d, size: %d\n", copySize, buffer, to, size);
                    } while(copySize == MAX_STRING_SIZE);
                    break;
                }
                case SC_PutInt: {
                    DEBUG('s', "PutInt\n");
                    int value = machine->ReadRegister(4);
                    consoledriver->PutInt(value);
                    break;
                }
                case SC_GetInt: {
                    DEBUG('s', "GetInt\n");
                    int value = consoledriver->GetInt();
                    machine->WriteRegister(2, value);
                    break;
                }
                case SC_ThreadCreate: {
                    DEBUG('s', "ThreadCreate\n");
                    int f = machine->ReadRegister(4);
                    int arg = machine->ReadRegister(5);
                    int callback = machine->ReadRegister(6);
                    int ret = do_ThreadCreate(f, arg, callback);
                    machine->WriteRegister(2, ret);
                    break;
                }
                case SC_ThreadExit: {
                    DEBUG('s', "ThreadExit\n");
                    do_ThreadExit();
                    break;
                }
                case SC_SemCreate: {
                    DEBUG('s', "SemCreate\n");
                    int value = machine->ReadRegister(4);
                    int pos = bitMapSem->Find();
                    Semaphore *sem = new Semaphore("Semaphore User", value);
                    if (pos == -1 || sem == NULL) {
                        machine->WriteRegister(2, pos);
                        break;
                    }
                    listSem[pos] = sem;
                    machine->WriteRegister(2, pos);
                    break;
                }
                case SC_SemWait: {
                    DEBUG('s', "SemWait\n");
                    int pos = machine->ReadRegister(4);
                    if (pos < 0 || pos >= MAX_USER_SEM) break;
                    listSem[pos]->P();
                    break;
                }
                case SC_SemPost: {
                    DEBUG('s', "SemPost\n");
                    int pos = machine->ReadRegister(4);
                    if (pos < 0 || pos >= MAX_USER_SEM) break;
                    listSem[pos]->V();
                    break;
                }
                case SC_SemDelete: {
                    DEBUG('s', "SemDelete\n");
                    int pos = machine->ReadRegister(4);
                    if (pos < 0 || pos >= MAX_USER_SEM) break;
                    bitMapSem->Clear(pos);
                    delete listSem[pos];
                    listSem[pos] = NULL;
                    break;
                }
                case SC_ForkExec: {
                    DEBUG('s', "ForkExec\n");
                    int from = machine->ReadRegister(4);
                    char to[MAX_STRING_SIZE];
                    copyStringFromMachine(from, to, MAX_STRING_SIZE);

                    int ret = do_ForkExec(to);
                    machine->WriteRegister(2, ret);
                    break;
                }
#endif
                default: {
                    ASSERT_MSG(FALSE, "Unimplemented system call %d\n", type);
                }
            }

            // Do not forget to increment the pc before returning!
            // This skips over the syscall instruction, to continue execution
            // with the rest of the program
            UpdatePC ();
            break;
          }

        case PageFaultException:
          if (!address) {
            ASSERT_MSG (FALSE, "NULL dereference at PC %x!\n", machine->registers[PCReg]);
          } else {
            // For now
            ASSERT_MSG (FALSE, "Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
          }
          break;

        case ReadOnlyException:
          // For now
          ASSERT_MSG (FALSE, "Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case BusErrorException:
          // For now
          ASSERT_MSG (FALSE, "Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case AddressErrorException:
          // For now
          ASSERT_MSG (FALSE, "Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case OverflowException:
          // For now
          ASSERT_MSG (FALSE, "Overflow at PC %x\n", machine->registers[PCReg]);
          break;

        case IllegalInstrException:
          // For now
          ASSERT_MSG (FALSE, "Illegal instruction at PC %x\n", machine->registers[PCReg]);
          break;

        default:
          ASSERT_MSG (FALSE, "Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
          break;
      }
}
