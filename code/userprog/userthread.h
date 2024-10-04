#ifdef CHANGED
#ifndef USER_THREAD_H
#define USER_THREAD_H

#include "system.h"
#include "syscall.h"

extern int do_ThreadCreate(int f, int arg, int callback);
extern void do_ThreadExit();

#endif // USER_THREAD_H
#endif // CHANGED