

#include "scheduler.h"


#ifndef SJF_SCHEDULER
#define SJF_SCHEDULER


using namespace std;


class SJFScheduler: public Scheduler {
public:

    SJFScheduler(vector<ProcessParams *> p_parameters);

    ~SJFScheduler();

    void AddToQueue(Process *process);

    bool UpdateQueue();

};


#endif