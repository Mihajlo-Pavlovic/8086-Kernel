/*
 * kernelev.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: OS1
 */

#include "kernelev.h"
#include "ivt.h"
#include "timer.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), pcb(Timer::runningPCB), sem(0){
	IVTEntry::get(ivtNo)->setEvent(this);
}

KernelEv::~KernelEv(){
	IVTEntry::get(ivtNo)->restore();
}

void KernelEv::wait(){
	LOCKED(
		if(Timer::runningPCB == pcb)
			sem.wait(0);
	)
}

void KernelEv::signal(){
	LOCKED(
		sem.signal();
	)
}
