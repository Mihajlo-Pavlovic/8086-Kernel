/*
 * slstall.h
 *
 *  Created on: Jul 12, 2019
 *      Author: OS1
 */

#ifndef SLSTALL_H_
#define SLSTALL_H_

#include "sleeplst.h"
#include "kernsem.h"

class SleepList;

class SleepLstList{
public:
	struct SleepElem{
		SleepList* lst;
		SleepElem* next;
		SleepElem(SleepList* lst) : lst(lst), next(0) {}
	};

	SleepLstList();

	void add(SleepList* lst);
	void update();


	SleepElem *head, *tail;
};



#endif /* SLSTALL_H_ */
