

#include "scheduler.h"


Scheduler::Scheduler(vector<ProcessParams *> p_parameters) {
    time_lapsed = 0;
    highest_pid = 0;
    process_params = p_parameters;

    // Creates process objects
    CreateNewProcesses();
}

Scheduler::~Scheduler() {
    for (auto & process : processes_list) {
        delete process.second;
    }
    processes_list.clear();
    processes_queue.clear();
}

void Scheduler::CreateNewProcesses() {
    for (auto & p : process_params) {    
        highest_pid++;
        Process *process = new Process(highest_pid, p->get_creation_time(), p->get_duration(), p->get_priority());

        process_context_block.AddProcessContext(process->GetPid(), 0);
        processes_list[process->GetPid()] = process;
    }
}

// Add to queue logic (FCFS as default)
void Scheduler::AddToQueue(Process *process) {              // MUDAR AQUI DEPOIS
    processes_queue.push_back(process);
    process->SetReadyState();
}

// Updates the queue using the scheduler method (FCFS as default)
// Returns bool true if process changed
bool Scheduler::UpdateQueue() {                             // MUDAR AQUI DEPOIS
    // Scheduler method
    int pid = processes_queue.front()->GetPid();
    // cout << (processes_list[pid]->GetExecutedTime() >= 2) << "\n";
    if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetDestructionState();
        processes_list[pid]->SetConclusionTime(time_lapsed);
        cout << "process " << pid << " finished" << "\n";
        if (!processes_queue.empty()) return true;
    } else if (processes_list[pid]->GetQuantumTime() >= 2) {
        cout << "chegou aqui" << pid << "\n";
        processes_queue.push_back(processes_queue.front());

        processes_queue.erase(processes_queue.begin());
        processes_list[pid]->SetReadyState();
        processes_list[pid]->SetQuantumTime(0);

    } 
    return false;
}
ProcessControlBlock Scheduler::GetProcessControlBLock() {
    return process_context_block;
}

vector<ProcessParams *> Scheduler::GetProcessParams() {
    return process_params;
}

map<int, Process *> Scheduler::GetProcessesList() {
    return processes_list;
}

vector<Process *> Scheduler::GetProcessesQueue() {
    return processes_queue;
}

int Scheduler::GetTimeLapsed() {
    return time_lapsed;
}

void Scheduler::IncreaseTimeLapsed() {
    time_lapsed++;
}
// void Run() {
//     while (time_lapsed < 30) {
//         cout << "Current Time: " << time_lapsed << "\n";

//         for (auto & process : processes_list) {
//             if (process.second->GetCreationTime() == time_lapsed) {
//                 // If process is created in current timestamp creates (Set to created state) and add to queue
//                 process.second->SetCreatedState();
//                 cout << "Process " << process.first << " created at timestamp " << time_lapsed << "s\n";
//                 AddToQueue(process.second);
//             }
//         }

//         if (UpdateQueue()) {
//             processes_queue.front()->SetInExecutionState();
//             int pid = processes_queue.front()->GetPid();
//             cpu->ContextChange(process_context_block.GetProcessContext(pid));
//             cout << "process " << pid << " started" << "\n";
//         }

//         if (!processes_queue.empty()) {
//             cpu->RunProcess();
//             processes_queue.front()->IncreaseExecutedTime();
//         } else {
//             break;
//         }
//         time_lapsed++;
//     }
// }