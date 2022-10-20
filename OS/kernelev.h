/*
 * kerenelev.h
 *
 *  Created on: Jul 14, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "kernsem.h"
#include "pcb.h"


typedef unsigned char IVTNo;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	IVTNo ivtNo;
	PCB* pcb;
	KernelSem sem;
};



#endif /* KERNELEV_H_ */
