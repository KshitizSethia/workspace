/*
 * Algo.h
 *
 *  Created on: Apr 28, 2015
 *      Author: Kshitiz
 */

#ifndef ALGO_H_
#define ALGO_H_

#include <fstream>
#include <deque>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "helpers.h"

using namespace std;

class Algo {
protected:
	//config
	ifstream input;
	const bool verbose;
	SchedulingAlgo algo;
	//state
	unsigned long numReq;
	vector<IORequest> pendingQueue, readyQueue, doneQueue;
	unsigned long time = 0, trackPos = 0;
	bool travelingToTargetTrack = false;
	bool travelingUp = true;
	//summary
	unsigned long tot_movement = 0, tot_turnaround_time = 0, tot_wait_time = 0,
			max_wait_time = 0;

	virtual void putReadyRequest(IORequest req) {
		readyQueue.push_back(req);
	}

	virtual IORequest getReadyRequest() {
		IORequest req = readyQueue[0];
		readyQueue.erase(readyQueue.begin());
		return req;
	}

	virtual void swapQueues() {
	}
	;

	IORequest dequeuePending() {
		IORequest result = pendingQueue[0];
		pendingQueue.erase(pendingQueue.begin());
		return result;
	}
public:
	Algo(InputParams params) :
			input(params.inputFilePath), verbose(params.verbose), algo(
					params.algo) {
		//read all io requests
		while (true) {
			string data;
			getline(input, data);
			if (data.find("#") == 0)
				continue;

			istringstream line(data);
			int time, trackNum;
			if (!(line >> time >> trackNum))
				break;
			IORequest req(time, trackNum);
			pendingQueue.push_back(req);
		}
		numReq = pendingQueue.size();
	}

	void simulate() {
		IORequest ready(0, 0);

		if (verbose)
			printf("TRACE\n");

		while (doneQueue.size() != numReq) {
			if (!travelingToTargetTrack) { // && !readyQueue.empty()) {
				if (!readyQueue.empty()) {
					//get a ready request and process it
					ready = getReadyRequest();
					travelingToTargetTrack = true;
					ready.issueTime = time;
					if (verbose)
						printf("%d:%6d issue %d %d\n", time, ready.id,
								ready.track, trackPos);

					//update summary stats
					unsigned int wait_time = time - ready.arrivalTime;
					unsigned int movement = abs(int(ready.track - trackPos)); //todo long int here?
					if (max_wait_time < wait_time)
						max_wait_time = wait_time;
					tot_movement += movement;
					tot_wait_time += wait_time;
				} else {
					//ready queue is empty and no request in progress
					IORequest req = dequeuePending();
					time = req.arrivalTime;
					if (verbose)
						printf("%d:%6d add %d\n", time, req.id, req.track);
					putReadyRequest(req);
				}
			}

			if (travelingToTargetTrack) {
				int eta = time + abs(int(ready.track - trackPos));

				//transfer requests which arrive before eta to ready
				while (!pendingQueue.empty()
						&& pendingQueue.front().arrivalTime <= eta) {
					IORequest pending = dequeuePending();

					time = pending.arrivalTime;
					if (verbose)
						printf("%d:%6d add %d\n", time, pending.id,
								pending.track);
					putReadyRequest(pending);
				}

				//arrive at track for current ready request
				travelingToTargetTrack = false;

				//update state
				time = eta;
				if (trackPos < ready.track)
					travelingUp = true;
				else if (trackPos > ready.track)
					travelingUp = false;
				trackPos = ready.track;
				ready.finishTime = time;
				doneQueue.push_back(ready);

				//update summary stats
				tot_turnaround_time += ready.finishTime - ready.arrivalTime;

				if (verbose) {
					printf("%d:%6d finish %d\n", time, ready.id,
							time - ready.arrivalTime);
				}
			}

			if (algo == SchedulingAlgo::fscan && readyQueue.empty()) {
				swapQueues();
				travelingUp = true;
			}
			flush(cout);
		}
	}

