/*
 * helpers.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string>

using namespace std;

enum SchedulingAlgo {
	FCFS, LCFS, RR, PRIO
};

class InputParams {
public:
	InputParams(int argc, char* argv[]) {
		for (int argNum = 1; argNum < argc; argNum++) {
			string arg(argv[argNum]);
			if (arg.compare("-v") == 0) {
				isVerbose = true;
			} else if (arg.substr(0, 2).compare("-s") == 0) {
				string temp = arg.substr(2);
				//todo put all scheduling algos here
				if (temp == "F")
					algo = FCFS;
				else if (temp == "L")
					algo = LCFS;
			} else if (inputFilePath == "") {
				inputFilePath = arg;
			} else {
				randFilePath = arg;
			}
		}
	}
	SchedulingAlgo algo;
	int algoParam;
	string inputFilePath;
	string randFilePath;
	bool isVerbose = false;
};

#endif /* HELPERS_H_ */
