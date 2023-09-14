

#include "operational_system.h"
#include <iomanip>


using namespace std;


OperationalSystem::OperationalSystem(Scheduler *scheduler_module, Cpu *c_cpu) {
    scheduler = scheduler_module;
    cpu = c_cpu;
}

OperationalSystem::~OperationalSystem() {
    delete scheduler;
    delete cpu;
}

void OperationalSystem::Run() {
    // Prints diagram header
    cout << "tempo     ";
    for (auto & p: scheduler->GetProcessesList()) {
        cout << "P" << p.second->GetPid() << " ";
    }
    cout << "\n";

    while (true) {
        for (auto & process : scheduler->GetProcessesList()) {
            if (process.second->GetCreationTime() == scheduler->GetTimeLapsed()) {
                // If process creation time equals current timestamp creates (Set to created state) and add to queue
                process.second->SetCreatedState();
                scheduler->IncreaseCreatedProcess();
                scheduler->AddToQueue(process.second);
            }
        }
        
        if (scheduler->UpdateQueue()) {
            // Current process needs to change
            scheduler->GetProcessesQueue().front()->SetInExecutionState();
            int pid = scheduler->GetProcessesQueue().front()->GetPid();

            // Change cpu context
            ProcessContext context = cpu->ContextChange(scheduler->GetProcessControlBLock().GetProcessContext(pid));
            scheduler->GetProcessesList()[pid]->IncreaseContextChangesCount();
            scheduler->GetProcessControlBLock().SetProcessContext(pid, context);
        }

        if (!scheduler->GetProcessesQueue().empty()) {
            // Simulates process execution
            cpu->RunProcess();
            scheduler->GetProcessesQueue().front()->IncreaseExecutedTime();
            scheduler->GetProcessesQueue().front()->IncreaseQuantumTime();
        } else {
            if (scheduler->GetCreatedProcess() >= scheduler->GetProcessParams().size()) {
                // Check if all process were already created
                break;
            }
        }
        PrintProcessesStates();     // Prints diagram step
        scheduler->IncreaseTimeLapsed();
    }
    // Prints simulation statistics
    PrintStats();
}

// Changes scheduler algorithm
void OperationalSystem::ChangeSchedulerMethod(Scheduler *scheduler_method) {
    delete scheduler;
    scheduler = scheduler_method;
}

// Print information functions:

void OperationalSystem::PrintProcessesStates() {
    int time = scheduler->GetTimeLapsed();
    stringstream strm;
    strm << " " << time << "-" << " " << (time+1);
    int n = strm.str().length();
    cout <<  strm.str() << string((10-n), ' ');
    for (auto & p: scheduler->GetProcessesList()) {
        switch (p.second->GetState()) {
            case State::Ready:
                cout << "-- " << setw(3);
                break;
            case State::Created:
                cout << "-- " << setw(3);
                break;
            case State::InExecution:
                cout << "## " << setw(3);
                break;
            default:
                cout << "   " << setw(3);
                break;
            }
    }
    cout << "\n";
}

void OperationalSystem::PrintStats() {
    // Calculate and print: turnaround time (TA), wait time (WT), context changes (CC)
    float avg_turnaround = 0;
    float avg_wait_time = 0;

    cout << "PID   TA   WT   CC  \n";

    float turnaround_time;
    float wait_time;
    int context_changes_count;
    for (auto & p: scheduler->GetProcessesList()) {
        turnaround_time = p.second->GetConclusionTime() - p.second->GetCreationTime();
        wait_time = turnaround_time - p.second->GetDuration();
        context_changes_count = p.second->GetContextChangesCount();
        avg_turnaround += turnaround_time;
        avg_wait_time += wait_time;
        cout << setw(3) << p.second->GetPid();
        cout << setw(5) << turnaround_time ;
        cout << setw(5) << wait_time;
        cout << setw(5)  << context_changes_count<< '\n';
    }
    avg_turnaround /= scheduler->GetProcessesList().size();
    avg_wait_time /= scheduler->GetProcessesList().size();
    cout << "Avg  " << setw(5) << avg_turnaround;
    cout << setw(5) << avg_wait_time << '\n';
}