

#include "scheduler.h"


#ifndef SJF_SCHEDULER
#define SJF_SCHEDULER


using namespace std;


class SJFScheduler: public Scheduler {
public:

    SJFScheduler(vector<ProcessParams *> p_parameters);

    ~SJFScheduler();

    // Specific SJF add to queue logic
    void doAddToQueue(Process *process) override;

    // Specific SJF updated queue logic
    bool doUpdateQueue();

};


#endif