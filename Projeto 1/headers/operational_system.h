

#include "scheduler.h"
#include "cpu.h"


#ifndef OPERATIONAL_SYSTEM
#define OPERATIONAL_SYSTEM


// Manages communication of scheduler with cpu and runs simulation
class OperationalSystem {
    private:
        Scheduler* scheduler;   // Used scheduler method
        Cpu *cpu;
    
    public:
        OperationalSystem(Scheduler *scheduler_module, Cpu *c_cpu);

        ~OperationalSystem();

        // Run simulation
        void Run();

        // Changes scheduler method
        void ChangeSchedulerMethod(Scheduler * scheduler_method);

        // Print information functions:
        void PrintProcessesStates();

        void PrintProcessesStats();
};


#endif