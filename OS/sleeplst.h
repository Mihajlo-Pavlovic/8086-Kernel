/*
 * sleeplst.h
 *
 *  Created on: Jul 12, 2019
 *      Author: OS1
 */

#ifndef SLEEPLST_H_
#define SLEEPLST_H_

#include "pcb.h"
#include "kernsem.h"

class SleepList{
public:
	struct SleepLstElem{
		PCB* pcb;
		SleepLstElem* next;
		Time waitTime;
		KernelSem* mySem;
		SleepLstElem(PCB* pcb, Time waitTime, KernelSem* mySem) : pcb(pcb), next(0), waitTime(waitTime), mySem(mySem) {}

	};
	SleepList(KernelSem* mySem);


	KernelSem* mySem;
	SleepLstElem* headFinitie;
	SleepLstElem* headUnlimited;
	SleepLstElem* tailUnlimited;

	void remove(int n);
	void add(PCB* pcb, Time wait,KernelSem* mySem);
	void update();
};



#endif /* SLEEPLST_H_ */
