/*
 * SJF.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef SJF_H_
#define SJF_H_

#include <queue>
#include <algorithm>

#include "scheduler.h"
#include "helpers.h"
#include "process.h"

class SJFScheduler: public Scheduler {
public:
	SJFScheduler(InputParams params) :
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
		Process result = readyQueue[0];
		readyQueue.erase(readyQueue.begin());

		if (params.isVeryVerbose) {
			printf("\t\tRQ: pid %lu extracted, queue: ", result.pid);
			printReadyQueue();
		}

		return result;
	}

	Process peekReadyProcess(){
		return readyQueue[0];
	}

	void putReadyProcess(Process p) {
		readyQueue.push_back(p);

		auto sorter = [](Process p1, Process p2){return p1.cpuTimeLeft<p2.cpuTimeLeft;};
		sort(readyQueue.begin(), readyQueue.end(), sorter);

		if (params.isVeryVerbose) {
			printf("\t\tRQ: pid %lu inserted, queue: ", p.pid);
			printReadyQueue();
		}
	}
};

#endif /* SJF_H_ */
