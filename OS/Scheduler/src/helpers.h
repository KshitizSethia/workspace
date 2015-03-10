/*
 * helpers.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Kshitiz
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string>
#include <cstdlib>

using namespace std;

enum class SchedulingAlgo
	:int {
		FCFS = 0x01, LCFS = 0x02, SJF = 0x04, RR = 0x08, PRIO = 0x10
};

/*
 inline SchedulingAlgo operator|(SchedulingAlgo a1, SchedulingAlgo a2) {
 return (SchedulingAlgo) (int(a1) | int(a2));
 }
 */

class InputParams {
public:
	InputParams(int argc, char* argv[]) {
		for (int argNum = 1; argNum < argc; argNum++) {
			string arg(argv[argNum]);
			if (arg.compare("-v") == 0) {
				isVerbose = true;
			} else if (arg.compare("-vv") == 0) {
				isVerbose=true;
				isVeryVerbose=true;
			} else if (arg.substr(0, 2).compare("-s") == 0) {

				string temp = arg.substr(2, 1);
				if (temp == "F")
					algo = SchedulingAlgo::FCFS;
				else if (temp == "L")
					algo = SchedulingAlgo::LCFS;
				else if (temp == "S")
					algo = SchedulingAlgo::SJF;
				else if (temp == "R") {
					algo = SchedulingAlgo::RR;
					algoQuantum = atoi(arg.substr(3).c_str());
				} else if (temp == "P") {
					algo = SchedulingAlgo::PRIO;
					algoQuantum = atoi(arg.substr(3).c_str());
				}
			} else if (inputFilePath == "") {
				inputFilePath = arg;
			} else {
				randFilePath = arg;
			}
		}
	}
	SchedulingAlgo algo;
	unsigned long algoQuantum;
	string inputFilePath;
	string randFilePath;
	bool isVerbose = false;
	bool isVeryVerbose=false;
};

#endif /* HELPERS_H_ */
