

#include "operational_system.h"
#include "fcfs_scheduler.h"
#include "sjf_scheduler.h"
#include "priority_scheduler.h"
#include "preempted_priority_scheduler.h"
#include "round_robin_scheduler.h"

#include <chrono>

int main()
{
    // auto start = chrono::steady_clock::now();
    // Read file
    File f;
    f.read_file();
    f.print_processes_params();

    // Allocates cpu and scheduler methods
    Cpu *cpu = new Cpu();
    FCFSScheduler *fcfs_scheduler = new FCFSScheduler(f.get_process_params());
    SJFScheduler *sjf_scheduler = new SJFScheduler(f.get_process_params());
    PriorityScheduler *priority_scheduler = new PriorityScheduler(f.get_process_params());
    PreemptedPriorityScheduler *pr_priority_scheduler = new PreemptedPriorityScheduler(f.get_process_params());
    RoundRobinScheduler *round_robin_scheduler = new RoundRobinScheduler(f.get_process_params());

    cout << "\nTurnaround Time: TA\nWait time: WT\nContext Changes: CC\n";

    // Creates operational system and runs all methods:
    // FCFS
    OperationalSystem os(fcfs_scheduler, cpu);
    cout << "\nFCFS:\n";
    os.Run();

    // SJF
    os.ChangeSchedulerMethod(sjf_scheduler);
    cout << "\nSJF:\n";
    os.Run();

    // Priority
    os.ChangeSchedulerMethod(priority_scheduler);
    cout << "\nPriority:\n";
    os.Run();

    // Preemted Priority
    os.ChangeSchedulerMethod(pr_priority_scheduler);
    cout << "\nPreemted Priority:\n";
    os.Run();

    // Round Robin
    os.ChangeSchedulerMethod(round_robin_scheduler);
    cout << "\nRound Robin:\n";
    os.Run();

    // auto end = chrono::steady_clock::now();
    // cout << "Elapsed time in microseconds: "
    //      << chrono::duration_cast<chrono::microseconds>(end - start).count()
    //      << " µs" << endl;
}