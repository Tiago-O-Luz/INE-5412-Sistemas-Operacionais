

#include "operational_system.h"


using namespace std;


OperationalSystem::OperationalSystem(Scheduler *scheduler_module, Cpu *c_cpu) {
    scheduler = scheduler_module;
    cpu = c_cpu;
    cout << "Simulation set\n";
}

OperationalSystem::~OperationalSystem() {
    delete scheduler;
    delete cpu;
}

void OperationalSystem::Run() {
    while (true) {   // scheduler->GetTimeLapsed() < 30
        cout << "Current Time: " << scheduler->GetTimeLapsed() << "\n";

        for (auto & process : scheduler->GetProcessesList()) {
            if (process.second->GetCreationTime() == scheduler->GetTimeLapsed()) {
                // If process is created in current timestamp creates (Set to created state) and add to queue
                process.second->SetCreatedState();
                cout << "Process " << process.first << " created at timestamp " << scheduler->GetTimeLapsed() << "s\n";
                scheduler->AddToQueue(process.second);
            }
        }

        if (scheduler->UpdateQueue()) {
            // Process queue was changed 
            scheduler->GetProcessesQueue().front()->SetInExecutionState();
            int pid = scheduler->GetProcessesQueue().front()->GetPid();

            // Change cpu context
            cpu->ContextChange(scheduler->GetProcessControlBLock().GetProcessContext(pid));
            cout << "process " << pid << " started" << "\n";
        }

        if (!scheduler->GetProcessesQueue().empty()) {
            cpu->RunProcess();
            scheduler->GetProcessesQueue().front()->IncreaseExecutedTime();
            scheduler->GetProcessesQueue().front()->IncreaseQuantumTime();
        } else {
            break;
        }
        scheduler->IncreaseTimeLapsed();
    }
}

void OperationalSystem::ChangeSchedulerMethod(Scheduler *scheduler_method) {
    delete scheduler;
    scheduler = scheduler_method;
}