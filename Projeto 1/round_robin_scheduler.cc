

#include "round_robin_scheduler.h"
#include <vector>


RoundRobinScheduler::RoundRobinScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters) {

};

RoundRobinScheduler::~RoundRobinScheduler() {}

void RoundRobinScheduler::doAddToQueue(Process *process) {
    // Adds to end of queue and set to ready
    processes_queue.push_back(process);
    process->SetReadyState();
}

bool RoundRobinScheduler::doUpdateQueue() {
    int pid = processes_queue.front()->GetPid();
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
        // Process has finished (executed time = duration)
        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetDestructionState();
        processes_list[pid]->SetConclusionTime(time_lapsed);
        if (!processes_queue.empty()) return true;
    } else if (processes_list[pid]->GetQuantumTime() >= QUANTUM) {
        // Process has reached quantum max (quantum time >= 2)
        processes_queue.push_back(processes_queue.front());     // Adds to end of queue
        processes_queue.erase(processes_queue.begin());         // Removes from beggining of queue
        processes_list[pid]->SetReadyState();
        processes_list[pid]->SetQuantumTime(0);                 // Resets quantum time
        return true;
    }
    // If first process on queue not in execution
    if (processes_queue.front()->GetState() == State::Ready) return true;
    return false;
}

