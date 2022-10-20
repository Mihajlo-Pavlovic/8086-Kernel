/*
 * main.cpp
 *
 *  Created on: May 23, 2019
 *      Author: OS1
 */

#include "idle.h"
#include "timer.h"
#include "pcb.h"
#include "mainth.h"
#include "lock.h"
#include "iostream.h"


int main(int argc, char* argv[]) {

	Timer::mainPCB = new PCB(defaultStackSize,20,0);
	Timer::runningPCB = Timer::mainPCB;
	Idle* idle = new Idle();
	MainThread* mainThread = new MainThread(argc, argv);

	idle->start();
	mainThread -> start();

	Timer::inic();
	dispatch();
	Timer::restore();
	cout << "ende";
	return 1;
}


