

#include "process.h"


Process::Process(int id, int crt, int d, int p) {
    state = State::Destruction;
    pid = id;
    creation_time = crt;
    duration = d;
    static_priority = p;
    dynamic_priority = p;
    executed_time = 0;
    quantum_time = 0;
}

int Process::GetPid() {
    return pid;
}

int Process::GetCreationTime() {
    return creation_time;
}

int Process::GetDuration() {
    return duration;
}

int Process::GetExecutedTime () {
    return executed_time;
}

int Process::GetStaticPriority() {
    return static_priority;
}

int Process::GetDynamicPriority() {
    return dynamic_priority;
}

Process::State Process::GetState() {
    return state;
}

void Process::IncreaseExecutedTime() {
    executed_time++;
}

void Process::SetConclusionTime(int c_time) {
    conclusion_time = c_time;
}

void Process::SetCreatedState() {
    state = State::Created;
}

void Process::SetReadyState() {
    state = State::Ready;
}

void Process::SetInExecutionState() {
    state = State::InExecution;
}

void Process::SetBlockedState() {
    state = State::Blocked;
}

void Process::SetDestructionState() {
    state = State::Destruction;
}
