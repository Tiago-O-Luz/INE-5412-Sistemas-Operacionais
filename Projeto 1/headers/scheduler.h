

#include<iostream>
#include<vector>
#include "read_file.cc"
#include "pcb.h"


#ifndef SCHEDULER
#define SCHEDULER


// Implements scheduler logic
class Scheduler
{
    // Implemented by child classes
    virtual void doAddToQueue(Process *process) {}
    virtual bool doUpdateQueue() {}

    protected:
        ProcessControlBlock process_context_block; // Process context
        vector<ProcessParams *> process_params; // Received process data for simulation
        map<int, Process *> processes_list; // Dictionary that stores (pid,process)
        vector<Process *> processes_queue; // Process queue of execution
        int time_lapsed; // Time since the beginning
        int highest_pid; // Used to create pids
        int created_process; // Number of process created until current time 

    public:
        Scheduler(vector<ProcessParams *> p_parameters);

        ~Scheduler();

        // Creates processes to each process_params
        void CreateNewProcesses();
        
        // Adds new process to queue
        void AddToQueue(Process *process);

        // Updates the queue using the scheduler method
        // Returns bool true if changes were made
        bool UpdateQueue();

        ProcessControlBlock GetProcessControlBLock();

        vector<ProcessParams *> GetProcessParams();
        
        map<int, Process *> GetProcessesList();

        vector<Process *> GetProcessesQueue();

        int GetTimeLapsed();

        void IncreaseTimeLapsed();

        int GetCreatedProcess();

        void IncreaseCreatedProcess();
};


#endif