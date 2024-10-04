// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"
#include "bitmap.h"
#include "noff.h"
#include "list.h"

#ifdef CHANGED
class Semaphore;
#endif

#define UserStacksAreaSize		4096	// increase this as necessary!

#ifdef CHANGED
#define ThreadNumPages  2 // Nombre de pages par threads
#endif

class AddrSpace: public dontcopythis {
  public:
    AddrSpace (OpenFile * executable); // Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();              // De-allocate an address space

    void InitRegisters (void);  // Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState (void);      // Save/restore address space-specific
    void RestoreState (void);   // info on a context switch

    #ifdef CHANGED
    bool beDestroyed;
    const static int maxNumThread = (int) (UserStacksAreaSize / (ThreadNumPages * PageSize)); // Le nombre maximum de thread possible
    int runningThreads; // Le nombres de threads qui tourne actuellement.
    Semaphore *semRunningThreads; // Semaphore pour manipuler la variable runningThreads.
    BitMap *bitMapStack; // La bitmap qui permet de trouver la nouvelle zone memoire libre.
    Semaphore *semBitMapStack; // Semaphore pour manipuler la variable bitMapStack
    void InitMainThread(); // Init le thread main pour qu'il fonctionne comme les autres
    void InitThreadRegisters(int f, int arg, int callback); // Init les registres pour un nouveau user thread.
    int GetNewZone(); // Renvoie la nouvelle zone du bitmap libre.
    void FreeBitMap(); // Liberer la bitmap prise par le thread
    bool ThreadAlone(); // Retourne si le thread courant est le dernier ou non.
    void UpdateRunningThread(int add); // Ajoute la valeur au nombre de thread qui tourne actuellement.
    int AllocateUserStack(void); // ENCIENNE fonction pour donner l'espace de la bitmap.
    #endif

    unsigned Dump(FILE *output, unsigned addr_s, unsigned sections_x, unsigned virtual_x, unsigned virtual_width,
                    unsigned physical_x, unsigned virtual_y, unsigned y,
                    unsigned blocksize);
                                // Dump program layout as SVG
    unsigned NumPages(void) { return numPages; }

  private:
    NoffHeader noffH;           // Program layout

    TranslationEntry * pageTable; // Page table
    unsigned int numPages;      // Number of pages in the page table
};

extern List AddrspaceList;

#endif // ADDRSPACE_H
