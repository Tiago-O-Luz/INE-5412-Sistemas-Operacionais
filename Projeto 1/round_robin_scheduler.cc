
#include "round_robin_scheduler.h"
#include <vector>

#define QUANTUM 2;

RoundRobinScheduler::RoundRobinScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters) {

};

RoundRobinScheduler::~RoundRobinScheduler() {}

void RoundRobinScheduler::AddToQueue(Process *process) {
    //
    processes_queue.push_back(process);
    process->SetReadyState();
}

bool RoundRobinScheduler::UpdateQueue() {
    // Scheduler method
    int pid = processes_queue.front()->GetPid();
    // cout << (processes_list[pid]->GetExecutedTime() >= 2) << "\n";
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetDestructionState();
        processes_list[pid]->SetConclusionTime(time_lapsed);
        cout << "process " << pid << " finished" << "\n";
        if (!processes_queue.empty()) return true;
    } else if (processes_list[pid]->GetQuantumTime() >= QUANTUM) {
        cout << "chegou aqui" << pid << "\n";
        processes_queue.push_back(processes_queue.front());

        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetReadyState();
        processes_list[pid]->SetQuantumTime(0);

    } 
    return false;
}

