
#include<vector>
#include<map>

using namespace std;

#ifndef TLB
#define TLB

class TransitionLookasideBuffer {
    private:
        map<int,int> table;
        map<int,int> inverted_table;

    public:
        TransitionLookasideBuffer();
        bool IsPageLoaded(int page_ref);
        void UpdatePageReference(int virtual_page, int physical_page);
        void UpdateInvertedPageReference(int physical_page, int virtual_page);
        int GetPageReference(int virtual_page);
        vector<int> GetLoadedPages();
        void PrintTable();
};




#endif