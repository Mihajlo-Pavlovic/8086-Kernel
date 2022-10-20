/*
 * kernsem.cpp
 *
 *  Created on: May 23, 2019
 *      Author: OS1
 */
#include "def.h"
#include "kernsem.h"
#include "lock.h"
#include "timer.h"
#include "schedule.h"
#include "pcb.h"
#include "pcblst.h"
#include "sleeplst.h"



SleepLstList* KernelSem::AllSleepLists = new SleepLstList();

KernelSem::KernelSem(int init) : val(init) { lst = new SleepList(this); }

KernelSem::~KernelSem(){
}
int KernelSem::wait(Time maxTimeToWait) {
	Timer::runningPCB->wakeSignal = 1;
	LOCK;
			if(--val < 0) {
				Timer::runningPCB->wakeSignal = 0;
				Timer::runningPCB->state = PCB::BLOCKED;
				lst->add(Timer::runningPCB, maxTimeToWait, this);
			UNLOCK
				dispatch();
			}
			else{
				UNLOCK;
			}
		return Timer::runningPCB->wakeSignal;
}

int KernelSem::signal(int n){
	LOCK;
			if( n < 0){
				UNLOCK;
				return n;
			}
			int ret = 0;
			if (n == 0) n = 1;
			while(n){
				if(++val <= 0)
					lst->remove(1);
				else break;
				ret++;
				n--;
			}
	UNLOCK;
	return ret;

}


