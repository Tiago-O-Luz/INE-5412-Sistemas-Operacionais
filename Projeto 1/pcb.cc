

#include "pcb.h"


using namespace std;


bool ProcessControlBlock::HasProcessContext(int pid) {
    map<int, ProcessContext>::iterator it;
    it = process_context_dict.find(pid);
    if (it != process_context_dict.end()) {
        return true;
    }
    return false;
}

ProcessContext ProcessControlBlock::GetProcessContext(int pid) {
    return process_context_dict[pid];
}

void ProcessControlBlock::AddProcessContext(int pid, int64_t pc) {
    ProcessContext process_context;
    process_context.program_counter = pc;
    process_context_dict[pid] = process_context;
}

void ProcessControlBlock::SetProcessContext(int pid, ProcessContext context) {
    map<int, ProcessContext>::iterator it;
    it = process_context_dict.find(pid);
    it->second.program_counter = context.program_counter;
    it->second.stack_pointer = context.stack_pointer;
    for (int i=0; i<NUMBER_OF_REGISTERS; i++) {
        it->second.reg[i] = context.reg[i];
    }
}