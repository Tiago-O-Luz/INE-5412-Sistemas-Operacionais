
#ifndef Scheduler
    #include "scheduler.cc"
#endif

using namespace std;

class SJFScheduler: public Scheduler {
public:

    SJFScheduler(vector<ProcessParams *> p_parameters, Cpu *cpu_p) : Scheduler(p_parameters, cpu_p) {};

    ~SJFScheduler() {}

    void Run() {
        while (true) {
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
        vector<Process *>::iterator it = processes_queue.begin();
        advance(it, 1);
        
        for (; it != processes_queue.end(); it++) {
            if (process->GetStaticPriority() < (*it)->GetStaticPriority() || it == processes_queue.end()) {
                processes_queue.insert(it, process);
            }
        }
    }

    bool UpdateQueue() {
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

};