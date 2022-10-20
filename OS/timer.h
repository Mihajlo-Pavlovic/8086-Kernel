/*
 * timer.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "def.h"
#include "pcb.h"
#include "thread.h"

void tick();

class Timer{
public:

	static PCB* mainPCB;
	static PCB* idlePCB;
	static PCB* runningPCB;
//	static PCB*

	static InterruptRoutine oldTimerInterrupt;

	static void inic();
	static void restore();
	static void interrupt timerInterrupt(...);



	static int volatile lockTimedOut;
	static int volatile requested;
};



#endif /* TIMER_H_ */
