

#include "scheduler.h"

#ifndef PRIORITY_SCHEDULER
#define PRIORITY_SCHEDULER


class PriorityScheduler : public Scheduler 
{
public:
    PriorityScheduler(vector<ProcessParams *> p_parameters);

    ~PriorityScheduler();

    void AddToQueue(Process *process);

    bool UpdateQueue();
};

#endif