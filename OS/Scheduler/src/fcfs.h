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

class FCFScheduler: public Scheduler {
public:
	FCFScheduler(InputParams params) :
			Scheduler(params) {
	}

protected:
	vector<Process> readyQueue;
	virtual Process getReadyProcess() {
		Process result = readyQueue[0];
		readyQueue.erase(readyQueue.begin());
		return result;
	}

	virtual void putReadyProcess(Process p) {
		readyQueue.push_back(p);
	}

	virtual Process peekReadyProcess(){
		return readyQueue[0];
	}
};

#endif /* FCFS_H_ */
