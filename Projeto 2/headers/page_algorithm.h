

#include "tlb.h"


#ifndef PAGE_ALG
#define PAGE_ALG

class PageAlgorithm{
    protected:
        TransitionLookasideBuffer tlb;
        int frameCount; // nº de quadros em uso
        int framesAmount; //nº de quadros max
        int pagefaults; //nº de acesso a memória

    public:
        virtual void ProcessReference(int page_ref) = 0;
};

#endif
