

#include<iostream>
#include<stdint.h>
#include <random>
#include "process.h"


#ifndef CPU
#define CPU


class Cpu {
    private:
        int64_t reg[NUMBER_OF_REGISTERS]; // CPU Registers
        int64_t stack_pointer;
        int64_t program_counter;
    
    public:
        // Changes the current process and returns context
        ProcessContext ContextChange(ProcessContext context);

        void RunProcess();
    
};


#endif