#include<vector>
#include<map>
#include "process.cc"


using namespace std;


class ProcessControlBlock {
    private:
        // Maps pid to process context
        map<int, ProcessContext> process_context_dict;
        
    public:
        bool HasProcessContext(int pid) {
            map<int, ProcessContext>::iterator it;
            it = process_context_dict.find(pid);
            if (it != process_context_dict.end()) {
                return true;
            }
            return false;
        }

        ProcessContext GetProcessContext(int pid) {
            map<int, ProcessContext>::iterator it;
            it = process_context_dict.find(pid);
            return it->second;
        }

        void AddProcessContext(int pid, int64_t pc) {
            ProcessContext process_context;
            process_context.program_counter = pc;
            // process_context.stack_pointer = sp;
            // for (int i=0; i<NUMBER_OF_REGISTERS; i++) {
            //     process_context.reg[i] = r[i];
            // }
            process_context_dict[pid] = process_context;
        }

        void SetProcessContext(int pid, int64_t r[NUMBER_OF_REGISTERS], int64_t sp, int64_t pc) {
            map<int, ProcessContext>::iterator it;
            it = process_context_dict.find(pid);
            it->second.program_counter = pc;
            it->second.stack_pointer = sp;
            for (int i=0; i<NUMBER_OF_REGISTERS; i++) {
                it->second.reg[i] = r[i];
            }
        }

};