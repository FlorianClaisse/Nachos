#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "copyright.h"
#include "utility.h"
#include "bitmap.h"
#include "synch.h"

class PageProvider:dontcopythis {
public:
    PageProvider();
    ~PageProvider();

    int GetEmptyPage(void);
    void ReleasePage(int page);
    int NumAvailPage(void);
    int BookPages(int numPages);
private:
    int remainingpages;
    BitMap *bitmap;
    Semaphore *semBitMap;
};
#endif // PAGEPROVIDER_H
#endif // CHANGED