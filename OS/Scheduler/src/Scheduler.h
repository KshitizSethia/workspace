#ifndef __Scheduler_h_
#define __Scheduler_h_

#include <string>
#include <fstream>
#include <vector>

#include "Process.h"

using namespace std;

enum SchedulingAlgo
{
	FCFS
};

class Scheduler{

public:
	Scheduler(bool logVerbose,
				string inputFilePath,
				string randFilePath,
				SchedulingAlgo algo):
					logVerbose(logVerbose),
					inputFilePath(inputFilePath),
					algo(algo)
	{
		randFile.open(randFilePath.c_str());
		long temp;randFile>>temp;

		time=0;
		totalCpuUsed=0;
		totalIOUsed=0;
		iOUsedTill=0;
	}

	void readTasks()
	{
		ifstream file(inputFilePath.c_str());
		int pid=0;
		while(!file.eof())
		{
			int arrivalTime, cpuTime, cpuBurst, ioBurst;
			if(file>> arrivalTime >> cpuTime >> cpuBurst >> ioBurst)
			{
				int staticPriority = getPriority();
				Process process(pid,arrivalTime, cpuTime, cpuBurst, ioBurst, staticPriority);
				createdQueue.push_back(process);
				pid++;
			}
		}

		file.close();
	}

	void execute()
	{
		for(auto & element : createdQueue)
		{
			addProcess(element);
		}
		int numProcesses = createdQueue.size();
		createdQueue.clear();
		createdQueue.reserve(numProcesses);

		run();

		switch(algo)
		{
			case FCFS: cout<<"FCFS"<<endl; break;
		}

		printSummary();
	}

	~Scheduler()
	{
		randFile.close();
	}
protected:
	long getRandomNum()
	{
		long result;
		randFile>>result;
		if(!randFile || randFile.eof())//last line reached
		{
			randFile.seekg(0);//rewind file
			randFile>>result;//read number of lines, discard
			randFile>>result;
		}
		return result;
	}

	int getBurstTime(int maxBurst)
	{
		int result = 1+(getRandomNum()%maxBurst);
		return result;
	}

	int time;
	int totalCpuUsed, totalIOUsed, iOUsedTill;

	virtual void addProcess(Process &process)=0;
	virtual void run()=0;

	bool logVerbose = false;
	string inputFilePath;
	SchedulingAlgo algo;
	ifstream randFile;

	vector<Process> createdQueue;

	int getPriority()
	{ return (getRandomNum()%4); }

private:
	void printSummary() {
		double totalTurnaroundTime, totalWaitTime;
		for (auto& process : createdQueue) {
			//todo print priority as well
			printf("%04d: %d %d %d %d %d | %d %d %d %d", process.pid,
					process.arrivalTime, process.cpuTimeInitial,
					process.cpuBurst, process.ioBurst, process.staticPriority,
					process.finishingTime,
					process.finishingTime - process.arrivalTime,
					process.totalIOTaken, process.totalReadyWait);
			cout<<endl;
			totalTurnaroundTime += process.finishingTime - process.arrivalTime;
			totalWaitTime += process.totalReadyWait;
		}
		int numProcesses = createdQueue.size();
		printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf",
				time,
				double(totalCpuUsed * 100.0 / time),
				double(totalIOUsed * 100.0 / time),
				double(totalTurnaroundTime / numProcesses),
				double(totalWaitTime / numProcesses),
				double(numProcesses * 100.0 / time));
		cout<<endl;
	}
};

#endif //__Scheduler_h_
