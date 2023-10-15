#include <stdio.h>  
#include <stdlib.h> 
#include <iostream> 
#include <string>
#include "read_file.cc"

int main(int argc, char*argv[]) {
    std::string input_file;
    int frames = std::stoi(argv[1]);
    std::cin >> input_file;

    File f(input_file);
    while()
        int page = f.read_file();
        fifo.ProcessReference(page);
        lru.ProcessReference(page);
    }

        opt.ProcessReference(page);
    CpuParams params(frames, f.get_pages_input());
    std::cout <<"\n"<< params.get_frames() << " quadros";
}