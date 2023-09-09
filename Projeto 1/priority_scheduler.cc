
#include "priority_scheduler.h"
#include <vector>

PriorityScheduler::PriorityScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters) {

};

PriorityScheduler::~PriorityScheduler() {}

void PriorityScheduler::AddToQueue(Process *process) {
    if (processes_queue.empty()) {
        processes_queue.push_back(process);
        process->SetReadyState();
    } else {
        std::vector<Process *>::iterator it = processes_queue.begin();
        int priority = process->GetStaticPriority();

        // Encontrar a posição correta para inserção
        while (it != processes_queue.end() && (*it)->GetStaticPriority() > priority) {
            ++it;
        }

    // Inserir o processo na posição correta
        processes_queue.insert(it, process);
        process->SetReadyState();
    }
}

bool PriorityScheduler::UpdateQueue() {
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

