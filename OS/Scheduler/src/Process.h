/*
 * processor.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

enum ProcessState {
	CREATED, READY, RUNNING, BLOCKED, DONE
};

class Process {
public:
	int pid, arrivalTime, cpuTime, cpuBurst, ioBurst, staticPrio;
	int runtimePrio, returnsAt, cpuTimeLeft;
	ProcessState state;
	int totalReadyWait = 0, totalIoTaken = 0;
	int readySince = 0, lastIoBurst = 0, lastCpuBurst = 0;
	int finishTime = 0;
	bool isPostponed=false;
	Process(int pid, int arrivalTime, int cpuTime, int cpuBurst, int ioBurst,
			int staticPriority) :
			pid(pid), arrivalTime(arrivalTime), cpuTime(cpuTime), cpuBurst(
					cpuBurst), ioBurst(ioBurst), staticPrio(staticPriority), runtimePrio(
					staticPrio - 1), returnsAt(arrivalTime), cpuTimeLeft(
					cpuTime), state(CREATED) {
	}

	static char* StateToString(ProcessState state) {
		switch (state) {
		case CREATED:
			return "CREATED";
		case READY:
			return "READY";
		case RUNNING:
			return "RUNNG";
		case BLOCKED:
			return "BLOCK";
		case DONE:
			return "Done";
		}
	}
};

#endif /* PROCESSOR_H_ */
