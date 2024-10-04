#ifdef CHANGED

#include "system.h"
#include "pageprovider.h"

PageProvider::PageProvider() {
    this->bitmap = new BitMap(NumPhysPages);
    this->semBitMap = new Semaphore("PageProvider bitmap", 1);
    this->remainingpages = NumPhysPages;
}

PageProvider::~PageProvider() {
    delete this->bitmap;
    delete this->semBitMap;
}

int PageProvider::GetEmptyPage() {
    //RandomInit(0);
    this->semBitMap->P();

    //int page = Random() % NumPhysPages;
    //while(this->bitmap->Test(page))
        //page = Random() % NumPhysPages;

    //this->bitmap->Mark(page);
    int page = this->bitmap->Find();
    memset(&machine->mainMemory[page * PageSize], 0, PageSize);
    this->semBitMap->V();
    return page;
}

void PageProvider::ReleasePage(int page) {
    this->semBitMap->P();
    bitmap->Clear(page);
    this->semBitMap->V();
}

int PageProvider::NumAvailPage() {
    this->semBitMap->P();
    int returnValue = this->bitmap->NumClear();
    this->semBitMap->V();
    return returnValue;
}

int PageProvider::BookPages(int numPages) {
    this->semBitMap->P();
    if (this->remainingpages - numPages < 0) {
        return -1;
    }
    this->remainingpages -= numPages;
    this->semBitMap->V();
    return 1;
}

#endif