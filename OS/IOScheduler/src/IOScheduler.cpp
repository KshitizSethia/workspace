//============================================================================
// Name        : IOScheduler.cpp
// Author      : Kshitiz Sethia
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "helpers.h"
#include "algo.h"
using namespace std;

Algo* createSchedulingAlgo(InputParams params)
{
	Algo* result;
	switch(params.algo){
	case SchedulingAlgo::fifo: result = new FIFO(params); break;
	case SchedulingAlgo::sstf: result = new SSTF(params); break;
	case SchedulingAlgo::scan: result = new SCAN(params); break;
	case SchedulingAlgo::cscan: result = new CSCAN(params); break;
	case SchedulingAlgo::fscan: result = new FSCAN(params); break;
	}
	return result;
}

int main(int argc, char* argv[]) {

	InputParams params(argc, argv);

	Algo *algo = createSchedulingAlgo(params);

	algo->simulate();
	algo->printSummary();

	return 0;
}
