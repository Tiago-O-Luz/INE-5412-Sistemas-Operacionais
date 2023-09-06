#include "fcfs_scheduler.cc"
// #include "sjf_scheduler.cc"

int main()
{
	File f;
	f.read_file();
	f.print_processes_params();

    Cpu *cpu = new Cpu();

    FCFSScheduler *fcfs_scheduler = new FCFSScheduler(f.get_process_params(), cpu);
    fcfs_scheduler->Run();
    delete fcfs_scheduler;

    // SJFScheduler *sjf_scheduler = new SJFScheduler(f.get_process_params(), cpu);
    // sjf_scheduler->Run();
    // delete sjf_scheduler;


}