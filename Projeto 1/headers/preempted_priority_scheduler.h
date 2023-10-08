

#include "scheduler.h"

#ifndef PREEMPTED_PRIORITY_SCHEDULER
#define PREEMPTED_PRIORITY_SCHEDULER


class PreemptedPriorityScheduler : public Scheduler 
{
public:
    PreemptedPriorityScheduler(vector<ProcessParams *> p_parameters);

    ~PreemptedPriorityScheduler();

    // Specific Preempted Priority add to queue logic
    void doAddToQueue(Process *process);

    // Specific Preempted Priority update queue logic
    bool doUpdateQueue();
};

#endif