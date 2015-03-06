//============================================================================
// Name        : Scheduler.cpp
// Author      : Kshitiz Sethia
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "Scheduler.h"
#include "SchedulerFCFS.h"

using namespace std;

/*
void LoadParams(int argc
				, char* argv[]
				, bool &logVerbose
				, SchedulingAlgo& algo
				, string &inputFilePath
				, string &randFilePath) {
	for (int argNum = 1; argNum < argc; argNum++) {
		string arg(argv[argNum]);
		if (arg.compare("-v") == 0) {
			logVerbose = true;
		} else if (arg.substr(0, 2).compare("-s") == 0) {
			string temp = arg.substr(2);
			cout << "Scheduling algo: " << temp << endl;
			//todo put all scheduling algos here
			if (temp == "F")
				algo = FCFS;
		} else if (inputFilePath == "") {
			inputFilePath = arg;
		} else {
			randFilePath = arg;
			randFile.open(arg.c_str());
			 int temp; randFile>>temp; //discard number of entries in file
		}
	}
}
*/

int main(int argc, char *argv[]) {

	bool logVerbose=false;
	SchedulingAlgo algo;
	string inputFilePath, randFilePath;
	//todo uncomment next line
	//LoadParams(argc, argv, logVerbose, algo, inputFilePath, randFilePath);

	logVerbose=true;
	algo=FCFS;
	inputFilePath="C:\\Cloud\\Dropbox\\NYU\\Courses\\OS\\lab2_assign\\input2";
	randFilePath = "C:\\Cloud\\Dropbox\\NYU\\Courses\\OS\\lab2_assign\\rfile";

	Scheduler *scheduler;
	switch(algo)
	{
	case FCFS: scheduler = new SchedulerFCFS(logVerbose, inputFilePath, randFilePath, algo); break;
	}

	scheduler->readTasks();
	scheduler->execute();

/*
	cout<<endl;
	double a,b;
	a = 1.0/3.0;
	b = 2.0/3.0;
	printf("%.2lf %.2lf\n", a, b);
	printf("%.3lf %.3lf\n", a, b);
*/

	return 0;
}
