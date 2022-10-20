/*
 * ivt.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: OS1
 */

#include "ivt.h"
#include "lock.h"

#include <dos.h>

IVTEntry* IVTEntry::table[256] = {0};

IVTEntry* IVTEntry::get(IVTNo ivtNo) { return table[ivtNo]; }

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine newInterrupt)	: ivtNo(ivtNo), newInterrupt(newInterrupt), event(0)
{
	table[ivtNo] = this;
}

void IVTEntry::setEvent(KernelEv* event){
	this->event = event;
	HARD_LOCKED(
		oldInterrupt = getvect(ivtNo);
		setvect(ivtNo, newInterrupt);
	)
}

void IVTEntry::restore(){
	event = 0;
	HARD_LOCKED(
		setvect(ivtNo, oldInterrupt);
	)
}
