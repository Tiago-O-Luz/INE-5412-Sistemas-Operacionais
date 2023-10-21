#include <stdio.h>  
#include <stdlib.h> 
#include <iostream> 
#include <string>
#include <chrono>
#include <omp.h>

#include "read_file.cc"
#include "fifo_algorithm.h"
#include "lru_algorithm.h"
#include "opt_algorithm.h"

int main(int argc, char*argv[]) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::string input_file;
    int frames = std::stoi(argv[1]);
    std::cin >> input_file;

    start = std::chrono::system_clock::now();

    FifoAlgorithm fifo(frames);
    LruAlgorithm lru(frames);

    File f(input_file);
    int page = f.read_file();

    // Process file
    while(page != 45) {
        std::cout << page << " pagina" << "\n";
        std::cout << endl;

        page = f.read_file();
    }

    OptAlgorithm opt(frames, f.get_pagemap());
    f.print_pages();
    
    // Process algorithms

    #pragma opm parallel 
    {
        #pragma opm sections 
        {
            #pragma opm section 
            {
                for (auto page: *f.get_inputrefs()) {   
                    fifo.ProcessReference(page);
                };
            }
            #pragma opm section 
            {
                for (auto page: *f.get_inputrefs()) {
                    lru.ProcessReference(page);
                }
            }
            #pragma opm section 
            {
                for (auto page: *f.get_inputrefs()) {
                    opt.ProcessReference(page);
                }
            }
        }
    }
    // for (auto page: *f.get_inputrefs()) {

    //     fifo.ProcessReference(page);
        
    //     lru.ProcessReference(page);
        
    //     opt.ProcessReference(page);
    // }
    //CpuParams params(frames, f.get_pages_input());
    //std::cout <<"\n"<< params.get_frames() << " quadros" << endl;
    std::cout << "FIFO: " << fifo.GetPageFaults() << " PFs" << endl;
    // fifo.print_queue();
    std::cout << "LRU: " << lru.GetPageFaults() << " PFs" << endl;
    // std::cout << lru.GetPageFaults() << "lru page faults" << endl;
    std::cout << "OPT: " << opt.GetPageFaults() << " PFs" << endl;

        // opt.ProcessReference(page);
    //
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished computation at " << std::ctime(&end_time)
            << "elapsed time: " << elapsed_seconds.count() << "s\n";
}