/*
 * Replacers.h
 *
 *  Created on: Apr 15, 2015
 *      Author: Kshitiz
 */

#ifndef REPLACERS_H_
#define REPLACERS_H_

#include "helpers.h"

#include <bitset>
#include <vector>
#include <iostream>
#include <climits>

using namespace std;

class ReplacementPolicy {
public:
	virtual int getFrameToBeReplaced(vector<int> &frameTable,
			int frameTableSize, vector<PageTableEntry> &virtualPageTable)=0;
};

class FIFO: public ReplacementPolicy {
protected:

	int rotateIndices() {
		int result = frameIndices[0];
		frameIndices.erase(frameIndices.begin());
		frameIndices.push_back(result);
		return result;
	}

	/*int rotateIndices(int firstInvalidFrame) {
		int result = frameIndices[0];
		frameIndices.erase(frameIndices.begin());
		frameIndices.insert(frameIndices.begin()+firstInvalidFrame , result);
		return result;
	}*/

public:
	vector<int> frameIndices;

	FIFO(int frameSize) {
		for (int frameIndex = 0; frameIndex < frameSize; frameIndex++) {
			frameIndices.push_back(frameIndex);
		}
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {
		return rotateIndices();
	}
};

class ClockPhysical: public FIFO {
public:
	ClockPhysical(int frameSize) :
			FIFO(frameSize) {
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {
		int result = rotateIndices();
		PageTableEntry *pte = &(virtualPageTable[frameTable[result]]);
		while (pte->referenced) {
			pte->referenced = 0;
			result = rotateIndices();
			pte = &(virtualPageTable[frameTable[result]]);
		}
		//elementToBeReplaced = (result + 1) % frameTable.size();
		//rotateFrameIndices();//todo needed?
		return result;
	}
};

class ClockVirtual: public ClockPhysical {
public:
	ClockVirtual() :
			ClockPhysical(_NUM_PAGES) {
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {
		/*int result = rotateFrameIndices();
		 PageTableEntry *pte = &(virtualPageTable[frameTable[result]]);
		 while (pte->referenced) {
		 pte->referenced = 0;
		 result = rotateFrameIndices();
		 pte = &(virtualPageTable[frameTable[result]]);
		 }
		 //elementToBeReplaced = (result + 1) % frameTable.size();
		 //rotateFrameIndices();//todo needed?*/
		bool done = false;
		int pageIndex = -1;
		while (!done) {
			pageIndex = rotateIndices();
			PageTableEntry &pte = virtualPageTable[pageIndex];
			if (pte.present) {
				if (pte.referenced)
					pte.referenced = 0;
				else {
					done = true;
				}
			}
		}

		return virtualPageTable[pageIndex].frameIndex;
	}
};

class LRU: public FIFO {
protected:

public:
	LRU(int frameSize) :
			FIFO(0) {//todo hacky change
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {
		return rotateIndices();
		printIndices();
	}

	void printIndices() {
		/*cout << "Indices: ";
		for (int index : frameIndices) {
			printf("%d ", index);
		}
		cout << endl;*/
	}
};

class AgingPhysical: public ReplacementPolicy {
protected:
	typedef bitset<32> age_field;
	vector<age_field> ages;

public:
	AgingPhysical() :
			ages(_NUM_PAGES, 0) {
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {

		for (int pageIndex = 0; pageIndex < ages.size(); pageIndex++) {
			ages[pageIndex] = (ages[pageIndex] >> 1);
			ages[pageIndex].set(31, virtualPageTable[pageIndex].referenced);
			virtualPageTable[pageIndex].referenced = 0;
			/*if (virtualPageTable[pageIndex].present)
			 printf("%d:%lu ", pageIndex, ages[pageIndex].to_ulong());*/
		}
		//cout << endl;

		int minFrameIndex = 0;
		int minIndex = frameTable[minFrameIndex];
		for (int frameIndex = 0; frameIndex < frameTable.size(); frameIndex++) {
			if (/*virtualPageTable[frameTable[frameIndex]].present
			 && */ages[minIndex].to_ulong()
					> ages[frameTable[frameIndex]].to_ulong()) {
				minIndex = frameTable[frameIndex];
				minFrameIndex = frameIndex;
			}
		}
		/*printf("min:  %d:%lu\n", frameTable[minFrameIndex],
		 ages[frameTable[minFrameIndex]]);*/

		//clear age counter
		ages[minIndex].reset();

		return minFrameIndex;
	}
};

class AgingVirtual: public AgingPhysical {
public:
	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {

		for (int pageIndex = 0; pageIndex < ages.size(); pageIndex++) {
			ages[pageIndex] = (ages[pageIndex] >> 1);
			ages[pageIndex].set(31, virtualPageTable[pageIndex].referenced);
			if (virtualPageTable[pageIndex].present)
				virtualPageTable[pageIndex].referenced = 0;
			/*if (virtualPageTable[pageIndex].present)
			 printf("%d:%lu ", pageIndex, ages[pageIndex].to_ulong());*/
		}
		//cout << endl;

		//int minFrameIndex = 0;
		int minIndex = -1;		//frameTable[minFrameIndex];
		unsigned long minAge = UINT_MAX;
		for (int pageIndex = 0; pageIndex < virtualPageTable.size();
				pageIndex++) {
			if (virtualPageTable[pageIndex].present
					&& minAge > ages[pageIndex].to_ulong()) {
				minIndex = pageIndex;
				minAge = ages[pageIndex].to_ulong();
			}
		}
		//("min:  %d:%lu\n", minIndex, minAge);
		//fflush (stdout);

		//clear age counter
		ages[minIndex].reset();

		return virtualPageTable[minIndex].frameIndex;
	}
};

class SecondChance: public FIFO {
public:
	SecondChance(int frameSize) :
			FIFO(frameSize) {
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {

		PageTableEntry *pte;
		int chosenIndex = -1;
		bool done = false;
		while (!done) {
			chosenIndex = rotateIndices();
			pte = &(virtualPageTable[frameTable[chosenIndex]]);
			if (pte->referenced) {
				pte->referenced = 0;
			} else {
				done = true;
			}
		}

		return chosenIndex;
	}
};

class NRU: public ReplacementPolicy {
protected:
	ifstream randFile;
	int counter = 0;
	int getRandFrame(vector<PageTableEntry> pageTable) {

		int randomNum = getNextRandomNum();
		int index = randomNum % pageTable.size();
		PageTableEntry &pte = pageTable[index];
		/*printf(
		 "-----page table size: %d, random number = %d, pageIndex = %d, frameIndex = %d\n",
		 pageTable.size(), randomNum, index, pte.frameIndex);*/
		return pte.frameIndex;
	}

