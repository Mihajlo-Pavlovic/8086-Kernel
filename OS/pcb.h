/*
 * pcb.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "lock.h"
#include "thread.h"
#include "def.h"
#include "pcblst.h"

class PCB {
public:
	enum State { NEW, READY, RUNNING, BLOCKED, TERMINATED };

	static Word const PSW_INIT;
	static StackSize const MAX_STACK_SIZE;

	static ID totID;

	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread* thread = 0);
	~PCB();

	void reschedule();

	void waitToComplete();

	ID id;

	Thread* myThread;

	StackSize stackCount;
	Word* stack;
	Register volatile ss,sp,bp;

	Time timeSlice;
	Time timeLeft;
	State state;
	int savedLock;

	PCBList waitList;

	int wakeSignal;

	static void threadWrapper();

	static PCBList allPCBs;

	PCBList childrenPCBs;
	PCB* parentPCB;

	Thread::SignalHandlerList handlers[16];


};


#endif /* PCB_H_ */
