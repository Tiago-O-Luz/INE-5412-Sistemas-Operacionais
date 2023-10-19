#include <stdio.h>  
#include <stdlib.h> 
#include <iostream> 
#include <string>
#include "read_file.cc"
#include "fifo_algorithm.h"
#include "lru_algorithm.h"

int main(int argc, char*argv[]) {
    std::string input_file;
    int frames = std::stoi(argv[1]);
    std::cin >> input_file;

    FifoAlgorithm fifo(frames);
    LruAlgorithm lru(frames);

    File f(input_file);
    int page = f.read_file();

    while(page != -48) {
        fifo.ProcessReference(page);
        lru.ProcessReference(page);

        std::cout << page << " página" << "\n";
        std::cout << endl;
        page = f.read_file();
    }

    std::cout << fifo.get_pagefaults() << "fifo page faults" << endl;
    std::cout << endl;
    fifo.print_queue();
    std::cout << endl;
    std::cout << lru.get_pagefaults() << "lru page faults" << endl;

        // opt.ProcessReference(page);
    CpuParams params(frames, f.get_pages_input());
    //
    std::cout <<"\n"<< params.get_frames() << " quadros";
}