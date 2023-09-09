# BIN = ./bin
HEADERS = ./headers

all:
	g++ main.cpp cpu.cc operational_system.cc pcb.cc process.cc scheduler.cc sjf_scheduler.cc fcfs_scheduler.cc priority_scheduler.cc round_robin_scheduler.cc -I $(HEADERS) -o app
run:
	./app
clean:
	rm ./app