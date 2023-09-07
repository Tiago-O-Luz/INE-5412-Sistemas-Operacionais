

#include <cstdint>


#ifndef PROCESS
#define PROCESS


#define NUMBER_OF_REGISTERS 6


struct ProcessContext {
    int64_t reg[NUMBER_OF_REGISTERS];
    int64_t stack_pointer;
    int64_t program_counter;
};


class Process {

    enum class State {
        Created, Ready, InExecution, Blocked, Destruction
    };

    private:
        State state;
        int pid;
        int creation_time;
	    int duration; //seconds
        int conclusion_time;
        int executed_time;
        int quantum_time;
	    int static_priority;
	    int dynamic_priority;
    
    public:
        
        Process(int id, int crt, int d, int p);

        int GetPid();

        int GetCreationTime();

        int GetDuration();

        int GetExecutedTime ();
        
        int GetStaticPriority();

        int GetDynamicPriority();

        State GetState();

        void IncreaseExecutedTime();

        void SetConclusionTime(int c_time);

        void SetCreatedState();

        void SetReadyState();

        void SetInExecutionState();

        void SetBlockedState();

        void SetDestructionState();
};

#endif