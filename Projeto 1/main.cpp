#include "scheduler.cc"


int main()
{
	File f;
	f.read_file();
	f.print_processes_params();

    Cpu *cpu = new Cpu();
    Scheduler scheduler(f.get_process_params(), cpu);
    scheduler.Run();
}