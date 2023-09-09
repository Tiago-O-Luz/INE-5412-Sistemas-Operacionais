

#include "scheduler.h"


#ifndef ROUNDROBIN_SCHEDULER
#define ROUNDROBIN_SCHEDULER


using namespace std;


class RoundRobinScheduler: public Scheduler {
public:

    RoundRobinScheduler(vector<ProcessParams *> p_parameters);

    ~RoundRobinScheduler();

    void AddToQueue(Process *process);

    bool UpdateQueue();

};


#endif