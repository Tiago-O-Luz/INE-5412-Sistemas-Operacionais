

#include<vector>
#include<map>
#include "process.h"


#ifndef PCB
#define PCB


using namespace std;


// Store and management of process contexts
class ProcessControlBlock {
    private:
        // Maps pid to process context
        map<int, ProcessContext> process_context_dict;
        
    public:
        bool HasProcessContext(int pid);

        ProcessContext GetProcessContext(int pid);

        void AddProcessContext(int pid, int64_t pc);

        void SetProcessContext(int pid, ProcessContext context);

};


#endif