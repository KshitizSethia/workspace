/*
 * prio.h
 *
 *  Created on: Mar 9, 2015
 *      Author: Kshitiz
 */

#ifndef PRIO_H_
#define PRIO_H_

#include <vector>
using namespace std;

class PrioScheduler: public RRScheduler {

public:
	PrioScheduler(InputParams params) :
			RRScheduler(params) {

	}
	//vector<Process> readyQueue;
	vector<Process> expiredQueue;

	Process getReadyProcess() override {
		if (readyQueue.empty()) {
			readyQueue = vector<Process>(expiredQueue);
			expiredQueue.clear();
			if (params.isVeryVerbose) {
				cout << "\t\tready and expired queues swapped" << endl;
			}
		}

		Process result = readyQueue[0];
		readyQueue.erase(readyQueue.begin());
		if (params.isVeryVerbose) {
			printf("\t\textracted: (%lu,%lu), queue: ", result.runtimePrio,
					result.pid);
			printQueue(readyQueue);
		}
		return result;
	}

	Process peekReadyProcess(){
		if(!readyQueue.empty()){
			return readyQueue[0];
		}
		return expiredQueue[0];
	}

	void putReadyProcess(Process proc) override {
		auto sorter =
				[](Process p1, Process p2) {return p1.runtimePrio>p2.runtimePrio;};

		if (proc.runtimePrio > -1) {
			readyQueue.push_back(proc);
			sort(readyQueue.begin(), readyQueue.end(), sorter);

			if (params.isVeryVerbose) {
				printf("\t\tinserted: (%lu,%lu), queue: ", proc.runtimePrio,
						proc.pid);
				printQueue(readyQueue);
			}
		} else {
			proc.runtimePrio = proc.staticPrio - 1;
			expiredQueue.push_back(proc);
			sort(expiredQueue.begin(), expiredQueue.end(), sorter);

			if (params.isVeryVerbose) {
				printf("\t\tinserted in expired: (%lu,%lu), queue: ",
						proc.runtimePrio, proc.pid);
				printQueue(expiredQueue);
			}
		}
	}

	void printQueue(vector<Process> q) {
		for (Process p : q) {
			printf("(%lu,%lu) -> ", p.runtimePrio, p.pid);
		}
		cout << endl;
	}
};

#endif /* PRIO_H_ */
