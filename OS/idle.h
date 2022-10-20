/*
 * idle.h
 *
 *  Created on: May 19, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "def.h"
#include "pcb.h"

class Idle : public Thread{
public:

	Idle();
	virtual void run();
};



#endif /* IDLE_H_ */
