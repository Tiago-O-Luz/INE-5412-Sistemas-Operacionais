

#include "fcfs_scheduler.h"


FCFSScheduler::FCFSScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters) {

};

FCFSScheduler::~FCFSScheduler() {}

void FCFSScheduler::doAddToQueue(Process *process) {
        // Adds to end of queue and set to ready
        processes_queue.push_back(process);
        process->SetReadyState();
}

bool FCFSScheduler::doUpdateQueue() {
    int pid = processes_queue.front()->GetPid();
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
        // Process has finished (executed time = duration)
        processes_queue.erase(processes_queue.begin());         // Remove from queue
        processes_list[pid]->SetDestructionState();             // Set as destructed
        processes_list[pid]->SetConclusionTime(time_lapsed);    // Save conclusion time
        if (!processes_queue.empty()) return true;
    }
    // If first process on queue not in execution
    if (processes_queue.front()->GetState() == State::Ready) return true;
    return false;
}
