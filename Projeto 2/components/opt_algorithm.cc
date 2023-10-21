

#include <iostream>
#include "opt_algorithm.h"


using namespace std;


OptAlgorithm::OptAlgorithm(int frameMax, map<int, vector<int>>* pagemap) {
    frameCount = 0;
    framesAmount = frameMax;
    pagefaults = 0;
    this->pagemap = pagemap;
};

void OptAlgorithm::ProcessReference(int page_ref) {
    if (!tlb.IsPageLoaded(page_ref)) {
        // If virtual page not loaded
        // Keep this code of FIFO
        std::cout << " pagina faltou: " << page_ref << endl;
        // tlb.PrintTable();
        pagefaults++;
        if (frameCount >= framesAmount) {
            // If all physical pages are occupied
            vector<int> loaded_pages = tlb.GetLoadedPages();
            int maxTimeRef = 0;
            int pageToSwap = 0;
            for (auto loaded_p: loaded_pages) {
                if (pagemap->at(loaded_p).empty()) {
                    pageToSwap = loaded_p;
                    break;
                };
                int i = pagemap->at(loaded_p).front();
                if (i >= maxTimeRef) {
                    maxTimeRef = i;
                    pageToSwap = loaded_p;
                }
            }
            cout << "Page swap:" << pageToSwap << " for " << page_ref << endl;

            int p_page = tlb.GetPageReference(pageToSwap);   // Get pyhsical page of first loaded virtual page
            tlb.UpdatePageReference(pageToSwap, 0);          // Update TLB to unload page
            tlb.UpdateInvertedPageReference(p_page, page_ref);
            tlb.UpdatePageReference(page_ref, p_page);              // Update TLB on new page entrance
            pagemap->at(page_ref).erase(pagemap->at(page_ref).begin());
        } else {
            // If physical pages available
            pagemap->at(page_ref).erase(pagemap->at(page_ref).begin());
            frameCount++;
            tlb.UpdatePageReference(page_ref, frameCount);          // Update TLB on new page entrance
            tlb.UpdateInvertedPageReference(frameCount, page_ref);          // Update TLB on new page entrance
        }
    } else {
        // If virtual page IS loaded put the page like the most recently used
        pagemap->at(page_ref).erase(pagemap->at(page_ref).begin());
    }  
}

int OptAlgorithm::GetPageFaults() {
    return pagefaults;
}