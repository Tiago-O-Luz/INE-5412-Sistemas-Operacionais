#include <stdio.h>  
#include <stdlib.h> 
#include <iostream> 
#include <string>

int main(int argc, char*argv[]) {
    std::string input_file;
    int frames = std::stoi(argv[1]);
    std::cin >> input_file;
    std::cout << frames << input_file;
}