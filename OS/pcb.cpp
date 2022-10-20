/*
 * pcb.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include "lock.h"
#include "timer.h"
#include "pcblst.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <IOSTREAM.H>

Word      const PCB::PSW_INIT = 0x0200; // I=1
StackSize const PCB::MAX_STACK_SIZE = 0x1000; // 64KB

ID PCB::totID = 0;
PCBList PCB::allPCBs;


PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) : myThread(thread), stack(0), timeSlice(timeSlice), timeLeft(timeSlice), state(NEW), savedLock(0), id(++totID), wakeSignal(1)
{
	LOCKED(
	if(stackSize > MAX_STACK_SIZE) stackSize = MAX_STACK_SIZE;
	stackCount = stackSize / sizeof(Word);

	if (thread){
		stack = new Word[stackCount];
		stack[stackCount - 1] = PSW_INIT;
		stack[stackCount - 2] = FP_SEG(threadWrapper);
		stack[stackCount - 3] = FP_OFF(threadWrapper);

		ss = FP_SEG(stack + stackCount - 12);
		sp = bp = FP_OFF(stack + stackCount - 12);
	}
	if(id == 1) Timer::mainPCB = this;
	if(id == 2) Timer::idlePCB = this;

	allPCBs.push(this);

	parentPCB = Timer::runningPCB;
	parentPCB->childrenPCBs.push(this);


	if(id > 1){
		for(int i = 0; i < 16; i++){
			Thread::SignalHandlerList::SignalHandlerListElem* iter = parentPCB->handlers[i].head;
			while(iter){
				handlers[i].push(iter->handler);
				iter = iter->next;
			}
			if(parentPCB->id == 1)
				myThread->blockedSignalsLocaly[i] = 0;
			else
				myThread->blockedSignalsLocaly[i] = parentPCB->myThread->blockedSignalsLocaly[i];
		}
	}
	else{
		handlers[0].push(Thread::signal0);
	}
	)
}
PCB::~PCB(){
	waitList.empty();
	if(stack) delete[] stack;
}

void PCB::reschedule() {
	LOCKED(
		if(this != Timer::idlePCB){
			state = READY;
			Scheduler::put(this);
		}
	)
}

void PCB::waitToComplete(){
	LOCKED(
		if(Timer::runningPCB != this && state != TERMINATED && this !=Timer::idlePCB && state != NEW){
			Timer::runningPCB->state = BLOCKED;
			waitList.push(Timer::runningPCB);
			dispatch();
		}
	)
}

void PCB::threadWrapper(){
	Timer::runningPCB->myThread->run();

	LOCKED(
		Timer::runningPCB->state = TERMINATED;
		Timer::runningPCB->parentPCB->myThread->signal(1);
		Timer::runningPCB->waitList.empty();

		dispatch();
	)
}

