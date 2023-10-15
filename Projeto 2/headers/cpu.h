

#include<iostream>
#include<stdint.h>
#include<vector>


#ifndef CPU
#define CPU

class Cpu {
    private:
        int frames; //nº de quadros
        int pagefaults; //nº de acesso a memória

    public:
        void runCpu();
};


#endif