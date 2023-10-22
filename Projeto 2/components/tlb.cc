
#include <iostream>
#include <omp.h>
#include "tlb.h"

using namespace std;

TransitionLookasideBuffer::TransitionLookasideBuffer() {
}

bool TransitionLookasideBuffer::IsPageLoaded(int page_ref) {
    return (!(this->table.find(page_ref) == this->table.end()) && table[page_ref] != 0);
}

void TransitionLookasideBuffer::UpdatePageReference(int virtual_page, int physical_page) {
    this->table[virtual_page] = physical_page;
}

void TransitionLookasideBuffer::UpdateInvertedPageReference(int physical_page, int virtual_page) {
    this->inverted_table[physical_page] = virtual_page;
}

int TransitionLookasideBuffer::GetPageReference(int virtual_page) {
    return table[virtual_page];
}

vector<int> TransitionLookasideBuffer::GetLoadedPages() {
    vector<int> v;
    
    #pragma omp parallel for
    for (auto i: inverted_table) {
        v.push_back(i.second);
    }
    return v;
}

void TransitionLookasideBuffer::PrintTable()
{
    for (const auto& par : table) {
        std::cout << par.first << ":" << par.second << "/";
    }
    std::cout << std::endl;
}
