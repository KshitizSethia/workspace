//============================================================================
// Name        : MemoryMgr.cpp
// Author      : Kshitiz Sethia
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "helpers.h"
#include "Replacers.h"

using namespace std;

void updateOldFrame(vector<int> &frameTable,
		vector<PageTableEntry> &virtualPageTable, const InputParams& params,
		int frameToBeReplaced, int time, SummaryStats &summary) {
	//manage old frame
	int oldPageIndex = frameTable[frameToBeReplaced];
	PageTableEntry& old_pte = virtualPageTable[oldPageIndex];
	//unmap
	summary.tot_unmaps++;
	if (params.printOutput) {
		printf("%d: UNMAP%4d%4d\n", time, oldPageIndex, frameToBeReplaced);
	}
	old_pte.present = 0;
	old_pte.referenced = 0;
	if (old_pte.modified) {
		//page out
		summary.tot_pageouts++;
		if (params.printOutput)
			printf("%d: OUT  %4d%4d\n", time, oldPageIndex, frameToBeReplaced);

		old_pte.modified = 0;
		old_pte.pagedout = 1;
	}
}

void bringInNewPage(const InputParams& params, const int time,
		const unsigned int allocatedFrameNumber, vector<int> &frameTable,
		PageTableEntry& pte, const int pageNumber, SummaryStats &summary,
		bool replacementNotDone, vector<PageTableEntry> &virtualPageTable) {
	//page in
	pte.present = 1;
	if (pte.pagedout) {
		summary.tot_pageins++;
		if (params.printOutput) {
			printf("%d: IN   %4d%4d\n", time, pageNumber, allocatedFrameNumber);
		}
	} else {
		summary.tot_zero++;
		if (params.printOutput) {
			printf("%d: ZERO     %4d\n", time, allocatedFrameNumber);
		}
	}
	//map
	frameTable[allocatedFrameNumber] = pageNumber;
	pte.frameIndex = allocatedFrameNumber;

	/*	//if LRU, move this page to end
	 if (!replacementNotDone && params.algo == ReplacementAlgo::LRU) {
	 frameTable.erase(frameTable.begin() + allocatedFrameNumber);
	 frameTable.push_back(pageNumber);
	 pte.frameIndex = frameTable.size() - 1;

	 restoreFrameIndicesInPageTable(frameTable, virtualPageTable);
	 }*/

	summary.tot_maps++;
	if (params.printOutput)
		printf("%d: MAP  %4d%4d\n", time, pageNumber, allocatedFrameNumber);
}

void allocateFrameToPage(const InputParams& params, vector<int> &frameTable,
		vector<PageTableEntry> &virtualPageTable, int time, int pageNumber,
		PageTableEntry& pte, int& used_frames, ReplacementPolicy* replacer,
		SummaryStats &summary) {
	unsigned int allocatedFrameNumber;
	if (pte.present) {
		//rearrange frameTable if LRU
		if (params.algo == ReplacementAlgo::LRU) {
			int currentlyUsedFrameIndex = pte.frameIndex;

			LRU *thisReplacer = dynamic_cast<LRU *>(replacer);
			vector<int> &frameIndices = thisReplacer->frameIndices;
			auto indexInFrameIndices = find(frameIndices.begin(),
					frameIndices.end(), currentlyUsedFrameIndex);
			frameIndices.erase(indexInFrameIndices);
			frameIndices.push_back(currentlyUsedFrameIndex);
			/*int pageIndex = frameTable[currentlyUsedFrameIndex];
			 frameTable.erase(frameTable.begin() + currentlyUsedFrameIndex);
			 frameTable.push_back(pageIndex);

			 restoreFrameIndicesInPageTable(frameTable, virtualPageTable);*/
			thisReplacer->printIndices();
		}
	} else {
		bool replacementNotDone = false;
		//if frame table full, replace page
		if (used_frames == params.numFrames) {
			//replace old frame
			int frameToBeReplaced = replacer->getFrameToBeReplaced(frameTable,
					params.numFrames, virtualPageTable);
			updateOldFrame(frameTable, virtualPageTable, params,
					frameToBeReplaced, time, summary);
			//put in new frame
			allocatedFrameNumber = frameToBeReplaced;
		} else {
			//put in new frame
			allocatedFrameNumber = used_frames;
			used_frames++;
			replacementNotDone = true;
			if (params.algo == ReplacementAlgo::LRU) {
				LRU *thisReplacer = dynamic_cast<LRU *>(replacer);
				thisReplacer->frameIndices.push_back(allocatedFrameNumber);
			}
		}
		bringInNewPage(params, time, allocatedFrameNumber, frameTable, pte,
				pageNumber, summary, replacementNotDone, virtualPageTable);
	}
}

