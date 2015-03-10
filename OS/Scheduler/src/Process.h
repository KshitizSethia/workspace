/*
 * processor.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

enum class ProcessState {
	CREATED, READY, RUNNING, BLOCKED, DONE,
};

class Process {
public:
	unsigned long pid, arrivalTime, cpuTime, cpuBurst, ioBurst, staticPrio;
	unsigned long returnsAt, cpuTimeLeft;
	ProcessState state;
	long totalReadyWait, totalIoTaken;
	unsigned long readySince, lastIoBurst, lastCpuBurst,
			cpuBurstRemaining;
	unsigned long finishTime;
	int runtimePrio;
	bool preempted;

	Process(unsigned long pid, unsigned long arrivalTime, unsigned long cpuTime,
			unsigned long cpuBurst, unsigned long ioBurst,
			unsigned long staticPriority) :
			pid(pid), arrivalTime(arrivalTime), cpuTime(cpuTime), cpuBurst(
					cpuBurst), ioBurst(ioBurst), staticPrio(staticPriority), returnsAt(
					arrivalTime), cpuTimeLeft(cpuTime), state(
					ProcessState::CREATED), runtimePrio(staticPrio - 1) {
		preempted=false;
		cpuBurstRemaining=0;
		finishTime=0;
		totalReadyWait = 0;
		totalIoTaken = 0;
		readySince = 0;
		lastIoBurst = 0;
		lastCpuBurst = 0;

	}

	static char* StateToString(ProcessState state) {
		switch (state) {
		case ProcessState::CREATED:
			return "CREATED";
		case ProcessState::READY:
			return "READY";
		case ProcessState::RUNNING:
			return "RUNNG";
		case ProcessState::BLOCKED:
			return "BLOCK";
		case ProcessState::DONE:
			return "Done";
		default: return "";
		}
	}
};

#endif /* PROCESSOR_H_ */
