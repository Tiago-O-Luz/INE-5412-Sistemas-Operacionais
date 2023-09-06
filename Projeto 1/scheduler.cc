#include<iostream>
#include<vector>
#include "read_file.cc"
#include "pcb.cc"
#include "cpu.cc"

using namespace std;

class Scheduler
{
protected:
    Cpu *cpu;
    ProcessControlBlock process_context_block;
    vector<ProcessParams *> process_params;
    map<int, Process *> processes_list; // Dictionary that stores (pid,process)
    vector<Process *> processes_queue; // Process queue of execution
    int time_lapsed; // Time since the beginning
    int highest_pid; // Used to create pids

public:
    Scheduler(vector<ProcessParams *> p_parameters, Cpu *cpu_p) {
        time_lapsed = 0;
        highest_pid = 0;
        process_params = p_parameters;

        cpu = cpu_p;

        // Creates process objects
        CreateNewProcesses();
        // int pid = processes_queue.front()->GetPid();
        // cpu->ContextChange(process_context_block.GetProcessContext(pid));
        // cout << "process " << pid << " started" << "\n";
    }

    ~Scheduler() {
        delete cpu;
        for (auto & process : processes_list) {
            delete process.second;
        }
        processes_list.clear();
        processes_queue.clear();
    }

    void CreateNewProcesses() {
        for (auto & p : process_params) {    
            highest_pid++;
            Process *process = new Process(highest_pid, p->get_creation_time(), p->get_duration(), p->get_priority());

            process_context_block.AddProcessContext(process->GetPid(), 0);
            processes_list[process->GetPid()] = process;
            //processes_queue.push_back(process); // Puts process on queue (Here comes the scheduler method)
        }
    }

    void Run() {
        while (time_lapsed < 30) {
            cout << "Current Time: " << time_lapsed << "\n";

            for (auto & process : processes_list) {
                if (process.second->GetCreationTime() == time_lapsed) {
                    // If process is created in current timestamp creates (Set to created state) and add to queue
                    process.second->SetCreatedState();
                    cout << "Process " << process.first << " created at timestamp " << time_lapsed << "s\n";
                    AddToQueue(process.second);
                }
            }

            if (UpdateQueue()) {
                processes_queue.front()->SetInExecutionState();
                int pid = processes_queue.front()->GetPid();
                cpu->ContextChange(process_context_block.GetProcessContext(pid));
                cout << "process " << pid << " started" << "\n";
            }

            if (!processes_queue.empty()) {
                cpu->RunProcess();
                processes_queue.front()->IncreaseExecutedTime();
            } else {
                break;
            }
            time_lapsed++;
        }
    }
    
    void AddToQueue(Process *process) {
            processes_queue.push_back(process);
            process->SetReadyState();
    }

    // Updates the queue using the scheduler method
    // Returns bool true if process changed
    bool UpdateQueue() {
        // Scheduler method
        int pid = processes_queue.front()->GetPid();
        if (processes_list[pid]->GetExecutedTime() >= processes_list[pid]->GetDuration()) {
            processes_queue.erase(processes_queue.begin());
            processes_list[pid]->SetDestructionState();
            processes_list[pid]->SetConclusionTime(time_lapsed);
            cout << "process " << pid << " finished" << "\n";
            if (!processes_queue.empty()) return true;
        }
        return false;
    }
};