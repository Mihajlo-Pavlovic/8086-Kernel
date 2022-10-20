/*
 * event.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: OS1
 */

#include "event.h"
#include "lock.h"
#include "kernelev.h"

Event::Event(IVTNo ivtNo){
	LOCKED(
		myImpl = new KernelEv(ivtNo);
	)
}

Event::~Event(){
	LOCKED(
		delete myImpl;
	)
}

void Event::wait(){
	LOCKED(
		myImpl->wait();
	)

}

void Event::signal(){
	LOCKED(
		myImpl->signal();
	)
}

