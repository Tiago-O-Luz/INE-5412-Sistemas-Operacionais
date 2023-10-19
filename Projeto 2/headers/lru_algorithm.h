

#include "page_algorithm.h"
#include <queue>

using namespace std;

#ifndef LRU_ALG
#define LRU_ALG


class LruAlgorithm: PageAlgorithm {
    private:
        vector<int> page_queue;
    public:
        LruAlgorithm(int frameMax);
        void ProcessReference(int page_ref) override;
        int get_pagefaults();
};

#endif