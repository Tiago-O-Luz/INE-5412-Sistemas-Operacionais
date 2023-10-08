

#include <cstdint>


#ifndef PROCESS
#define PROCESS


#define NUMBER_OF_REGISTERS 6


// Process context info
struct ProcessContext {
    int64_t reg[NUMBER_OF_REGISTERS];
    int64_t stack_pointer;
    int64_t program_counter;
};

// Process possible states
enum class State {
    Created, Ready, InExecution, Blocked, Destruction
};

class Process {

    private:
        State state; // Starts at Destruction until creation_time
        int pid;
        int creation_time;
	    int duration; //seconds
        int conclusion_time;
        int executed_time;
        int quantum_time;
	    int static_priority;
	    int dynamic_priority;
        int context_changes_count;
    
    public:
        
        Process(int id, int crt, int d, int p);

        int GetPid();

        int GetCreationTime();

        int GetDuration();

        int GetExecutedTime ();

        int GetConclusionTime();
        
        int GetStaticPriority();

        int GetDynamicPriority();

        int GetQuantumTime();

        int GetContextChangesCount();

        State GetState();

        void IncreaseExecutedTime();

        void IncreaseQuantumTime();

        void IncreaseContextChangesCount();

        void SetConclusionTime(int c_time);

        void SetCreatedState();

        void SetReadyState();

        void SetInExecutionState();

        void SetBlockedState();

        void SetDestructionState();

        void SetQuantumTime(int quantum);
};

#endif