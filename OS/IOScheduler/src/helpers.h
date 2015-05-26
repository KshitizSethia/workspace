/*
 * helpers.h
 *
 *  Created on: Apr 28, 2015
 *      Author: Kshitiz
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string>
using namespace std;

enum class SchedulingAlgo
	:int {
		fifo, sstf, scan, cscan, fscan
};

class IORequest {
public:
	unsigned long id;
	unsigned long arrivalTime;
	unsigned long track;
	unsigned long issueTime, finishTime;
	IORequest(int arr, int track) :
			arrivalTime(arr), track(track) {
		static int idCounter=0;
		id=idCounter++;
	}
};

class InputParams {
public:
	InputParams(int argc, char* argv[]) {
		for (int argNum = 1; argNum < argc; argNum++) {
			string arg(argv[argNum]);
			if (arg.substr(0, 2).compare("-v") == 0) {
				verbose = true;
			} else if (arg.substr(0, 2).compare("-s") == 0) {
				loadAlgo(arg);
			} else {
				inputFilePath = arg;
			}
		}
	}
	bool verbose = false;
	string inputFilePath;
	SchedulingAlgo algo;
private:
	void loadAlgo(const string& arg) {
		string temp = arg.substr(2, 1);
		if (temp == "i")
			algo = SchedulingAlgo::fifo;
		else if (temp == "j")
			algo = SchedulingAlgo::sstf;
		else if (temp == "s")
			algo = SchedulingAlgo::scan;
		else if (temp == "c")
			algo = SchedulingAlgo::cscan;
		else if (temp == "f")
			algo = SchedulingAlgo::fscan;
	}
};

#endif /* HELPERS_H_ */
