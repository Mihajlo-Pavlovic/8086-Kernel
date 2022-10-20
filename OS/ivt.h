/*
 * ivt.h
 *
 *  Created on: Jul 14, 2019
 *      Author: OS1
 */

#include "kernelev.h"
#include "def.h"


class IVTEntry{
public:
	static IVTEntry* get(IVTNo ivtNo);

	IVTEntry(IVTNo ivtNo, InterruptRoutine newInterrupt);
	~IVTEntry() { restore(); }

	void signal() { event->signal(); }
	void setEvent(KernelEv* event);
	void restore();

	InterruptRoutine oldInterrupt;
	InterruptRoutine newInterrupt;

private:
	static IVTEntry* table[];

	IVTNo ivtNo;
	KernelEv* event;
};


#define PREPAREENTRY(ivtNo, callOld)                    \
extern IVTEntry ivtEntry##ivtNo;                        \
                                                        \
void interrupt eventInterrupt##ivtNo(...) {             \
    ivtEntry##ivtNo.signal();                           \
    if (callOld) ivtEntry##ivtNo.oldInterrupt();        \
    dispatch();                                         \
}                                                       \
                                                        \
IVTEntry ivtEntry##ivtNo(ivtNo, eventInterrupt##ivtNo);

