

#include "fcfs_scheduler.h"
#include "sjf_scheduler.h"
#include "operational_system.h"
// #include "sjf_scheduler.cc"

int main()
{
    File f;
	f.read_file();
	f.print_processes_params();


    Cpu *cpu = new Cpu();

    FCFSScheduler *fcfs_scheduler = new FCFSScheduler(f.get_process_params());
    SJFScheduler *sjf_scheduler = new SJFScheduler(f.get_process_params());

    OperationalSystem os(fcfs_scheduler, cpu);
    os.Run();
    
    os.ChangeSchedulerMethod(sjf_scheduler);
    os.Run();



}