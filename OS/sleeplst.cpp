/*
 * sleeplst.cpp
 *
 *  Created on: Jul 12, 2019
 *      Author: OS1
 */

#include "sleeplst.h"
#include "timer.h"
#include "lock.h"
#include "slstall.h"
#include "kernsem.h"
#include "SCHEDULE.H"

#include "IOSTREAM.H"



SleepList::SleepList(KernelSem* mySem): mySem(mySem){
	headUnlimited = tailUnlimited = 0;
	headFinitie = 0;
	KernelSem::AllSleepLists->add(this);
}

void SleepList::add(PCB* pcb, Time wait, KernelSem* mySem){
	/*LOCKED(*/
		HARD_LOCKED(SleepLstElem* temp = new SleepLstElem(pcb, wait, mySem);)
		if(!wait){
			temp->waitTime = -1;
			if(headUnlimited)
				tailUnlimited->next = temp;
			else
				headUnlimited = temp;
			tailUnlimited = temp;
		}
		else{
			if(!headFinitie)
				headFinitie = temp;
			else{
				SleepLstElem *cur = headFinitie, *prev= 0;
				if(wait < headFinitie->waitTime){
					temp->next = headFinitie;
					headFinitie->waitTime -= temp->waitTime;
					headFinitie = temp;
				}
				else{
					while(cur->next && temp->waitTime >= cur->waitTime){
						temp->waitTime -= cur->waitTime;
						prev = cur;
						cur = cur->next;
					}
					if(cur->next || cur->waitTime > temp->waitTime){
						cur->waitTime -= temp->waitTime;
						temp->next = cur;
						prev->next = temp;
					}
					else{
						temp->waitTime -= cur->waitTime;
						cur->next = temp;
					}
				}
			}
		}
/*	)*/
}

void SleepList::remove(int n){
	while( n > 0){
		SleepLstElem* e = 0;
		if(headUnlimited){
			e = headUnlimited;
			headUnlimited = headUnlimited->next;
			n--;
			e->pcb->wakeSignal = 1;
			e->pcb->reschedule();
			e->pcb = 0;
			HARD_LOCKED(
				delete e;
			)
			continue;
		}
		else if(headFinitie){
			e = headFinitie;
			headFinitie = headFinitie->next;
			headFinitie->waitTime += e->waitTime;
			n--;
			e->pcb->wakeSignal = 1;
			e->pcb->reschedule();
			e->pcb = 0;
			HARD_LOCKED(
				delete e;
			)
			continue;
		}
		else
			return;
	}
}


void SleepList::update(){
	if(headFinitie){
		headFinitie->waitTime--;
		SleepLstElem* temp = 0;
		while(headFinitie && headFinitie->waitTime == 0){
			temp = headFinitie;
			temp->pcb->reschedule();
			temp->mySem->incVal();
			headFinitie = headFinitie->next;
			temp->pcb = 0;
			temp->mySem = 0;
			HARD_LOCKED( delete temp; )
		}
	}
}
