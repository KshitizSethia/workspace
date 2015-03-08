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

/*
 struct ComparatorLCFS {
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

class LCFScheduler: public Scheduler {
public:
	LCFScheduler(InputParams params) :
			Scheduler(params) {
	}

protected:
	vector<Process> processingQueue;
	Process getProcess() {
		Process proc = processingQueue[0];
		processingQueue.erase(processingQueue.begin());
		if (params.isVerbose) {
			printf("\t\tpid %d extracted, queue ", proc.pid);
			printQueue();
		}
		return proc;
	}

	void putProcess(Process proc) {
		unsigned insertAt = processingQueue.size();
		for (unsigned index = 0; index < processingQueue.size(); index++) {
			Process other = processingQueue[index];

			if (proc.returnsAt < other.returnsAt) {
				insertAt = index;
				break;
			}
		}
		processingQueue.insert(processingQueue.begin() + insertAt, proc);

		if (params.isVerbose) {
			printf("\t\tpid %d inserted, queue: ", proc.pid);
			printQueue();
		}
		/*processingQueue.push_back(proc);
		 //return: true(p1>p2) false(p2>p1)
		 auto sorter = [](Process p1, Process p2) {
		 if(p1.returnsAt<p2.returnsAt)
		 return true;
		 else if(p1.returnsAt==p2.returnsAt &&p1.state==READY && p2.state==READY)
		 return
		 else
		 return false;
		 };
		 sort(processingQueue.begin(), processingQueue.end(), sorter);*/
	}

	bool empty() {
		return processingQueue.empty();
	}

	void printQueue() {
		for (Process p : processingQueue) {
			printf("%d(%s) -> ", p.pid, Process::StateToString(p.state));
		}
		cout << endl;
	}

	vector<Process> readyQueue;

	void printReadyQueue() {
		for (Process p : readyQueue) {
			printf("%d(%s) -> ", p.pid, Process::StateToString(p.state));
		}
		cout << endl;
	}

	Process getReadyProcess() {
		Process result = readyQueue.back();
		readyQueue.pop_back();

		if (params.isVerbose) {
			printf("\t\tRQ: pid %d extracted, queue: ", result.pid);
			printReadyQueue();
		}

		return result;
	}

	void putReadyProcess(Process p) {
		readyQueue.push_back(p);
		if (params.isVerbose) {
			printf("\t\tRQ: pid %d inserted, queue: ", p.pid);
			printReadyQueue();
		}
	}
};

#endif /* LCFS_H_ */
