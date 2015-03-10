/*
 * rr.h
 *
 *  Created on: Mar 8, 2015
 *      Author: Kshitiz
 */

#ifndef RR_H_
#define RR_H_

class RRScheduler:public FCFScheduler{
public:
	RRScheduler(InputParams params):FCFScheduler(params){
	}
};

#endif /* RR_H_ */
