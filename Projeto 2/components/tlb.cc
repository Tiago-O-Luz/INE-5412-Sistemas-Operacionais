

#include "tlb.h"

using namespace std;

TransitionLookasideBuffer::TransitionLookasideBuffer() {
}

bool TransitionLookasideBuffer::IsPageLoaded(int page_ref) {
    return !(this->table.find(page_ref) == this->table.end());
}

void TransitionLookasideBuffer::UpdatePageReference(int virtual_page, int physical_page) {
    this->table[virtual_page] = physical_page;
}

int TransitionLookasideBuffer::GetPageReference(int virtual_page) {
    return table[virtual_page];
}