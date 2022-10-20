/*
 * slstall.cpp
 *
 *  Created on: Jul 12, 2019
 *      Author: OS1
 */


#include "slstall.h"

SleepLstList::SleepLstList() { head = tail = 0; }

void SleepLstList::add(SleepList* lst){
	SleepElem* temp = new SleepElem(lst);
	if (head)
		tail->next = temp;
	else
		head = temp;
	tail = temp;
}

void SleepLstList::update(){
	SleepElem* iter = head;
	while(iter){
		iter->lst->update();
		iter = iter->next;
	}
}
