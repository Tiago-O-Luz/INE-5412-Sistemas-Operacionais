
#include<vector>
#include<map>

using namespace std;

#ifndef TLB
#define TLB

class TransitionLookasideBuffer {
    private:
        int tableSize;
        map<int,int> table;

    public:
        TransitionLookasideBuffer();
        bool IsPageLoaded(int page_ref);
        void UpdatePageReference(int virtual_page, int physical_page);
        int GetPageReference(int virtual_page);
};




#endif