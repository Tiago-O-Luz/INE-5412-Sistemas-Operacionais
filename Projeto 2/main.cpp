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

    File f(input_file);
    int page = f.read_file();

    while(page != -48) {
        // fifo.ProcessReference(page);
        // lru.ProcessReference(page);
        std::cout << page << " página" << "\n";
        page = f.read_file();
    }

        // opt.ProcessReference(page);
    CpuParams params(frames, f.get_pages_input());
    std::cout <<"\n"<< params.get_frames() << " quadros";
}