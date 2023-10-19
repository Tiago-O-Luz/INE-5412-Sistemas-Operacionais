
#include <iostream>
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

int TransitionLookasideBuffer::GetPageReference(int virtual_page) {
    return table[virtual_page];
}

void TransitionLookasideBuffer::print_table()
{
    for (const auto& par : table) {
        std::cout << par.first << ":" << par.second << "/";
    }
    std::cout << std::endl;
}