	void printSummary() {
		if (verbose) {
			auto sorter = [](IORequest p1, IORequest p2) {return p1.id<p2.id;};
			sort(doneQueue.begin(), doneQueue.end(), sorter);
			printf("IOREQS INFO\n");
			for (IORequest req : doneQueue) {
				printf("%5d:%6d%6d%6d\n", req.id, req.arrivalTime,
						req.issueTime, req.finishTime);
			}
		}
		printf("SUM: %d %d %.2lf %.2lf %d\n", time, tot_movement,
				((double) tot_turnaround_time / numReq),
				((double) tot_wait_time / numReq), max_wait_time);
	}
};

class FIFO: public Algo {
public:
	FIFO(InputParams params) :
			Algo(params) {
	}
};

class SSTF: public Algo {
public:
	SSTF(InputParams params) :
			Algo(params) {
	}

	IORequest getReadyRequest() override {
		auto sorter = [&](IORequest r1, IORequest r2) {
			long dist1 = abs((long)(r1.track-trackPos));
			long dist2 = abs((long)(r2.track-trackPos));
			return dist1==dist2? r1.id<r2.id:dist1<dist2;
		};

		sort(readyQueue.begin(), readyQueue.end(), sorter);

		return Algo::getReadyRequest();
	}
};

class SCAN: public Algo {
public:
	SCAN(InputParams params) :
			Algo(params) {
	}

	IORequest getReadyRequest() override {
		if (travelingUp) {
			IORequest result(0, 0);
			auto sorter = [](IORequest r1, IORequest r2) {
				return r1.track==r2.track?
				r1.id<r2.id
				:r1.track<r2.track;
			};
			sort(readyQueue.begin(), readyQueue.end(), sorter);

			//choose element from this list
			int indexToErase = -1;
			for (IORequest req : readyQueue) {
				result = req;
				indexToErase++;
				if (result.track >= trackPos)
					break;

			}
			readyQueue.erase(readyQueue.begin() + indexToErase);

			return result;
		} else {
			IORequest result(0, 0);
			auto sorter = [](IORequest r1, IORequest r2) {
				return r1.track==r2.track?
				r1.id<r2.id
				:r1.track>r2.track;
			};
			sort(readyQueue.begin(), readyQueue.end(), sorter);

			//choose element from this list
			int indexToErase = -1;
			for (IORequest req : readyQueue) {
				result = req;
				indexToErase++;
				if (result.track <= trackPos)
					break;
			}
			readyQueue.erase(readyQueue.begin() + indexToErase);

			return result;
		}
	}

};

class CSCAN: public Algo {
public:
	CSCAN(InputParams params) :
			Algo(params) {
	}

	IORequest getReadyRequest() override {
		IORequest result(0, 0);
		auto sorter = [](IORequest r1, IORequest r2) {
			return r1.track==r2.track?
			r1.id<r2.id
			:r1.track<r2.track;
		};
		sort(readyQueue.begin(), readyQueue.end(), sorter);

		//choose element from this list
		int indexToErase = -1;
		for (IORequest req : readyQueue) {
			result = req;
			indexToErase++;
			if (result.track >= trackPos)
				break;

		}
		if (result.track < trackPos) {
			indexToErase = 0;
			result = readyQueue[0];
		}
		readyQueue.erase(readyQueue.begin() + indexToErase);

		return result;
	}
};

class FSCAN: public SCAN {
protected:
	void swapQueues() override {
		readyQueue = vector<IORequest>(waitQueue);
		waitQueue.clear();
	}
public:
	vector<IORequest> waitQueue;
	FSCAN(InputParams params) :
			SCAN(params) {
	}

	void putReadyRequest(IORequest req) override {
		if (travelingToTargetTrack) {
			waitQueue.push_back(req);
		} else {
			readyQueue.push_back(req);
		}
	}

	IORequest getReadyRequest() override {
		if (readyQueue.empty()) {
			swapQueues();
		}
		return SCAN::getReadyRequest();
	}
};

#endif /* ALGO_H_ */
