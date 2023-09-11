

#include "cpu.h"


ProcessContext Cpu::ContextChange(ProcessContext context) {
    // Saves current context
    ProcessContext current_context;
    current_context.program_counter = program_counter;
    current_context.stack_pointer = stack_pointer;
    for (int i = 0; i < NUMBER_OF_REGISTERS; i++) {
        current_context.reg[i] = reg[i];
    }
    
    // Loads new context
    program_counter = context.program_counter;
    stack_pointer = context.stack_pointer;
    for (int i = 0; i < NUMBER_OF_REGISTERS; i++) {
        reg[i] = context.reg[i];
    }

    return current_context;
}

void Cpu::RunProcess() {
    // Changes context to simulate program running
    program_counter += (rand() % 10 + 1); // Random increase of pc
    stack_pointer += ((rand() % 16 + 1) * 4) - 8*4; // Random increase or decrease of sp
    // Random number for registers
    for (int i = 0; i < NUMBER_OF_REGISTERS; i++) {
        reg[i] = rand();
    }
}

