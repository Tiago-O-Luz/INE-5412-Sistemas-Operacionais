

#include "page_algorithm.h"
#include <queue>

using namespace std;

#ifndef FIFO_ALG
#define FIFO_ALG


class FifoAlgorithm: PageAlgorithm {
    private:
        queue<int> page_queue;
    public:
        FifoAlgorithm(int frameMax);
        void ProcessReference(int page_ref) override;
        int get_pagefaults();
        void print_queue();
};

#endif