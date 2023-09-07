

#include "sjf_scheduler.h"


SJFScheduler::SJFScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters) {};

SJFScheduler::~SJFScheduler() {}

void SJFScheduler::AddToQueue(Process *process) {
    vector<Process *>::iterator it = processes_queue.begin();
    advance(it, 1);
    
    for (; it != processes_queue.end(); it++) {
        if (process->GetDuration() < (*it)->GetDuration() || it == processes_queue.end()) {
            processes_queue.insert(it, process);
        }
    }
}

bool SJFScheduler::UpdateQueue() {
    // Scheduler method
    int pid = processes_queue.front()->GetPid();
    cout << (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) << "\n";
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetDestructionState();
        processes_list[pid]->SetConclusionTime(time_lapsed);
        cout << "process " << pid << " finished" << "\n";
        if (!processes_queue.empty()) return true;
    }
    return false;
}

