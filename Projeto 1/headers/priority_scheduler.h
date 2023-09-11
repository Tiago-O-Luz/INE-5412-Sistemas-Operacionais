

#include "scheduler.h"

#ifndef PRIORITY_SCHEDULER
#define PRIORITY_SCHEDULER


class PriorityScheduler : public Scheduler 
{
public:
    PriorityScheduler(vector<ProcessParams *> p_parameters);

    ~PriorityScheduler();

    // Specific Priority add to queue logic
    void doAddToQueue(Process *process);

    // Specific Priority update queue logic
    bool doUpdateQueue();
};

#endif