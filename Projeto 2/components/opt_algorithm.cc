

#include <iostream>
#include "opt_algorithm.h"
#include <omp.h>


using namespace std;


OptAlgorithm::OptAlgorithm(int frameMax, map<int, vector<int>>* pagemap) {
    frameCount = 0;
    framesMax = frameMax;
    pageFaults = 0;
    this->pagemap = pagemap;
};

void OptAlgorithm::ProcessReference(int page_ref) {
    if (!tlb.IsPageLoaded(page_ref)) {
        // If virtual page not loaded
        // std::cout << " pagina faltou: " << page_ref << endl;
        // tlb.PrintTable();
        pageFaults++;
        if (frameCount >= framesMax) {
            // If all physical pages are occupied
            vector<int> loaded_pages = tlb.GetLoadedPages(); // Get current loaded virtual pages
            int maxTimeRef = 0;
            int pageToSwap = 0; // Page that will take longer to be called again

            #pragma omp parallel for schedule(guided) reduction(max:maxTimeRef)
            for (auto loaded_p: loaded_pages) {
                // Iterate over every loaded page
                if (pagemap->at(loaded_p).empty()) {
                    // If loaded page wont be called again found page for swap
                    #pragma omp critical
                    {
                        pageToSwap = loaded_p;
                    }
                    break;
                };
                int i = pagemap->at(loaded_p).front();
                if (i >= maxTimeRef) {
                    // If this is page that will take longer to be called again
                    maxTimeRef = i;
                    #pragma omp critical 
                    {
                        pageToSwap = loaded_p;
                    }
                }
            }
            // cout << "Page swap:" << pageToSwap << " for " << page_ref << endl;

            int p_page = tlb.GetPageReference(pageToSwap);      // Get pyhsical page of loaded virtual page
            tlb.UpdatePageReference(pageToSwap, 0);             // Update TLB to unload page
            tlb.UpdateInvertedPageReference(p_page, page_ref);  // Update Inverterd Table
            tlb.UpdatePageReference(page_ref, p_page);          // Update TLB on new page entrance
            pagemap->at(page_ref).erase(pagemap->at(page_ref).begin()); // Remove reference of page from map structure
        } else {
            // If physical pages available
            pagemap->at(page_ref).erase(pagemap->at(page_ref).begin()); // Remove reference of page from map structure
            frameCount++;
            tlb.UpdatePageReference(page_ref, frameCount);              // Update TLB on new page entrance
            tlb.UpdateInvertedPageReference(frameCount, page_ref);      // Update TLB on new page entrance
        }
    } else {
        // If virtual page is loaded
        pagemap->at(page_ref).erase(pagemap->at(page_ref).begin()); // Remove reference of page from map structure
    }  
}

int OptAlgorithm::GetPageFaults() {
    return pageFaults;
}