void printPageTable(vector<PageTableEntry> &virtualPageTable) {
	for (int pageIndex = 0; pageIndex < _NUM_PAGES; pageIndex++) {
		PageTableEntry& pte = virtualPageTable[pageIndex];
		if (pte.present) {
			cout << pageIndex << ":";
			pte.referenced ? cout << "R" : cout << "-";
			pte.modified ? cout << "M" : cout << "-";
			pte.pagedout ? cout << "S" : cout << "-";
		} else {
			pte.pagedout ? cout << "#" : cout << "*";
		}
		cout << " ";
	}
	cout << endl;
}

void printFrameTable(const InputParams& params, vector<int> &frameTable) {
	for (int frameIndex = 0; frameIndex < params.numFrames; frameIndex++) {
		int pageNumber = frameTable[frameIndex];
		pageNumber == -1 ? cout << "* " : cout << pageNumber << " ";
	}
	cout << endl;
}

void printSummary(SummaryStats summary, int time) {
	unsigned long long totalcost = (summary.tot_maps + summary.tot_unmaps)
			* 400;
	totalcost += (summary.tot_pageins + summary.tot_pageouts) * 3000;
	totalcost += summary.tot_zero * 150;
	totalcost += time;
	printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n", time,
			summary.tot_unmaps, summary.tot_maps, summary.tot_pageins,
			summary.tot_pageouts, summary.tot_zero, totalcost);
}

void runMemoryManager(ifstream &in, const InputParams& params,
		ReplacementPolicy* replacer) {
	int time = 0;
	SummaryStats summary;
	int used_frames = 0;
	vector<int> frameTable(params.numFrames);
	vector<PageTableEntry> virtualPageTable(_NUM_PAGES);

	for (int frameIndex = 0; frameIndex < params.numFrames; frameIndex++)
		frameTable[frameIndex] = -1;

	while (true) {
		//read line
		string line;
		getline(in, line);

		//ignore if comment
		if (line.find("#") == 0)
			continue;

		//read inputs
		istringstream iss(line);
		int isWrite;
		int pageNumber;
		if (!(iss >> isWrite >> pageNumber))
			break;

		//verify virtual page index < 64
		if (pageNumber > 63) {
			//TODO print error
		}

		if (params.printOutput) {
			printf("==> inst: %d %d\n", isWrite, pageNumber);
		}

		PageTableEntry &pte = virtualPageTable[pageNumber];
		allocateFrameToPage(params, frameTable, virtualPageTable, time,
				pageNumber, pte, used_frames, replacer, summary);

		//update modified and referenced bits of page
		if (isWrite) {
			pte.modified = 1;
		}
		pte.referenced = 1;

		time++;			//todo is this correct?

		if (params.printPageTableEveryTime)
			printPageTable(virtualPageTable);

		if (params.printFrameTableEveryTime)
			printFrameTable(params, frameTable);

		fflush(stdout);
	}

	if (params.printPagetable) {
		printPageTable(virtualPageTable);
	}

	if (params.printFrametable) {
		printFrameTable(params, frameTable);
	}

	if (params.printSummary) {
		printSummary(summary, time);
	}
}

ReplacementPolicy* createReplacementPolicy(const InputParams& params) {
	ReplacementPolicy* replacer;
	switch (params.algo) {
	case ReplacementAlgo::FIFO: {
		replacer = new FIFO(params.numFrames);
		break;
	}
	case ReplacementAlgo::NRU: {
		replacer = new NRU(params.randFilePath);
		break;
	}
	case ReplacementAlgo::SecondChance: {
		replacer = new SecondChance(params.numFrames);
		break;
	}
	case ReplacementAlgo::ClockPhysical: {
		replacer = new ClockPhysical(params.numFrames);
		break;
	}
	case ReplacementAlgo::ClockVirtual: {
		replacer = new ClockVirtual();
		break;
	}
	case ReplacementAlgo::LRU: {
		replacer = new LRU(params.numFrames);
		break;
	}
	case ReplacementAlgo::AgingPhysical: {
		replacer = new AgingPhysical();
		break;
	}
	case ReplacementAlgo::AgingVirtual: {
		replacer = new AgingVirtual();
		break;
	}
	case ReplacementAlgo::Random: {
		replacer = new Random(params.randFilePath);
		break;
	}
	}
	return replacer;
}

int main(int argc, char* argv[]) {

	InputParams params(argc, argv);

	ReplacementPolicy* replacer = createReplacementPolicy(params);

	ifstream in(params.inputFilePath);
	if (!in.is_open()) {
		cout << "Input file did not open\n";
		return 0;
	}

	runMemoryManager(in, params, replacer);

	return 0;
}
