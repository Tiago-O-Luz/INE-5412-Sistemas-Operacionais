
#include <iostream> 

#include "lru_algorithm.h"

using namespace std;

LruAlgorithm::LruAlgorithm(int frameMax) {
    frameCount = 0;
    framesAmount = frameMax;
    pagefaults = 0;
}

void LruAlgorithm::ProcessReference(int page_ref) {
    if (!tlb.IsPageLoaded(page_ref)) {
        // If virtual page not loaded
        // Keep this code of FIFO
        std::cout << " pagina faltou: " << pagefaults << endl;
        // tlb.print_table();
        pagefaults++;
        if (frameCount >= framesAmount) {
            // If all physical pages are occupied
            int p_page = tlb.GetPageReference(page_queue.front());   // Get pyhsical page of first loaded virtual page
            tlb.UpdatePageReference(page_queue.front(), 0);          // Update TLB to unload page
            page_queue.erase(page_queue.begin());                                       // Remove page from queue
            page_queue.push_back(page_ref);                              // Add new page reference to queue
            tlb.UpdatePageReference(page_ref, p_page);              // Update TLB on new page entrance
        } else {
            // If physical pages available
            page_queue.push_back(page_ref);                           // Add new page reference to queue
            tlb.UpdatePageReference(page_ref, frameCount+1);          // Update TLB on new page entrance
            frameCount++;
        }
    } else {
        // If virtual page IS loaded put the page like the most recently used
        vector<int>::iterator it = page_queue.begin();
        while (*it != page_ref)
        {
            it++;
        }
        page_queue.erase(it);
        page_queue.push_back(page_ref);

        // std::queue<int> temp_page_queue;                            // Make a temporary queue
        // for (int i = 0; i < frameCount; i++) {                      
        //     int page = page_queue.front();
        //     page_queue.pop();

        //     if (page != page_ref) {                                 // If is NOT the current page reference, just push back to the temporary queue
        //         temp_page_queue.push(page);
        //     }
        // }
        // temp_page_queue.push(page_ref);                             // At end, push the page reference

        // page_queue = temp_page_queue;                               // the temporary queue is the new page queue
    }
}

int LruAlgorithm::get_pagefaults()
{
    return pagefaults;
}
