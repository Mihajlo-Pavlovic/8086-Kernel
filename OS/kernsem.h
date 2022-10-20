/*
 * kernsem.h
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "sleeplst.h"
#include "slstall.h"

class SleepList;
class SleepLstList;


class KernelSem {
public:

	static SleepLstList* AllSleepLists;

	KernelSem(int init);
	~KernelSem();
	int wait(Time maxTimeToWait);
	int signal(int n = 0);

	int getVal() const { return val; }

	void incVal() { val++; }


	SleepList* lst;



private:
	int val;

};


#endif /* KERNSEM_H_ */
