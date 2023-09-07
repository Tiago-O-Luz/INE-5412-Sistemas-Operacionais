

#include<iostream>
#include<vector>
#include "read_file.cc"
#include "pcb.h"


#ifndef SCHEDULER
#define SCHEDULER


class Scheduler
{
    protected:
        ProcessControlBlock process_context_block;
        vector<ProcessParams *> process_params;
        map<int, Process *> processes_list; // Dictionary that stores (pid,process)
        vector<Process *> processes_queue; // Process queue of execution
        int time_lapsed; // Time since the beginning
        int highest_pid; // Used to create pids

    public:
        Scheduler(vector<ProcessParams *> p_parameters);

        ~Scheduler();

        // Creates processes to each process_params
        void CreateNewProcesses();
        
        // Adds new process to queue
        void AddToQueue(Process *process);

        // Updates the queue using the scheduler method
        // Returns bool true if process changed
        bool UpdateQueue();

        ProcessControlBlock GetProcessControlBLock();

        vector<ProcessParams *> GetProcessParams();
        
        map<int, Process *> GetProcessesList();

        vector<Process *> GetProcessesQueue();

        int GetTimeLapsed();

        void IncreaseTimeLapsed();
};


#endif