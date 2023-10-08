

#include "scheduler.h"


#ifndef FCFS_SCHEDULER
#define FCFS_SCHEDULER


class FCFSScheduler : public Scheduler 
{
public:

    FCFSScheduler(vector<ProcessParams *> p_parameters);

    ~FCFSScheduler();

    // Specific FCFS add to queue logic
    void doAddToQueue(Process *process) override;

    // Specific FCFS updated queue logic
    bool doUpdateQueue() override;
};


#endif