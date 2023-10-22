#include <stdio.h>  
#include <stdlib.h> 
#include <iostream> 
#include <string>
// #include <chrono>
#include <omp.h>

#include "read_file.cc"
#include "fifo_algorithm.h"
#include "lru_algorithm.h"
#include "opt_algorithm.h"

int main(int argc, char*argv[]) {
    // std::chrono::time_point<std::chrono::system_clock> start, end;
    if (argc < 2) {
        std::cout << "Informe a quantidade de frames para simulacao";
        return 1;
    }

    std::string input_file;
    int frames = std::stoi(argv[1]);
    std::cin >> input_file;

    // start = std::chrono::system_clock::now();

    File f(input_file);
    int page = f.ReadFile();

    // Process file
    while(page != 45) {
        page = f.ReadFile();
    }

    FifoAlgorithm fifo(frames);
    LruAlgorithm lru(frames);
    OptAlgorithm opt(frames, f.GetPagemap());
    
    // Process algorithms

    #pragma opm parallel 
    {
        #pragma opm sections 
        {
            #pragma opm section 
            {
                #pragma opm parallel for
                for (auto page: *f.GetInputRefs()) {   
                    fifo.ProcessReference(page);
                };
            }
            #pragma opm section 
            {
                #pragma opm parallel for
                for (auto page: *f.GetInputRefs()) {
                    lru.ProcessReference(page);
                }
            }
            #pragma opm section 
            {
                #pragma opm parallel for
                for (auto page: *f.GetInputRefs()) {
                    opt.ProcessReference(page);
                }
            }
        }
    }
    if (f.GetFlagError()) {
        std::cout << frames << " quadros" << endl;
        std::cout << f.GetLenghtRefs() << " refs" << endl;
        std::cout << "FIFO: " << fifo.GetPageFaults() << " PFs" << endl;
        std::cout << "LRU: " << lru.GetPageFaults() << " PFs" << endl;
        std::cout << "OPT: " << opt.GetPageFaults() << " PFs" << endl;
    }

    // end = std::chrono::system_clock::now();

    // std::chrono::duration<double> elapsed_seconds = end - start;
    // std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    // std::cout << "finished computation at " << std::ctime(&end_time)
    //         << "elapsed time: " << elapsed_seconds.count() << "s\n";
}