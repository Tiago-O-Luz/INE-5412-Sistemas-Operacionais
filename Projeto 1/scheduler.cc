#include<iostream>
#include<vector>
#include "read_file.cc"
#include "pcb.cc"
#include "cpu.cc"

using namespace std;

class Scheduler
{
public:
    Scheduler(vector<ProcessParams *> p_parameters, Cpu *cpu_p) {
        time_lapsed = 0;
        highest_pid = 0;
        process_params = p_parameters;

        cpu = cpu_p;

        // Creates process objects
        CreateNewProcesses();
        int pid = processes_queue.front()->GetPid();
        cpu->ContextChange(process_context_block.GetProcessContext(pid));
        cout << "process " << pid << " started" << "\n";
    }

    void CreateNewProcesses() {
        for (auto & p : process_params) {    
            highest_pid++;
            Process *process = new Process(highest_pid, p->get_creation_time(), p->get_duration(), p->get_priority());

            process_context_block.AddProcessContext(process->GetPid(), 0);
            processes_list[process->GetPid()] = process; // Puts process on queue (Here comes the scheduler method)
            processes_queue.push_back(process); // Puts process on queue (Here comes the scheduler method)
        }
    }

    void Run() {
        while (time_lapsed< 10) {
            cpu->RunProcess();
            time_lapsed++;

            // Change process
            int pid = processes_queue.front()->GetPid();
            processes_queue.erase(processes_queue.begin());
            processes_list[pid]->SetDestructionState();

            cout << "process " << pid << " finished" << "\n";

            processes_queue.front()->SetInExecutionState();
            pid = processes_queue.front()->GetPid();
            cpu->ContextChange(process_context_block.GetProcessContext(pid));
            cout << "process " << pid << " started" << "\n";
        }
    }
    
private:
    Cpu *cpu;
    ProcessControlBlock process_context_block;
    vector<ProcessParams *> process_params;
    map<int, Process *> processes_list; // Dictionary that stores (pid,process)
    vector<Process *> processes_queue; // Process queue of execution
    int time_lapsed; // Time since the beginning
    int highest_pid; // Used to create pids

};
