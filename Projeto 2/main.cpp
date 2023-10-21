#include <stdio.h>  
#include <stdlib.h> 
#include <iostream> 
#include <string>
#include "read_file.cc"
#include "fifo_algorithm.h"
#include "lru_algorithm.h"
#include "opt_algorithm.h"

int main(int argc, char*argv[]) {
    std::string input_file;
    int frames = std::stoi(argv[1]);
    std::cin >> input_file;

    FifoAlgorithm fifo(frames);
    LruAlgorithm lru(frames);

    File f(input_file);
    int page = f.read_file();

    while(page != 45) {
        std::cout << page << " pagina" << "\n";
        std::cout << endl;
        
        fifo.ProcessReference(page);
        lru.ProcessReference(page);

        page = f.read_file();
    }

    OptAlgorithm opt(frames, f.get_pagemap());
    
    for (auto page: *f.get_inputrefs()) {
        opt.ProcessReference(page);
    }
    //CpuParams params(frames, f.get_pages_input());
    //std::cout <<"\n"<< params.get_frames() << " quadros" << endl;
    std::cout << "FIFO: " << fifo.GetPageFaults() << " PFs" << endl;
    // fifo.print_queue();
    std::cout << "LRU: " << lru.GetPageFaults() << " PFs" << endl;
    // std::cout << lru.GetPageFaults() << "lru page faults" << endl;
    std::cout << "OPT: " << opt.GetPageFaults() << " PFs" << endl;

        // opt.ProcessReference(page);
    //
}