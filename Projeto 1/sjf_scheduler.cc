

#include "sjf_scheduler.h"

SJFScheduler::SJFScheduler(vector<ProcessParams *> p_parameters) : Scheduler(p_parameters){};

SJFScheduler::~SJFScheduler() {}

void SJFScheduler::AddToQueue(Process *process)
{
    if (processes_queue.empty()) {
        processes_queue.push_back(process);
        process->SetReadyState();
    } else {
        std::vector<Process *>::iterator it = processes_queue.begin();
        int duration = process->GetDuration();

        // Encontrar a posição correta para inserção
        while (it != processes_queue.end() && (*it)->GetDuration() < duration) {
            ++it;
        }

        // Inserir o processo na posição correta
        processes_queue.insert(it, process);
        process->SetReadyState();

    }
}

bool SJFScheduler::UpdateQueue()
{
    // Scheduler method
    int pid = processes_queue.front()->GetPid();
    cout << (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) << "\n";
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration())
    {
        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetDestructionState();
        processes_list[pid]->SetConclusionTime(time_lapsed);
        cout << "process " << pid << " finished"
             << "\n";
        if (!processes_queue.empty())
            return true;
    }
    return false;
}
