/*
 * Process.h
 *
 *  Created on: Mar 5, 2015
 *      Author: Kshitiz
 */

#ifndef PROCESS_H_
#define PROCESS_H_

enum ProcessState{
	CREATED,
	READY,
	RUNNING,
	BLOCKED,
	DONE
};

class Process{
public:
	int pid, arrivalTime, remainingCpuTime, cpuTimeInitial, cpuBurst, ioBurst;
	int staticPriority, dynamicArrivalTime;
	int inLastStateSince, finishingTime, totalIOTaken, totalReadyWait;
	ProcessState state;
	Process(int pid, int arrivalTime, int cpuTime, int cpuBurst, int ioBurst, int staticPriority):
		pid(pid),
		arrivalTime(arrivalTime),
		remainingCpuTime(cpuTime),
		cpuTimeInitial(cpuTime),
		cpuBurst(cpuBurst),
		ioBurst(ioBurst),
		staticPriority(staticPriority),
		dynamicArrivalTime(arrivalTime)
	{
		inLastStateSince=0;
		finishingTime=0;
		totalIOTaken=0;
		totalReadyWait=0;
		state=CREATED;
	}


};


#endif /* PROCESS_H_ */
