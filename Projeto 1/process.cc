

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
        Process(int id, int crt, int d, int p) {
            state = State::Created;
            pid = id;
            creation_time = crt;
            duration = d;
            static_priority = p;
            dynamic_priority = p;
            executed_time = 0;
            quantum_time = 0;
        }

        int GetPid() {
            return pid;
        }

        int GetCreationTime() {
            return creation_time;
        }

        int GetDuration() {
            return duration;
        }
        
        int GetStaticPriority() {
            return static_priority;
        }

        int GetDynamicPriority() {
            return dynamic_priority;
        }

        State GetState() {
            return state;
        }

        void SetConclusionTime(int c_time) {
            conclusion_time = c_time;
        }

        void SetCreatedState() {
            state = State::Created;
        }

        void SetReadyState() {
            state = State::Ready;
        }

        void SetInExecutionState() {
            state = State::InExecution;
        }

        void SetBlockedState() {
            state = State::Blocked;
        }

        void SetDestructionState() {
            state = State::Destruction;
        }
};