

#include "scheduler.h"

#define QUANTUM 2

#ifndef ROUNDROBIN_SCHEDULER
#define ROUNDROBIN_SCHEDULER


using namespace std;


class RoundRobinScheduler: public Scheduler {
public:

    RoundRobinScheduler(vector<ProcessParams *> p_parameters);

    ~RoundRobinScheduler();

    // Specific Round Robin add to queue logic
    void doAddToQueue(Process *process);

    // Specific Round Robin update queue logic
    bool doUpdateQueue();

};


#endif