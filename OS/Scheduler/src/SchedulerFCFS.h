/*
 * SchedulingAlgo_FCFS.h
 *
 *  Created on: Mar 5, 2015
 *      Author: Kshitiz
 */

#ifndef SCHEDULINGALGO_FCFS_H_
#define SCHEDULINGALGO_FCFS_H_

#include <queue>

#include "Scheduler.h"
#include "Process.h"

using namespace std;

struct FCFSComparator {
public:
	//If Compare(x,y) is true, then x is smaller than y.
	bool operator()(Process& p1, Process& p2) {
		return p1.dynamicArrivalTime > p2.dynamicArrivalTime;
	}
};

class SchedulerFCFS: public Scheduler {
public:
	SchedulerFCFS(bool logVerbose, string inputFilePath, string randFilePath,
			SchedulingAlgo algo) :
			Scheduler(logVerbose, inputFilePath, randFilePath, algo) {
	}

	void addProcess(Process &process) {

		/*
		 int readyWait= time - process.dynamicArrivalTime;
		 if(readyWait>0)
		 {process.totalReadyWait += readyWait;}
		 else//jump in time to process ready to execute
		 {
		 time = process.dynamicArrivalTime;
		 readyWait=0;
		 }

		 if(logVerbose
		 && process.arrivalTime==process.dynamicArrivalTime)
		 { printf("%d %d %d: CREATED ->READY\n" ,time ,process.pid, readyWait); }
		 //todo use of readywait above might be erroneous.
		 */

		readyQueue.push(process);
	}
	void run() {
		while (!readyQueue.empty()) {
			Process process = readyQueue.top();
			readyQueue.pop();

			switch (process.state) {
			case CREATED: {
				CreatedToReady(process);
				break;
			}
			case BLOCKED: {
				BlockedToReady(process);
				break;
			}
			case READY: {
				ReadyToRunning(process);
				if (process.remainingCpuTime > 0) {
					RunningToBlocked(process);
				} else {
					RunningToDone(process);
				}
			}
			}
		}
	}
protected:
	priority_queue<Process, vector<Process>, FCFSComparator> readyQueue;

	void CreatedToReady(Process& process) {
		int wait = time - process.dynamicArrivalTime;
		if (wait < 0) { //jump in time to process ready to execute
			time = process.dynamicArrivalTime;
			wait = 0;
		}
		process.state = READY;
		if (logVerbose) {
			printf("%d %d %d: CREATED ->READY", time, process.pid, wait);
			cout<<endl;
		}
		//todo use of readywait above might be erroneous.
		readyQueue.push(process);
	}

	void BlockedToReady(Process& process) {
		int wait = time - process.dynamicArrivalTime;
		if (wait < 0) //jump in time to process ready to execute
				{
			time = process.dynamicArrivalTime;
			wait = 0;
		}
		process.state = READY;
		if (logVerbose) {
			printf("%d %d %d: BLOCK -> READY", process.dynamicArrivalTime, process.pid,
					process.inLastStateSince);
			cout<<endl;
		}
		readyQueue.push(process);
	}

	void ReadyToRunning(Process& process) {
		int cpuBurst = getBurstTime(process.cpuBurst);
		if (cpuBurst > process.remainingCpuTime) {
			cpuBurst = process.remainingCpuTime;
		}
		int readyWait = time - process.dynamicArrivalTime;
		if (readyWait > 0) {
			process.totalReadyWait += readyWait;
		} else //jump in time to process ready to execute
		{
			time = process.dynamicArrivalTime;
			readyWait = 0;
		}
		process.state = RUNNING;
		if (logVerbose) {
			printf("%d %d %d: READY ->RUNNG cb=%d rem=%d prio=%d", time,
					process.pid, readyWait, cpuBurst, process.remainingCpuTime,
					process.staticPriority);
			cout<<endl;
		}
		time += cpuBurst;
		totalCpuUsed += cpuBurst;
		process.inLastStateSince = cpuBurst;
		process.remainingCpuTime -= cpuBurst;
	}

	void RunningToBlocked(Process& process) {
		int ioBurst = getBurstTime(process.ioBurst);
		process.state = BLOCKED;
		if (logVerbose) {
			printf("%d %d %d: RUNNG ->BLOCK ib=%d rem=%d", time, process.pid,
					process.inLastStateSince, ioBurst,
					process.remainingCpuTime);
			cout<<endl;
		}
		process.totalIOTaken += ioBurst;
		process.inLastStateSince = ioBurst;
		process.dynamicArrivalTime = time + ioBurst;
		if (process.dynamicArrivalTime > iOUsedTill) {
			if (iOUsedTill < time) {
				iOUsedTill = time;
			}
			totalIOUsed += process.dynamicArrivalTime - iOUsedTill;
			iOUsedTill = process.dynamicArrivalTime;
		}
		readyQueue.push(process);
	}

	void RunningToDone(Process& process) {
		process.finishingTime = time;
		process.state = DONE;
		if (logVerbose) {
			printf("%d %d %d: Done", time, process.pid,
					process.inLastStateSince);
			cout<<endl;
		}
		createdQueue.insert(createdQueue.begin() +process.pid, process);
	}
};

#endif /* SCHEDULINGALGO_FCFS_H_ */
