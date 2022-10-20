/*
 * pcblst.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */
#include "pcblst.h"
#include "lock.h"
#include "pcb.h"
#include "timer.h"
#include "schedule.h"

PCBList::PCBList() : head(0), tail(0) {}

PCBList::~PCBList(){
	LOCKED(
		while(pop());
	)
}

void PCBList::push(PCB* pcb){

	LOCKED(
		Node* tmp = new Node(pcb);
		if(!head)
			head = tmp;
		else
			tail -> next = tmp;
		tail = tmp;
	)
}

PCB* PCBList::pop(){
	LOCKED(
		PCB* ret = 0;
		if(head){
			ret = head->pcb;
			Node* tmp = head;
			head = head->next;
			if(!head) tail = 0;
			tmp->pcb = 0;
			HARD_LOCKED(
				delete tmp;
			)
		}
	)
	return ret;
}

void PCBList::empty(){
	LOCKED(
		for(PCB* p; p = pop(); p->reschedule());
	)
}

void PCBList::remove(PCB* pcb){
	LOCKED(
		if(head){
			Node* cur = head;
			Node* prev = 0;
			while(cur && cur->pcb != pcb){
				cur = cur->next;
				prev = cur;
			}
			if(cur){
				if(cur != head){
					prev -> next = cur -> next;
				}
				if(cur == tail){
					tail = prev;
				}
				if(cur == head){
					head = head->next;
				}
				cur -> pcb = 0;
				HARD_LOCKED(
					delete cur;
				)
			}
		}
	)
}
