/*
 * helpers.h
 *
 *  Created on: Apr 15, 2015
 *      Author: Kshitiz
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string>
#include <iostream>
#include <cstring>

using namespace std;

static const int _NUM_PAGES = 64;

enum class ReplacementAlgo {
	NRU,
	LRU,
	Random,
	FIFO,
	SecondChance,
	ClockPhysical,
	ClockVirtual,
	AgingPhysical,
	AgingVirtual
};

struct PageTableEntry {
	unsigned int present :1, modified :1, referenced :1, pagedout :1;
	unsigned int frameIndex :28; //todo why do we need 28 bits here?

	PageTableEntry() {
		memset(this, 0, sizeof *this);
	}
};

struct SummaryStats {
	unsigned long tot_maps = 0, tot_unmaps = 0, tot_pageins = 0, tot_pageouts =
			0, tot_zero = 0;
};

class InputParams {
public:
	InputParams(int argc, char* argv[]) {
		for (int argNum = 1; argNum < argc; argNum++) {
			string arg(argv[argNum]);
			if (arg.substr(0, 2).compare("-o") == 0) {
				loadOutputFlags(arg);
			} else if (arg.substr(0, 2).compare("-a") == 0) {
				loadAlgo(arg);
			} else if (arg.substr(0, 2).compare("-f") == 0) {
				framesGiven = true;
				string frames_str = arg.substr(2);
				istringstream iss(frames_str);
				iss >> numFrames;
				//todo check if numFrames <64?
			} else if (inputFilePath == "") {
				inputFilePath = arg;
			} else {
				randFilePath = arg;
			}
		}
		if (!algoGiven)
			algo = ReplacementAlgo::LRU;
		if(!framesGiven)
			numFrames=32;
	}
	ReplacementAlgo algo;
	int numFrames;
	string inputFilePath;
	string randFilePath;
	bool printOutput = false;
	bool printPagetable = false;
	bool printPageTableEveryTime = false;
	bool printFrametable = false;
	bool printFrameTableEveryTime = false;
	bool printSummary = false;
private:
	bool algoGiven = false;
	bool framesGiven = false;

	void loadOutputFlags(const string& arg) {
		string flags = arg.substr(2);
		if (flags.find('O') != string::npos) {
			printOutput = true;
		}
		if (flags.find('P') != string::npos) {
			printPagetable = true;
		}
		if (flags.find('F') != string::npos) {
			printFrametable = true;
		}
		if (flags.find('S') != string::npos) {
			printSummary = true;
		}
		if (flags.find('p') != string::npos) {
			printPageTableEveryTime = true;
		}
		if (flags.find('f') != string::npos) {
			printFrameTableEveryTime = true;
		}
	}

	void loadAlgo(const string& arg) {
		algoGiven = true;
		string temp = arg.substr(2, 1);
		if (temp == "N")
			algo = ReplacementAlgo::NRU;
		else if (temp == "l")
			algo = ReplacementAlgo::LRU;
		else if (temp == "r")
			algo = ReplacementAlgo::Random;
		else if (temp == "f")
			algo = ReplacementAlgo::FIFO;
		else if (temp == "s")
			algo = ReplacementAlgo::SecondChance;
		else if (temp == "c")
			algo = ReplacementAlgo::ClockPhysical;
		else if (temp == "a")
			algo = ReplacementAlgo::AgingPhysical;
		else if (temp == "X")
			algo = ReplacementAlgo::ClockVirtual;
		else if (temp == "Y")
			algo = ReplacementAlgo::AgingVirtual;

	}
};

#endif /* HELPERS_H_ */
