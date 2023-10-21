

#include "page_algorithm.h"
#include <queue>

using namespace std;

#ifndef OPT_ALG
#define OPT_ALG


class OptAlgorithm: PageAlgorithm {
    private:
        map<int, vector<int>>* pagemap;
    public:
        OptAlgorithm(int frameMax, map<int, vector<int>>* pagemap);
        void ProcessReference(int page_ref) override;
        int GetPageFaults();
};

#endif