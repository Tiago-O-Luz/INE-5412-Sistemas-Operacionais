

#include "fifo_algorithm.h"

using namespace std;

FifoAlgorithm::FifoAlgorithm(int frameMax) {
    frameCount = 0;
    framesAmount = frameMax;
}

void FifoAlgorithm::ProcessReference(int page_ref) {
    if (!tlb.IsPageLoaded(page_ref)) {
        // If virtual page not loaded
        pagefaults++;
        if (frameCount >= framesAmount) {
            // If all physical pages are occupied
            int p_page = tlb.GetPageReference(page_queue.back());   // Get pyhsical page of fisrt loaded virtual page
            tlb.UpdatePageReference(page_queue.back(), 0);          // Update TLB to unload page
            page_queue.pop();                                       // Remove page from queue
            page_queue.push(page_ref);                              // Add new page reference to queue
            tlb.UpdatePageReference(page_ref, p_page);              // Update TLB on new page entrance
        } else {
            // If physical pages available
            page_queue.push(page_ref);                              // Add new page reference to queue
            tlb.UpdatePageReference(page_ref, frameCount);          // Update TLB on new page entrance
            frameCount++;
        }
    }
}
