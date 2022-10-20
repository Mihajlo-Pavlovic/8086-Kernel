/*
 * semaphor.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */
#include "semaphor.h"
#include "lock.h"
#include "kernsem.h"
#include "timer.h"

Semaphore::Semaphore(int init){

		myImpl = new KernelSem(init);
}

Semaphore::~Semaphore(){

		delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){
	LOCKED(
			int ret = myImpl->wait(maxTimeToWait);
	)
	return ret;
}

int Semaphore::signal(int n ){
	LOCKED(
		int ret = myImpl->signal(n);
	)
	return ret;
}

int Semaphore::val() const {
	LOCKED(
			int ret = myImpl->getVal();
	)
	return ret;
}
