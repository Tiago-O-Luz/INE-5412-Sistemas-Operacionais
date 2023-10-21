
#include <iostream> 

#include "fifo_algorithm.h"

using namespace std;

FifoAlgorithm::FifoAlgorithm(int frameMax) {
    frameCount = 0;
    framesAmount = frameMax;
    pagefaults = 0;
}

void FifoAlgorithm::ProcessReference(int page_ref) {
    // std::cout << tlb.IsPageLoaded(page_ref) << "se pagina esta carregada" << endl;
    if (!tlb.IsPageLoaded(page_ref)) {
        // If virtual page not loaded
        pagefaults++;
        if (frameCount >= framesAmount) {
            // If all physical pages are occupied
            int p_page = tlb.GetPageReference(page_queue.front());   // Get pyhsical page of first loaded virtual page
            tlb.UpdatePageReference(page_queue.front(), 0);          // Update TLB to unload page
            page_queue.pop();                                       // Remove page from queue
            page_queue.push(page_ref);                              // Add new page reference to queue
            tlb.UpdatePageReference(page_ref, p_page);              // Update TLB on new page entrance
        } else {
            // If physical pages available
            page_queue.push(page_ref);                              // Add new page reference to queue
            tlb.UpdatePageReference(page_ref, frameCount+1);          // Update TLB on new page entrance
            frameCount++;
        }
        std::cout << " pagina faltou: " << pagefaults << endl;
        // tlb.print_table();
    }
}

int FifoAlgorithm::GetPageFaults() {
    return pagefaults;
}

void FifoAlgorithm::print_queue() {
    while (!page_queue.empty()) {
        std::cout << page_queue.front() << " ";
        page_queue.pop();
    }
}