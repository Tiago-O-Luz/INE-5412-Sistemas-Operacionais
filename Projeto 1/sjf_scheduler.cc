

#include "sjf_scheduler.h"

SJFScheduler::SJFScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters){};

SJFScheduler::~SJFScheduler() {}

void SJFScheduler::doAddToQueue(Process *process)
{
    if (processes_queue.empty()) {
        processes_queue.push_back(process);
        process->SetReadyState();
    } else {
        std::vector<Process *>::iterator it = processes_queue.begin();
        int duration = process->GetDuration();

        // Find correct position to insert process
        while (it != processes_queue.end() && (*it)->GetDuration() < duration) {
            ++it;
        }

        // If is shortest job sets at second position (Prevents preemption before current process finishes)
        if (it == processes_queue.begin() && processes_queue.front()->GetState() == State::InExecution) it++;
        processes_queue.insert(it, process);
        process->SetReadyState();
    }
}

bool SJFScheduler::doUpdateQueue()
{
    int pid = processes_queue.front()->GetPid();
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
        // Process has finished (executed time = duration)
        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetDestructionState();
        processes_list[pid]->SetConclusionTime(time_lapsed);
        if (!processes_queue.empty()) return true;
    }
    // If first process on queue not in execution
    if (processes_queue.front()->GetState() == State::Ready) return true;
    return false;
}
