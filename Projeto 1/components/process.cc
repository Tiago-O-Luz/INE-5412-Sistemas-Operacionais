

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
    context_changes_count = 0;
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

int Process::GetConclusionTime () {
    return conclusion_time;
}

int Process::GetStaticPriority() {
    return static_priority;
}

int Process::GetDynamicPriority() {
    return dynamic_priority;
}

int Process::GetQuantumTime() {
    return quantum_time;
}

int Process::GetContextChangesCount() {
    return context_changes_count;
}

State Process::GetState() {
    return state;
}

void Process::IncreaseExecutedTime() {
    executed_time++;
}

void Process::IncreaseContextChangesCount() {
    context_changes_count++;
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

void Process::SetQuantumTime(int quantum) {
    quantum_time = quantum;
}

void Process::IncreaseQuantumTime() {
    quantum_time++;
}