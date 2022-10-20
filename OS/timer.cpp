/*
 * timer.cpp
 *
 *  Created on: Jun 30, 2019
 *      Author: OS1
 */
#include "timer.h"
#include "thread.h"
#include "def.h"
#include "lock.h"
#include "pcb.h"
#include "kernsem.h"
#include "sleeplst.h"

#include "SCHEDULE.H"
#include <IOSTREAM.H>
#include <dos.h>



PCB* Timer::idlePCB = 0;
PCB* Timer::mainPCB = 0;
PCB* Timer::runningPCB = 0;


InterruptRoutine Timer::oldTimerInterrupt = 0;

int volatile Timer::lockTimedOut = 0;
int volatile Timer::requested = 0;


void Timer::inic(){
	HARD_LOCKED(
		oldTimerInterrupt = getvect(0x08);
		setvect(0x08, timerInterrupt);
	)
}


void Timer::restore(){
	HARD_LOCKED(
		setvect(0x08, oldTimerInterrupt);
	)
}

static Register volatile tsp, tss, tbp;

void interrupt Timer::timerInterrupt(...){
	if(!requested){
		oldTimerInterrupt();
		KernelSem::AllSleepLists->update();
		if(runningPCB -> timeLeft > 0){
			runningPCB->timeLeft--;
			if(runningPCB->timeLeft == 0 && lock && runningPCB->timeSlice != 0) /*lockTimedOut = 1*/
				runningPCB->timeLeft = 1;
		}
	}

	if(requested || (runningPCB->timeSlice != 0 && runningPCB ->timeLeft == 0 && !lock)){

		asm {
				mov tss, ss;
				mov tsp, sp;
				mov tbp, bp;
		}
		runningPCB->ss = tss;
		runningPCB->sp = tsp;
		runningPCB->bp = tbp;
		runningPCB->savedLock = lock;

		if(runningPCB->state == PCB::RUNNING && runningPCB != idlePCB && runningPCB != mainPCB ){
			runningPCB->state = PCB::READY;
			Scheduler::put(runningPCB);
		}

		runningPCB = Scheduler::get();
		if(runningPCB == 0){ runningPCB = idlePCB; /*HARD_LOCKED(cout << "\nidle\n";)*/}
		runningPCB->state = PCB::RUNNING;
		runningPCB->timeLeft = runningPCB->timeSlice;

		tss = runningPCB->ss;
		tsp = runningPCB->sp;
		tbp = runningPCB->bp;

		asm {
			mov ss, tss;
			mov sp, tsp;
			mov bp, tbp;
		}

		lock = runningPCB->savedLock;

		requested = 0;

		//obrada signala
		if(runningPCB->id > 2){
		LOCKED(
			asm sti;
				cout << "Obradjujem signal za " << runningPCB->id << endl;
				int signal = runningPCB->myThread->takeNext();
				while(signal != -1){
					cout << "signal koji se obradjuje: " << signal << " ";
					runningPCB->myThread->processSignal(signal);
					signal = runningPCB->myThread->takeNext();

				}
			asm cli;
		)
		}
	}

}



