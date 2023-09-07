

#include "scheduler.h"
#include "cpu.h"


#ifndef OPERATIONAL_SYSTEM
#define OPERATIONAL_SYSTEM


class OperationalSystem {
    private:
        Scheduler* scheduler;
        Cpu *cpu;
    
    public:
        OperationalSystem(Scheduler *scheduler_module, Cpu *c_cpu);

        ~OperationalSystem();

        void Run();

        void ChangeSchedulerMethod(Scheduler * scheduler_method);
};


#endif