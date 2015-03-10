/*
 * Scheduler.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <fstream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#include "process.h"

using namespace std;

const unsigned _MAX_PRIO = 4;

class Scheduler {
public:
	Scheduler(InputParams params) :
			params(params) {
		randFile.open(params.randFilePath.c_str());
		int temp;
		randFile >> temp; //discard first line
	}

	void readTasks() {
		ifstream file(params.inputFilePath.c_str());
		unsigned long pid = 0;
		while (!file.eof()) {
			unsigned long arrivalTime, cpuTime, cpuBurst, ioBurst;
			if (file >> arrivalTime >> cpuTime >> cpuBurst >> ioBurst) {
				unsigned long staticPriority = getRandomNum(_MAX_PRIO);
				Process process(pid, arrivalTime, cpuTime, cpuBurst, ioBurst,
						staticPriority);
				spareQueue.push_back(process);
				pid++;
			}
		}

		file.close();
	}

	void simulate() {
		for (Process &process : spareQueue) {
			putProcess(process);
			hackArray.push_back(0);
		}
		spareQueue.clear();

		while (!empty()) {
			Process proc = getProcess();

			if (time < proc.returnsAt)
				time = proc.returnsAt;

			int wait = time > proc.returnsAt ? time - proc.arrivalTime : 0;

			switch (proc.state) {
			case ProcessState::CREATED: {
				CreatedToReady(wait, proc);
				break;
			}
			case ProcessState::BLOCKED: {
				BlockedToReady(wait, proc);
				break;
			}
			case ProcessState::READY: {
				assert(wait == 0);
				if (processorBusyTill > proc.returnsAt) { //proccessor is busy
					ReadyPostponed(proc);
				} else { //processor is ready to take process
					proc = getReadyProcess();
					if (!(params.algo == SchedulingAlgo::RR //not RR or PRIO
					|| params.algo == SchedulingAlgo::PRIO)) {
						ReadyToRunning_nopreemption(proc);
					} else { //RR or PRIO
						ReadyToRunning_withPreemption(proc);
					}
				}
				break;
			}
			case ProcessState::RUNNING: {
				if (proc.cpuTimeLeft > 0) {
					if (!(params.algo == SchedulingAlgo::RR // send to blocked
					|| params.algo == SchedulingAlgo::PRIO)
							|| ((params.algo == SchedulingAlgo::RR
									|| params.algo == SchedulingAlgo::PRIO)
									&& !proc.cpuBurstRemaining > 0)) {
						RunningToBlocked(proc);
					} else { //send to ready
						RunningToReady(proc);
					}
				} else {						//(proc.cpuTimeLeft == 0)
					RunningToDone(proc);
				}
				break;
			}

			}

		};
	}

	void printSummary() {
		switch (params.algo) {
		case SchedulingAlgo::FCFS:
			cout << "FCFS";
			break;
		case SchedulingAlgo::LCFS:
			cout << "LCFS";
			break;
		case SchedulingAlgo::SJF:
			cout << "SJF";
			break;
		case SchedulingAlgo::RR:
			cout << "RR " << params.algoQuantum;
			break;
		case SchedulingAlgo::PRIO:
			cout << "PRIO " << params.algoQuantum;
			break;
		}
		cout << endl;

		auto sorter = [](Process p1, Process p2) {return p1.pid<p2.pid;};
		sort(spareQueue.begin(), spareQueue.end(), sorter);

		double totalTurnaroundTime, totalWaitTime;
		for (Process process : spareQueue) {
			unsigned long waitTime = process.finishTime - process.arrivalTime
					- process.cpuTime - process.totalIoTaken;
			printf("%04lu: %lu %lu %lu %lu %lu | %lu %lu %lu %lu", process.pid,
					process.arrivalTime, process.cpuTime, process.cpuBurst,
					process.ioBurst, process.staticPrio, process.finishTime,
					process.finishTime - process.arrivalTime,
					process.totalIoTaken, waitTime);
			//hackArray[process.pid]);//process.totalReadyWait);
			cout << endl;
			totalTurnaroundTime += process.finishTime - process.arrivalTime;
			totalWaitTime += waitTime;//hackArray[process.pid];//process.totalReadyWait;
		}

		unsigned long numProcesses = spareQueue.size();
		printf("SUM: %lu %.2lf %.2lf %.2lf %.2lf %.3lf", time,
				double(totalCpuUsed * 100.0 / time),
				double(totalIoUsed * 100.0 / time),
				double(totalTurnaroundTime / numProcesses),
				double(totalWaitTime / numProcesses),
				double(numProcesses * 100.0 / time));
		cout << endl;

	}
protected:
	InputParams params;
	ifstream randFile;
	vector<Process> spareQueue;
	unsigned long time = 0;
	unsigned long processorBusyTill = 0, ioBusyTill = 0;
	unsigned long totalCpuUsed = 0, totalIoUsed = 0;
	vector<unsigned long> hackArray;

	void printMessage(Process process, int inStateSince,
			ProcessState finalState) {
		if (params.isVerbose) {
			switch (finalState) {
			case ProcessState::DONE: {
				printf("%lu %lu %lu: %s", time, process.pid, inStateSince,
						Process::StateToString(finalState));
				break;
			}
			default: {
				printf("%lu %lu %lu: %s -> %s", time, process.pid, inStateSince,
						Process::StateToString(process.state),
						Process::StateToString(finalState));
				break;
			}
			}

			if (process.state == ProcessState::READY
					&& finalState == ProcessState::RUNNING)
				if (params.algo == SchedulingAlgo::RR
						|| params.algo == SchedulingAlgo::PRIO) {
					printf(" cb=%lu rem=%lu prio=%d", process.cpuBurstRemaining,
							process.cpuTimeLeft, process.runtimePrio);
				} else {
					printf(" cb=%lu rem=%lu prio=%d", process.lastCpuBurst,
							process.cpuTimeLeft, process.runtimePrio);
				}
			else if (process.state == ProcessState::RUNNING) {
				if (finalState == ProcessState::BLOCKED)
					printf(" ib=%lu rem=%lu", process.lastIoBurst,
							process.cpuTimeLeft);
				else if (finalState == ProcessState::READY)
					printf(" cb=%lu rem=%lu prio=%d", process.cpuBurstRemaining,
							process.cpuTimeLeft, process.runtimePrio);
			}
			cout << endl;
		}
	}

	unsigned long getRandomNum(unsigned long upperBound) {
		unsigned long result;
		randFile >> result;
		if (!randFile || randFile.eof()) //last line reached
				{
			randFile.seekg(0); //rewind file
			randFile >> result; //read number of lines, discard
			randFile >> result;
		}
		result = 1 + (result % upperBound);
		return result;
	}

	vector<Process> processingQueue;
	Process getProcess() {
		Process proc = processingQueue[0];
		processingQueue.erase(processingQueue.begin());
		if (params.isVeryVerbose) {
			printf("\t\tpid %lu extracted, queue ", proc.pid);
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

		if (params.isVeryVerbose) {
			printf("\t\tpid %lu inserted, queue: ", proc.pid);
			printQueue();
		}
	}

	bool empty() {
		return processingQueue.empty();
	}

	void printQueue() {
		for (Process p : processingQueue) {
			printf("%lu(%s) -> ", p.pid, Process::StateToString(p.state));
		}
		cout << endl;
	}

	virtual Process getReadyProcess()=0;
	virtual void putReadyProcess(Process)=0;
	virtual Process peekReadyProcess()=0;

	void CreatedToReady(int wait, Process& proc) {
		assert(wait == 0);
		printMessage(proc, wait, ProcessState::READY);
		proc.state = ProcessState::READY;
		proc.readySince = time;
		putProcess(proc);
		putReadyProcess(proc);
	}

	void BlockedToReady(int wait, Process& proc) {
		assert(wait == 0);
		printMessage(proc, proc.lastIoBurst + wait, ProcessState::READY);
		proc.state = ProcessState::READY;
		proc.readySince = time;
		putProcess(proc);
		putReadyProcess(proc);
	}

	void ReadyPostponed(Process& proc) {
		//proccessor is busy
		Process proc2 = peekReadyProcess();
		//proc.totalReadyWait += processorBusyTill - proc.returnsAt;
		hackArray[proc2.pid] += processorBusyTill - proc.returnsAt;
		proc.returnsAt = processorBusyTill;
		if (params.isVeryVerbose) {
			printf(
					"\t\t%lu %lu: Processor busy, postponed to %lu, total wait %lu\n",
					time, proc2.pid, proc.returnsAt, hackArray[proc2.pid]); //proc.totalReadyWait);
		}
		putProcess(proc);
	}

	void ReadyToRunning_nopreemption(Process& proc) {
		proc.lastCpuBurst = min(getRandomNum(proc.cpuBurst), proc.cpuTimeLeft);
		printMessage(proc, time - proc.readySince, ProcessState::RUNNING);
		proc.state = ProcessState::RUNNING;
		proc.returnsAt = time + proc.lastCpuBurst;
		processorBusyTill = proc.returnsAt;
		proc.cpuTimeLeft -= proc.lastCpuBurst;
		totalCpuUsed += proc.lastCpuBurst;
		putProcess(proc);
	}

	void ReadyToRunning_withPreemption(Process& proc) {
		//RR or PRIO
		if (!proc.cpuBurstRemaining > 0) {
			proc.cpuBurstRemaining = min(proc.cpuTimeLeft,
					getRandomNum(proc.cpuBurst));
		}
		proc.lastCpuBurst = min(proc.cpuBurstRemaining, params.algoQuantum);
		printMessage(proc, time - proc.readySince, ProcessState::RUNNING);
		proc.state = ProcessState::RUNNING;
		proc.returnsAt = time + proc.lastCpuBurst;
		processorBusyTill = proc.returnsAt;
		proc.cpuTimeLeft -= proc.lastCpuBurst;
		totalCpuUsed += proc.lastCpuBurst;
		proc.cpuBurstRemaining -= proc.lastCpuBurst;
		putProcess(proc);
	}

	void RunningToBlocked(Process& proc) {
		proc.lastIoBurst = getRandomNum(proc.ioBurst);
		proc.totalIoTaken += proc.lastIoBurst;
		printMessage(proc, proc.lastCpuBurst, ProcessState::BLOCKED);
		proc.state = ProcessState::BLOCKED;
		proc.returnsAt = time + proc.lastIoBurst;
		proc.runtimePrio = proc.staticPrio - 1;

		if (ioBusyTill < time) {
			ioBusyTill = time;
		}
		if (proc.returnsAt > ioBusyTill) {
			totalIoUsed += proc.returnsAt - ioBusyTill;
			ioBusyTill = proc.returnsAt;
		}
		if (params.isVeryVerbose) {
			printf("\t\ttotalIoUsed %lu", totalIoUsed);
			cout << endl;
		}

		putProcess(proc);
	}

	void RunningToReady(Process& proc) {
		//send to ready
		printMessage(proc, proc.lastCpuBurst, ProcessState::READY);
		proc.state = ProcessState::READY;
		proc.readySince = time;
		if (params.algo == SchedulingAlgo::PRIO) {
			proc.runtimePrio--;
		}
		putProcess(proc);
		putReadyProcess(proc);
	}

	void RunningToDone(Process& proc) {
		//(proc.cpuTimeLeft == 0)
		printMessage(proc, proc.lastCpuBurst, ProcessState::DONE);
		proc.state = ProcessState::DONE;
		proc.finishTime = time;
		spareQueue.push_back(proc);
	}
};

#endif /* SCHEDULER_H_ */
