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
    f.read_file();

    CpuParams params(frames, f.get_pages_input());
    std::cout <<"\n"<< params.get_frames() << " quadros";
}