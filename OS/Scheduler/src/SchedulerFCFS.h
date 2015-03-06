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

struct FCFSComparator{
public:
	//If Compare(x,y) is true, then x is smaller than y.
	bool operator()(Process& p1, Process& p2){
		return p1.dynamicArrivalTime >p2.dynamicArrivalTime;
	}
};

class SchedulerFCFS: public Scheduler{
public:
	SchedulerFCFS(bool logVerbose,
			string inputFilePath,
			string randFilePath,
			SchedulingAlgo algo)
		:Scheduler(logVerbose, inputFilePath, randFilePath, algo){}

	void addProcess(Process &process){

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
	void run()
	{
		while(!readyQueue.empty())
		{
			Process process = readyQueue.top();
			readyQueue.pop();


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

			int cpuBurst = getBurstTime(process.cpuBurst);
			if(cpuBurst>process.remainingCpuTime)
				{cpuBurst=process.remainingCpuTime;}

			if(logVerbose)
			{ printf("%d %d %d: READY ->RUNNG cb=%d rem=%d prio=%d\n"
					, time, process.pid, readyWait, cpuBurst
					, process.remainingCpuTime, process.staticPriority); }

			time	+= cpuBurst;
			totalCpuUsed += cpuBurst;
			process.remainingCpuTime	-= cpuBurst;


			if(process.remainingCpuTime>0)
			{
				int ioBurst = getBurstTime(process.ioBurst);

				if(logVerbose)
				{ printf("%d %d %d: RUNNG ->BLOCK ib=%d rem=%d\n"
						, time, process.pid, cpuBurst
						, ioBurst, process.remainingCpuTime); }

				process.totalIOTaken += ioBurst;
				process.dynamicArrivalTime = time+ioBurst;

				if(process.dynamicArrivalTime>iOUsedTill)
					{
						if(iOUsedTill<time)
							{iOUsedTill=time;}

						totalIOUsed +=process.dynamicArrivalTime-iOUsedTill;
						iOUsedTill = process.dynamicArrivalTime;
					}

				if(logVerbose)
				{ printf("%d %d %d: BLOCK -> READY\n", time+ioBurst, process.pid, ioBurst); }

				readyQueue.push(process);
			}
			else
			{
				process.finishingTime = time;
				if(logVerbose)
					{ printf("%d %d %d: Done\n", time, process.pid, cpuBurst); }
				createdQueue.push_back(process);
			}
		}
	}
protected:
	priority_queue<Process, vector<Process>, FCFSComparator> readyQueue;
};

#endif /* SCHEDULINGALGO_FCFS_H_ */
