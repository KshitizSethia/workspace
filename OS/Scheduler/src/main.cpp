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

using namespace std;

int main(int argc, char* argv[]) {

	InputParams params(argc, argv);

	Scheduler *scheduler;
	switch(params.algo){
	case FCFS: scheduler = new FCFScheduler(params); break;
	case LCFS: scheduler = new LCFScheduler(params); break;
	}

	scheduler->readTasks();
	scheduler->simulate();
	scheduler->printSummary();
}
