/*
 * LCFS.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef LCFS_H_
#define LCFS_H_

#include <queue>

#include "scheduler.h"
#include "helpers.h"
#include "process.h"

class LCFScheduler: public Scheduler {
public:
	LCFScheduler(InputParams params) :
			Scheduler(params) {
	}

protected:
	vector<Process> readyQueue;

	void printReadyQueue() {
		for (Process p : readyQueue) {
			printf("%lu(%s) -> ", p.pid, Process::StateToString(p.state));
		}
		cout << endl;
	}

	Process getReadyProcess() {
		Process result = readyQueue.back();
		readyQueue.pop_back();

		if (params.isVeryVerbose) {
			printf("\t\tRQ: pid %lu extracted, queue: ", result.pid);
			printReadyQueue();
		}

		return result;
	}

	void putReadyProcess(Process p) {
		readyQueue.push_back(p);
		if (params.isVeryVerbose) {
			printf("\t\tRQ: pid %lu inserted, queue: ", p.pid);
			printReadyQueue();
		}
	}

	Process peekReadyProcess(){
		return readyQueue.back();
	}
};

#endif /* LCFS_H_ */
