

#include "fcfs_scheduler.h"


FCFSScheduler::FCFSScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters) {

};

FCFSScheduler::~FCFSScheduler() {}

void FCFSScheduler::AddToQueue(Process *process) {
        processes_queue.push_back(process);
        process->SetReadyState();
}

bool FCFSScheduler::UpdateQueue() {
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
