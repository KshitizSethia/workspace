/*
 * FCFS.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef FCFS_H_
#define FCFS_H_

#include <queue>

#include "scheduler.h"
#include "helpers.h"
#include "process.h"

/*
 struct ComparatorFCFS {
 public:
 //Returns:
 //false: p1 > p2
 //true:  p2 > p1
 bool operator()(Process& p1, Process& p2) {
 if (p1.returnsAt >= p2.returnsAt)
 return true;
 else if (p1.returnsAt == p2.returnsAt)
 if (p1.state == CREATED && p2.state == CREATED && p1.pid > p2.pid)
 return true;
 return false;
 }
 };
 */

class FCFScheduler: public Scheduler {
public:
	FCFScheduler(InputParams params) :
			Scheduler(params) {
	}

protected:
	vector<Process> processingQueue;
	Process getProcess() {
		Process proc = processingQueue[0];
		processingQueue.erase(processingQueue.begin());
		return proc;
	}

	void putProcess(Process proc) {
		processingQueue.push_back(proc);
		auto sorter =
				[](Process p1, Process p2) {return p1.returnsAt<p2.returnsAt;};
		sort(processingQueue.begin(), processingQueue.end(), sorter);
	}

	bool empty() {
		return processingQueue.empty();
	}

	vector<Process> readyQueue;
	Process getReadyProcess() {
		Process result = readyQueue[0];
		readyQueue.erase(readyQueue.begin());
		return result;
	}

	void putReadyProcess(Process p) {
		readyQueue.push_back(p);
	}

};

#endif /* FCFS_H_ */
