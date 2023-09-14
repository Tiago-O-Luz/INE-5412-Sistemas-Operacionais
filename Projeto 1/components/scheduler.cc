

#include "scheduler.h"


Scheduler::Scheduler(vector<ProcessParams *> p_parameters) {
    time_lapsed = 0;
    highest_pid = 0;
    created_process = 0;
    process_params = p_parameters;

    // Creates process objects
    CreateNewProcesses();
}

Scheduler::~Scheduler() {
    // Deletes processes and clear list and queue
    for (auto & process : processes_list) {
        delete process.second;
    }
    processes_list.clear();
    processes_queue.clear();
}

void Scheduler::CreateNewProcesses() {
    for (auto & p : process_params) { 
        // For each process_params creates a process with pid = highest pid   
        highest_pid++;
        Process *process = new Process(highest_pid, p->get_creation_time(), p->get_duration(), p->get_priority());

        // Creates process context
        process_control_block.AddProcessContext(process->GetPid(), 0);

        // Stores process in map
        processes_list[process->GetPid()] = process;
    }
}

// Calls add to queue logic implemented by child class
void Scheduler::AddToQueue(Process *process) {
    doAddToQueue(process);
}

// Calls update queue logic implemented by child class
// Returns bool true if process changed
bool Scheduler::UpdateQueue() {
    if (!processes_queue.empty()) {
        doUpdateQueue();
    }
}

ProcessControlBlock Scheduler::GetProcessControlBLock() {
    return process_control_block;
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

int Scheduler::GetCreatedProcess() {
    return created_process;
}

void Scheduler::IncreaseCreatedProcess() {
    created_process++;
}