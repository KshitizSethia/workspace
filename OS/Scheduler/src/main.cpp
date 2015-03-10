//============================================================================
// Name        : Scheduler.cpp
// Author      : Kshitiz Sethia
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include "helpers.h"
#include "fcfs.h"
#include "lcfs.h"
#include "sjf.h"
#include "rr.h"
#include "prio.h"

using namespace std;

int main(int argc, char* argv[]) {

	InputParams params(argc, argv);

	Scheduler *scheduler;
	switch(params.algo){
	case SchedulingAlgo::FCFS: 	scheduler = new FCFScheduler(params); break;
	case SchedulingAlgo::LCFS: 	scheduler = new LCFScheduler(params); break;
	case SchedulingAlgo::SJF: 	scheduler = new SJFScheduler(params); break;
	case SchedulingAlgo::RR:	scheduler = new RRScheduler(params); break;
	case SchedulingAlgo::PRIO:	scheduler = new PrioScheduler(params); break;
	}

	scheduler->readTasks();
	scheduler->simulate();
	scheduler->printSummary();
}
