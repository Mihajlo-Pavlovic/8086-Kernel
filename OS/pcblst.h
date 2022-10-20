/*
 * pcblst.h
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */

#ifndef PCBLST_H_
#define PCBLST_H_

#include "pcblst.h"

class PCB;
class KernelSem;

class PCBList{
public:
	PCBList();
	~PCBList();

	void push(PCB* pcb);
	PCB* pop ();

	void remove(PCB* pcb);

	void empty();

	struct Node{
		PCB* pcb;
		Node* next;

		Node(PCB* pcb, Node* next = 0) : pcb(pcb), next(next) { }
	};

	Node* head;
	Node* tail;
 };




#endif /* PCBLST_H_ */
