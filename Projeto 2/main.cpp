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
    int i = 0;                      // Lógica que eu tinha montato para a função que retornaria cada ṕagina lida no read_file (NAO ESTA COMPLETO)
    while(i != -1) {
        int page = f.read_file();
        if (page != -1) {
            fifo.ProcessReference(page);
            lru.ProcessReference(page);
        }
        i++;
    }

        opt.ProcessReference(page);
    CpuParams params(frames, f.get_pages_input());
    std::cout <<"\n"<< params.get_frames() << " quadros";
}