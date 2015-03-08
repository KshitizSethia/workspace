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
		int pid = 0;
		while (!file.eof()) {
			int arrivalTime, cpuTime, cpuBurst, ioBurst;
			if (file >> arrivalTime >> cpuTime >> cpuBurst >> ioBurst) {
				int staticPriority = getRandomNum(_MAX_PRIO);
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
		}
		spareQueue.clear();

		while (!empty()) {
			Process proc = getProcess();

			if (time < proc.returnsAt)
				time = proc.returnsAt;

			int wait = time > proc.returnsAt ? time - proc.arrivalTime : 0;

			switch (proc.state) {
			case CREATED: {
				assert(wait == 0);
				printMessage(proc, wait, READY);
				proc.state = READY;
				proc.readySince = time;
				putProcess(proc);
				putReadyProcess(proc);
				break;
			}
			case BLOCKED: {
				assert(wait == 0);
				printMessage(proc, proc.lastIoBurst + wait, READY);
				proc.state = READY;
				proc.readySince = time;
				putProcess(proc);
				putReadyProcess(proc);
				break;
			}
			case READY: {
				assert(wait == 0);
				if (processorBusyTill > proc.returnsAt) {
					proc.totalReadyWait += processorBusyTill - proc.returnsAt;
					proc.returnsAt = processorBusyTill;
					proc.isPostponed = true;
					if (params.isVerbose) {
						printf("\t\t%d %d: Processor busy, postponed to %d\n",
								time, proc.pid, proc.returnsAt);
					}
					putProcess(proc);
				} else {
					proc = getReadyProcess();
					proc.isPostponed = false;
					proc.lastCpuBurst = getRandomNum(proc.cpuBurst);
					proc.lastCpuBurst =
							proc.lastCpuBurst > proc.cpuTimeLeft ?
									proc.cpuTimeLeft : proc.lastCpuBurst;
					printMessage(proc, time - proc.readySince, RUNNING);
					proc.state = RUNNING;
					proc.returnsAt = time + proc.lastCpuBurst;
					processorBusyTill = proc.returnsAt;
					proc.cpuTimeLeft -= proc.lastCpuBurst;
					totalCpuUsed += proc.lastCpuBurst;
					putProcess(proc);
				}
				break;
			}
			case RUNNING: {
				if (proc.cpuTimeLeft > 0) {
					proc.lastIoBurst = getRandomNum(proc.ioBurst);
					proc.totalIoTaken += proc.lastIoBurst;
					printMessage(proc, proc.lastCpuBurst, BLOCKED);
					proc.state = BLOCKED;
					proc.returnsAt = time + proc.lastIoBurst;

					if (ioBusyTill < time) {
						totalIoUsed += proc.lastIoBurst;
					} else {
						totalIoUsed += proc.returnsAt - ioBusyTill;
					}
					ioBusyTill = proc.returnsAt;

					putProcess(proc);
				} else {
					printMessage(proc, proc.lastCpuBurst, DONE);
					proc.state = DONE;
					proc.finishTime = time;
					spareQueue.push_back(proc);
				}
				break;
			}

			}

		};
	}

	void printSummary() {
		switch (params.algo) {
		case FCFS:
			cout << "FCFS";
			break;
		case LCFS:
			cout << "LCFS";
			break;
		}
		cout << endl;

		auto sorter = [](Process p1, Process p2) {return p1.pid<p2.pid;};
		sort(spareQueue.begin(), spareQueue.end(), sorter);

		double totalTurnaroundTime, totalWaitTime;
		for (Process process : spareQueue) {
			printf("%04d: %d %d %d %d %d | %d %d %d %d", process.pid,
					process.arrivalTime, process.cpuTime, process.cpuBurst,
					process.ioBurst, process.staticPrio, process.finishTime,
					process.finishTime - process.arrivalTime,
					process.totalIoTaken, process.totalReadyWait);
			cout << endl;
			totalTurnaroundTime += process.finishTime - process.arrivalTime;
			totalWaitTime += process.totalReadyWait;
		}

		int numProcesses = spareQueue.size();
		printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf", time,
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
	unsigned time = 0;
	unsigned processorBusyTill = 0, ioBusyTill = 0;
	unsigned totalCpuUsed = 0, totalIoUsed = 0;

	void printMessage(Process process, int inStateSince,
			ProcessState finalState) {
		if (params.isVerbose) {
			switch (finalState) {
			case DONE: {
				printf("%d %d %d: %s", time, process.pid, inStateSince,
						Process::StateToString(finalState));
				break;
			}
			default: {
				printf("%d %d %d: %s -> %s", time, process.pid, inStateSince,
						Process::StateToString(process.state),
						Process::StateToString(finalState));
				break;
			}
			}

			if (process.state == READY && finalState == RUNNING)
				printf(" cb=%d rem=%d prio=%d", process.lastCpuBurst,
						process.cpuTimeLeft, process.runtimePrio);
			else if (process.state == RUNNING && finalState == BLOCKED)
				printf(" ib=%d rem=%d", process.lastIoBurst,
						process.cpuTimeLeft);

			cout << endl;
		}
	}

	long getRandomNum(int upperBound) {
		long result;
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

	virtual Process getProcess()=0;
	virtual void putProcess(Process)=0;
	virtual bool empty()=0;
	virtual Process getReadyProcess()=0;
	virtual void putReadyProcess(Process)=0;
};

#endif /* SCHEDULER_H_ */
