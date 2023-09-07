

#include "scheduler.h"


#ifndef FCFS_SCHEDULER
#define FCFS_SCHEDULER


class FCFSScheduler : public Scheduler 
{
public:

    FCFSScheduler(vector<ProcessParams *> p_parameters);

    ~FCFSScheduler();

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

    void AddToQueue(Process *process);

    bool UpdateQueue();
};


#endif