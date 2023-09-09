

#include "fcfs_scheduler.h"
#include "sjf_scheduler.h"
#include "priority_scheduler.h"
#include "operational_system.h"
#include "round_robin_scheduler.h"
// #include "sjf_scheduler.cc"

int main()
{
    File f;
	f.read_file();
	f.print_processes_params();


    Cpu *cpu = new Cpu();

    FCFSScheduler *fcfs_scheduler = new FCFSScheduler(f.get_process_params());
    SJFScheduler *sjf_scheduler = new SJFScheduler(f.get_process_params());
 //   PriorityScheduler *priority_scheduler = new PriorityScheduler(f.get_process_params());

    OperationalSystem os(fcfs_scheduler, cpu);
    os.Run();
    // cout << "sjf:";
    // os.ChangeSchedulerMethod(sjf_scheduler);
    // os.Run();

 //   os.ChangeSchedulerMethod(priority_scheduler);
 //   os.Run();


}