	~NRU() {
		randFile.close();
	}

	int getNextRandomNum() {
		int randomNum;
		randFile >> randomNum;
		if (!randFile) {
			randFile.clear();
			randFile.seekg(ios_base::beg);
			//discard first line
			int crap;
			randFile >> crap;
			randFile >> randomNum;
		}
		return randomNum;
	}

public:

	NRU(string randFilePath) :
			randFile(randFilePath) {
		//discard first line
		int crap;
		randFile >> crap;
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {

		//map pages to classes
		vector<PageTableEntry> class0, class1, class2, class3;
		for (int pageIndex = 0; pageIndex < _NUM_PAGES; pageIndex++) {
			PageTableEntry &pte = virtualPageTable[pageIndex];

			if (!pte.present)
				continue;

			if (!pte.referenced && !pte.modified) {
				class0.push_back(pte);
			} else if (!pte.referenced && pte.modified) {
				class1.push_back(pte);
			} else if (pte.referenced && !pte.modified) {
				class2.push_back(pte);
			} else {
				class3.push_back(pte);
			}
		}

		//randomly choose page from lowest unempty class
		int frameToBeReplaced = -1;
		if (!class0.empty()) {
			//printf("-----Class 0 chosen\n");
			frameToBeReplaced = getRandFrame(class0);
		} else if (!class1.empty()) {
			//printf("-----Class 1 chosen\n");
			frameToBeReplaced = getRandFrame(class1);
		} else if (!class2.empty()) {
			//printf("-----Class 2 chosen\n");
			frameToBeReplaced = getRandFrame(class2);
		} else if (!class3.empty()) {
			//printf("-----Class 3 chosen\n");
			frameToBeReplaced = getRandFrame(class3);
		}

		counter++;
		if (counter == 10) {
			counter = 0;
			for (int pageIndex = 0; pageIndex < _NUM_PAGES; pageIndex++) {
				PageTableEntry &pte = virtualPageTable[pageIndex];
				if (pte.present)
					pte.referenced = 0;
			}
		}

		return frameToBeReplaced;
	}
};

class Random: public NRU {
public:
	Random(string randFilePath) :
			NRU(randFilePath) {
	}

	int getFrameToBeReplaced(vector<int> &frameTable, int frameTableSize,
			vector<PageTableEntry> &virtualPageTable) override {

		int randomNum = getNextRandomNum();
		int index = randomNum % frameTable.size();
		return index;
	}
};

#endif /* REPLACERS_H_ */
