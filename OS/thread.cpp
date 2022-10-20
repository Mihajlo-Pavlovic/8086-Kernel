/*
 * thread.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef THREAD_CPP_
#define THREAD_CPP_

#include "def.h"
#include "thread.h"
#include "lock.h"
#include "timer.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "IOSTREAM.H"
int Thread::blockedSignalsGlobaly[16] = {0};

ID Thread::getRunningId(){
	return Timer::runningPCB->id;
}

Thread* Thread::getThreadById(ID id){
	LOCKED(
		Thread* ret = 0;
		PCBList::Node* iter = PCB::allPCBs.head;
		while(iter && iter->pcb->id != id)
			iter = iter->next;
		if(iter) ret = iter->pcb->myThread;
	)
	return ret;
}

ID Thread::getId(){
	return myPCB->id;
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	LOCKED(

		myPCB = new PCB(stackSize, timeSlice, this);

	)

}

Thread::~Thread(){
	LOCKED(
		waitToComplete();
		delete myPCB;
	)
}
void Thread::start(){
	LOCKED(
		myPCB->reschedule();
	)
}

void Thread::waitToComplete(){
	LOCKED(
		myPCB->waitToComplete();
	)
}

void Thread::signal(SignalID signal){
	if(signal<16)
			signalList.push(signal);

}
void Thread::blockSignal(SignalID signal){
	LOCKED(
		blockedSignalsLocaly[signal] = 1;
	)
}

void Thread::unblockSignal(SignalID signal){
	LOCKED(
		blockedSignalsLocaly[signal] = 0;
	)
}

void Thread:: blockSignalGlobally(SignalID signal){
	LOCKED(
		blockedSignalsGlobaly[signal] = 1;
	)
}

void Thread::unblockSignalGlobally(SignalID signal){
	LOCKED(
		blockedSignalsGlobaly[signal] = 0;
	)
}

void Thread::registerHandler(SignalID signal, SignalHandler handler){
		++lock;
		myPCB->handlers[signal].push(handler);
		--lock;
}

void Thread::unregisterAllHandlers(SignalID id){
	myPCB->handlers[id].removeAll();

}

void Thread::swap(SignalID id, SignalHandler hand1, SignalHandler hand2){
	SignalHandlerList::SignalHandlerListElem *temp = myPCB->handlers[id].head, *first = 0, *second = 0;
	while(temp){
		if(temp->handler == hand1)
			first = temp;

		else if(temp->handler == hand2)
			second = temp;
		if(first && second){
			SignalHandler tmp = first->handler;
			first->handler = second->handler;
			second->handler = tmp;
			break;
		}
		temp = temp->next;
	}
}
void Thread::processSignal(SignalID signal){
	SignalHandlerList::SignalHandlerListElem* tmp = myPCB->handlers[signal].head;
	while(tmp){
		tmp->handler();
		tmp = tmp->next;
	}
}
Thread::SignalList::~SignalList(){
	LOCKED(
	SignalList::SignalListElem* e;
	while(head){
		e = head;
		head = head->next;
		delete e;
	}
	)
}
void Thread::SignalList::push(SignalID signalID){
	SignalListElem* temp = new SignalListElem(signalID);
	if(head)
		tail->next = temp;
	else
		head = temp;
	tail = temp;
}

void Thread::SignalList::remove(SignalListElem* elem){
	SignalListElem *cur = head, *prev = 0;
	while(cur && cur != elem){
		prev = cur;
		cur = cur->next;
	}
	if(cur){
		if(cur != head)
			prev->next = cur->next;
		else
			head = head->next;
		if(cur == tail)
			tail = prev;
		HARD_LOCKED(
			delete cur;
		)
	}
}
int Thread::SignalList::takeNext(){
	int ret = -1;
	SignalListElem* iter = head;
	while(iter){
		if(Timer::runningPCB->myThread->blockedSignalsLocaly[iter->signalID] || Thread::blockedSignalsGlobaly[iter->signalID]){
			iter = iter->next;
		}
		else{
			ret = iter->signalID;
			remove(iter);
			break;
		}
	}
	return ret;
}

Thread::SignalHandlerList::~SignalHandlerList(){
	LOCKED(
			SignalHandlerList::SignalHandlerListElem* e;
		while(head){
			e = head;
			head = head->next;
			delete e;
		}
	)
}

void Thread::SignalHandlerList::push(SignalHandler handler){
	LOCKED(
		SignalHandlerListElem* temp = new SignalHandlerListElem(handler);
			if(head)
				tail->next = temp;
			else
				head = temp;
			tail = temp;
	)
}

void Thread::SignalHandlerList::removeAll(){
	SignalHandlerListElem *iter = head, *temp;
	while(iter){
		temp = iter;
		iter = iter->next;
		HARD_LOCKED(
			delete temp;
		)
	}
	head = tail = 0;
}

void Thread::signal0(){
	HARD_LOCKED(
	Timer::runningPCB->state = PCB::TERMINATED;
	Timer::runningPCB->waitList.empty();
	/*Timer::runningPCB->parentPCB->myThread->signal(1);*/
	)
	dispatch();
}



void dispatch(){
	HARD_LOCKED(
		Timer::requested = 1;
		Timer::timerInterrupt();
	)
}

#endif /* THREAD_CPP_ */
/*for(int i = 0; i < 16; i++){
	SignalHandlerList::SignalHandlerListElem* iter = myPCB->parentPCB->myThread->handlers[i].head;
	SignalHandlerList::SignalHandlerListElem* prev = 0;
	while(iter){
		if(!handlers[i].head)
			prev =  handlers[i].head = SignalHandlerList::SignalHandlerListElem::SignalHandlerListElem(iter->handler);
		else{
			prev->next = SignalHandlerList::SignalHandlerListElem::SignalHandlerListElem(iter->handler);
			prev = prev->next;
		}

		iter = iter->next;
	}
	handlers[i].tail = prev;
}*/
